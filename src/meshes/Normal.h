#ifndef ECE_6122_FINAL_PROJECT_NORMAL_H
#define ECE_6122_FINAL_PROJECT_NORMAL_H


#include <GL/glew.h>
#include <glm/glm.hpp>

struct Normal : public glm::vec3
{
	Normal() : glm::vec3() { }
	Normal(float x, float y, float z) : glm::vec3(x, y, z) { }
};


#endif //ECE_6122_FINAL_PROJECT_NORMAL_H
