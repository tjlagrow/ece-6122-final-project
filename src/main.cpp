#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#define VS_CUBE_PATH  "../shaders/vertex.glsl"
#define FS_CUBE_PATH  "../shaders/fragment.glsl"
#define VS_FONT_PATH  "../shaders/vs_font.glsl"
#define FS_FONT_PATH  "../shaders/fs_font.glsl"
#define FONT_PATH     "/usr/share/fonts/truetype/freefont/FreeSans.ttf"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480
#define WINDOW_TITLE    "ECE6122 Final Project"

void render_text(const char *text,
				 float x, float y, float sx, float sy,
				 FT_Face face, FT_GlyphSlot g)
{
	const char *p;
	for (p = text; *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] =
		{
			{ x2,   -y2,   0, 0 },
			{ x2+w, -y2,   1, 0 },
			{ x2,   -y2-h, 0, 1 },
			{ x2+w, -y2-h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x / 64) * sx;
		y += (g->advance.y / 64) * sy;
	}
}

/**
 * Main program entry point
 * @param argc Number of command-line arguments
 * @param argv Command line arguments array
 * @return Returns 0 on success, else error
 */
int main(int argc, char **argv)
{
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, &cfg);

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shader1(VS_CUBE_PATH, FS_CUBE_PATH);
	Shader shader2(VS_FONT_PATH, FS_FONT_PATH);

//	PhysicsEngine engine;
//	engine.simple_ball_drop();

	GLfloat vertices[] =
	{
		+0.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower left
		+1.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower right
		+1.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper right
		+0.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper left
		+0.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower left
		+1.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower right
		+1.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper right
		+0.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper left
	};

	GLuint indices[] =
	{
		0, 1, 2,  2, 3, 0, // back
		4, 5, 6,  6, 7, 4, // front
		1, 5, 6,  6, 2, 1, // right
		0, 4, 7,  7, 3, 0, // left
		2, 3, 7,  7, 6, 2, // top
		0, 1, 5,  5, 4, 0, // bottom
	};

	GLuint vao, vbo, ibo;
	GLuint loc;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	shader1.enable();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	loc = shader1.getAttribLocation("vpos");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(loc);

	loc = shader1.getAttribLocation("vcol");
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(loc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	shader1.disable();

	///////////////////////////////////////////////

	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	float fov = 45.0f;
	glm::mat4 pMatrix = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
	glm::mat4 vMatrix = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mMatrix = glm::mat4(1.0f);
	glm::mat4 mvpMatrix = pMatrix * vMatrix * mMatrix;

	///////////////////////////////////////////////
	shader2.enable();
	GLuint tao;
	glGenVertexArrays(1, &tao);
	glBindVertexArray(tao);
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		printf("Could not initialize Freetype\n");
	FT_Face face;
	if (FT_New_Face(ft, FONT_PATH, 0, &face))
		printf("Could not open Freetype font\n");
	FT_Set_Pixel_Sizes(face, 0, 48);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		printf("Could not load character 'x'\n");
	FT_GlyphSlot g = face->glyph;
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	shader2.setUniform1i("sampler", 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	loc = shader2.getAttribLocation("vcoord");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	shader2.disable();
	///////////////////////////////////////////////

	double tn = window.get_time();
	double td = tn + 1.0f;
	size_t indices_to_draw = sizeof(indices)/sizeof(GLuint);

	while (! window.should_close())
	{
		window.clear();

		if ((tn = window.get_time()) > td)
		{
			td = tn + 1.0f;
		}

		shader1.enable();
		shader1.setUniformMat4("mvp", mvpMatrix);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices_to_draw, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader1.disable();

		shader2.enable();
		glBindVertexArray(tao);
		static GLfloat yellow[4] = {1, 1, 0, 1};
		shader2.setUniform4fv("ucolor", 1, yellow);
		float sx = 2.0f / window.get_width();
		float sy = 2.0f / window.get_height();
		render_text("hello world", -1+8*sx, 1-50*sy, sx, sy, face, g);
		glBindVertexArray(0);
		shader2.disable();


		window.update();
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	shader1.disable();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return (0);
}
