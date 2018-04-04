#include "Font.h"
#include <map>

Font::Font(const char *vertex_path, const char *fragment_path, const char *ttf_path)
{
	GLint loc;

	if (FT_Init_FreeType(&m_ft))
		throw "Could not initialize Freetype";

	if (FT_New_Face(m_ft, ttf_path, 0, &m_face))
		throw "Could not open Freetype font";

	// Define font size to extract from face
	FT_Set_Pixel_Sizes(m_face, 0, 48);

	m_shader = new Shader(vertex_path, fragment_path);
	m_shader->enable();

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	m_shader->setUniform1i("sampler", 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &m_vbo);
	loc = m_shader->getAttribLocation("vcoord");
	glEnableVertexAttribArray(loc);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

	m_shader->disable();
}

Font::~Font()
{
	delete m_shader;
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft);

}

void Font::begin() const
{
	m_shader->enable();
}

void Font::end() const
{
	m_shader->disable();
}

void Font::write(const char *text, float x, float y, float sx, float sy)
{
	const char *p;

	for(p = text; *p; p++)
	{
		if(FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);

		float x2 = x + m_face->glyph->bitmap_left * sx;
		float y2 = -y - m_face->glyph->bitmap_top * sy;
		float w = m_face->glyph->bitmap.width * sx;
		float h = m_face->glyph->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (m_face->glyph->advance.x/64) * sx;
		y += (m_face->glyph->advance.y/64) * sy;
	}
}
