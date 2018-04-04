#ifndef ECE_6122_FINAL_PROJECT_FONT_H
#define ECE_6122_FINAL_PROJECT_FONT_H

#include "Shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#define VS_FONT_PATH  "../shaders/vs_font.glsl"
#define FS_FONT_PATH  "../shaders/fs_font.glsl"
#define TTF_PATH      "/usr/share/fonts/truetype/freefont/FreeSans.ttf"

class Font
{
public:
	Font(const char *vertex_path, const char *fragment_path, const char *ttf_path);
	~Font();

	void begin() const;
	void end() const;
	void write(const char *text, float x, float y, float sx, float sy);

private:
	FT_Library m_ft;
	FT_Face m_face;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_tex;

	Shader *m_shader;

};


#endif //ECE_6122_FINAL_PROJECT_FONT_H
