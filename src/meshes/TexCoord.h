#ifndef ECE_6122_FINAL_PROJECT_TEXCOORD_H
#define ECE_6122_FINAL_PROJECT_TEXCOORD_H


#include <GL/glew.h>
#include <glm/glm.hpp>

struct TexCoord : public glm::vec3
{
	TexCoord(float u, float v, float t) : glm::vec3(u, v, t) { }
};


#endif //ECE_6122_FINAL_PROJECT_TEXCOORD_H
