#ifndef ECE_6122_FINAL_PROJECT_FACE_H
#define ECE_6122_FINAL_PROJECT_FACE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Face : public glm::uvec3
{
	Face(unsigned int x, unsigned int y, unsigned int z) :
		glm::uvec3(x, y, z) { }
};

#endif //ECE_6122_FINAL_PROJECT_FACE_H
