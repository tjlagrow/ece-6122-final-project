#ifndef ECE_6122_FINAL_PROJECT_GLMUTILS_H
#define ECE_6122_FINAL_PROJECT_GLMUTILS_H

#include <glm/glm.hpp>

glm::vec3 &operator/(glm::vec3 &lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return (lhs);
}


#endif //ECE_6122_FINAL_PROJECT_GLMUTILS_H
