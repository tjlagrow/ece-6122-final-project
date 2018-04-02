#ifndef ECE_6122_FINAL_PROJECT_FONT_H
#define ECE_6122_FINAL_PROJECT_FONT_H

#include "Shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Window.h"

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
	void write(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_tbo;

	Shader *m_shader;

};


#endif //ECE_6122_FINAL_PROJECT_FONT_H
