#ifndef ECE_6122_FINAL_PROJECT_TEXCOORD_H
#define ECE_6122_FINAL_PROJECT_TEXCOORD_H


#include <GL/glew.h>
#include <glm/glm.hpp>

struct TexCoord : public glm::vec2
{
	TexCoord() : glm::vec2() { }
	TexCoord(float x, float y) : glm::vec2(x, y) { }
};


#endif //ECE_6122_FINAL_PROJECT_TEXCOORD_H
