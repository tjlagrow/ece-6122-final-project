#ifndef ECE_6122_FINAL_PROJECT_TEXTWRITER_H
#define ECE_6122_FINAL_PROJECT_TEXTWRITER_H

#include "Shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#define VS_FONT_PATH  "../shaders/vs_font.glsl"
#define FS_FONT_PATH  "../shaders/fs_font.glsl"
#define TTF_PATH      "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf"

struct atlas;

class TextWriter
{
public:
	TextWriter(const char *vertex_path, const char *fragment_path, const char *ttf_path, int font_size);
	~TextWriter();

	void begin();
	void end();
	void write(const char *text, float x, float y, float sx, float sy, const glm::vec4 &color);

private:
	FT_Library m_ft;
	FT_Face m_face;

	GLuint m_vao;
	GLuint m_vbo;

	Shader *m_shader;

	struct atlas *m_atlas;
};


#endif //ECE_6122_FINAL_PROJECT_TEXTWRITER_H
