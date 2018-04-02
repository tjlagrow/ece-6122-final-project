//
// Created by rory on 4/1/18.
//

#include "Font.h"

Font::Font(const char *vertex_path, const char *fragment_path, const char *ttf_path)
{
	GLuint loc;

	m_shader = new Shader(vertex_path, fragment_path);

	if (FT_Init_FreeType(&m_ft))
		throw "Could not initialize Freetype";

	if (FT_New_Face(m_ft, ttf_path, 0, &m_face))
		throw "Could not open Freetype font";
	FT_Set_Pixel_Sizes(m_face, 0, 48);

	if (FT_Load_Char(m_face, 'X', FT_LOAD_RENDER))
		throw "Could not load character";

	m_shader->enable();
	m_shader->setUniform1i("sampler", 0);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenTextures(1, &m_tbo);

	glBindVertexArray(m_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tbo);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	loc = m_shader->getAttribLocation("vcoord");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_shader->disable();
}

Font::~Font()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft);
}

void Font::begin() const
{
	m_shader->enable();
	glBindVertexArray(m_vao);
}

void Font::end() const
{
	glBindVertexArray(0);
	m_shader->disable();
}

void Font::write(const char *text, float x, float y, int width, int height)
{
	static GLfloat yellow[4] = {1, 1, 0, 1};
	m_shader->setUniform4fv("ucolor", 1, yellow);
	float sx = 2.0f / width;
	float sy = 2.0f / height;

	for (const char *p = text; *p; p++)
	{
		if (FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
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

		GLfloat box[4][4] =
		{
			{ x2,   -y2,   0, 0 },
			{ x2+w, -y2,   1, 0 },
			{ x2,   -y2-h, 0, 1 },
			{ x2+w, -y2-h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (m_face->glyph->advance.x / 64) * sx;
		y += (m_face->glyph->advance.y / 64) * sy;
	}
}
