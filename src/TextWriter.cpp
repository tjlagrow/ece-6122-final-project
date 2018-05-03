#include "TextWriter.h"
#include <algorithm> // std::max

#define MAXWIDTH  1024

extern int verbose;

static const GLchar *uniform_sampler = "sampler";
static const GLchar *uniform_vcoord = "vcoord";
static const GLchar *uniform_color = "inColor";

struct point
{
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

/**
 * The atlas struct holds a texture that contains the visible US-ASCII characters
 * of a certain font rendered with a certain character height.
 * It also contains an array that contains all the information necessary to
 * generate the appropriate vertex and texture coordinates for each character.
 *
 * After the constructor is run, you don't need to use any FreeType functions anymore.
 */
struct atlas
{
	GLuint tex; // texture object

	unsigned int w; // width of texture in pixels
	unsigned int h; // height of texture in pixels

	struct
	{
		float ax;    // advance.x
		float ay;    // advance.y

		float bw;    // bitmap.width;
		float bh;    // bitmap.height;

		float bl;    // bitmap_left;
		float bt;    // bitmap_top;

		float tx;    // x offset of glyph in texture coordinates
		float ty;    // y offset of glyph in texture coordinates
	} c[128];        // character information

	atlas(FT_Face face, int height)
	{
		FT_Set_Pixel_Sizes(face, 0, height);
		FT_GlyphSlot g = face->glyph;

		unsigned int roww = 0;
		unsigned int rowh = 0;
		w = 0;
		h = 0;

		memset(c, 0, sizeof(c));

		/* Find minimum size for a texture holding all visible ASCII characters */
		for (int i = 32; i < 128; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}
			if (roww + g->bitmap.width + 1 >= MAXWIDTH)
			{
				w = std::max(w, roww);
				h += rowh;
				roww = 0;
				rowh = 0;
			}
			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		w = std::max(w, roww);
		h += rowh;

		/* Create a texture that will be used to hold all ASCII glyphs */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Paste all glyph bitmaps into the texture, remembering the offset */
		int ox = 0;
		int oy = 0;

		rowh = 0;

		for (int i = 32; i < 128; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH)
			{
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			c[i].ax = g->advance.x >> 6;
			c[i].ay = g->advance.y >> 6;

			c[i].bw = g->bitmap.width;
			c[i].bh = g->bitmap.rows;

			c[i].bl = g->bitmap_left;
			c[i].bt = g->bitmap_top;

			c[i].tx = ox / (float) w;
			c[i].ty = oy / (float) h;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

		if (verbose)
			printf("Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
	}

	~atlas()
	{
		glDeleteTextures(1, &tex);
	}
};

TextWriter::TextWriter(const char *vertex_path, const char *fragment_path, const char *ttf_path, int font_size)
{
	if (FT_Init_FreeType(&m_ft))
		throw "Could not initialize Freetype";

	if (FT_New_Face(m_ft, ttf_path, 0, &m_face))
		throw "Could not open Freetype font";

	m_shader = new Shader(vertex_path, fragment_path);
	m_shader->enable();
	m_shader->setUniform1i(uniform_sampler, 0);
	m_shader->disable();

	m_atlas = new atlas(m_face, font_size);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
}

TextWriter::~TextWriter()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_atlas;
	delete m_shader;
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft);
}

void TextWriter::begin()
{
	m_shader->enable();
	m_shader->setUniform1i("sampler", 0);

	glBindVertexArray(m_vao);

	/* Use the texture containing the atlas */
	glBindTexture(GL_TEXTURE_2D, m_atlas->tex);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	GLint loc = m_shader->getAttribLocation(uniform_vcoord);
	glVertexAttribPointer((GLuint)loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray((GLuint)loc);
}

void TextWriter::end()
{
	GLint loc = m_shader->getAttribLocation(uniform_vcoord);
	glDisableVertexAttribArray((GLuint)loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_shader->disable();
}

void TextWriter::write(const char *text, float x, float y, float sx, float sy, const glm::vec4 &color)
{
	const uint8_t *p;

	m_shader->setUniform4f(uniform_color, color);

	point coords[6 * strlen(text)];
	int c = 0;

	/* Loop through all characters */
	for (p = (const uint8_t *) text; *p; p++)
	{
		/* Calculate the vertex and texture coordinates */
		float x2 = x + m_atlas->c[*p].bl * sx;
		float y2 = -y - m_atlas->c[*p].bt * sy;
		float w = m_atlas->c[*p].bw * sx;
		float h = m_atlas->c[*p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += m_atlas->c[*p].ax * sx;
		y += m_atlas->c[*p].ay * sy;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		coords[c++] = (point) { x2    , -y2    , m_atlas->c[*p].tx                                 , m_atlas->c[*p].ty };
		coords[c++] = (point) { x2 + w, -y2    , m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->w, m_atlas->c[*p].ty };
		coords[c++] = (point) { x2    , -y2 - h, m_atlas->c[*p].tx                                 , m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->h };
		coords[c++] = (point) { x2 + w, -y2    , m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->w, m_atlas->c[*p].ty };
		coords[c++] = (point) { x2    , -y2 - h, m_atlas->c[*p].tx                                 , m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->h };
		coords[c++] = (point) { x2 + w, -y2 - h, m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->w, m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->h };
	}

	/* Draw all the character on the screen in one go */
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, c);
}
