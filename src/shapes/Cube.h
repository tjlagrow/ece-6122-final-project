#ifndef ECE_6122_FINAL_PROJECT_CUBE_H
#define ECE_6122_FINAL_PROJECT_CUBE_H

#include "../Shape.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Cube : public Shape
{
public:
	Cube(glm::mat4 model_transform);
	Cube(glm::vec4 color, glm::mat4 model_transform);
	~Cube();

	void setColor(const glm::vec4 &color);
private:


};


#endif //ECE_6122_FINAL_PROJECT_CUBE_H
