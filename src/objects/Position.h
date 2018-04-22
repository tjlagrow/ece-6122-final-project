#ifndef ECE_6122_FINAL_PROJECT_POSITION_H
#define ECE_6122_FINAL_PROJECT_POSITION_H


#include <GL/glew.h>
#include <glm/glm.hpp>

struct Position : public glm::vec3
{
	Position() : glm::vec3() { }
	Position(float x, float y, float z) : glm::vec3(x, y, z) { }

	glm::vec3 getGlmVec3()
	{
		return glm::vec3(this->x, this->y, this->z);
	}

	glm::vec4 getGlmVec4()
	{
		return glm::vec4(this->x, this->y, this->z, 1.0f);
	}
};


#endif //ECE_6122_FINAL_PROJECT_POSITION_H
