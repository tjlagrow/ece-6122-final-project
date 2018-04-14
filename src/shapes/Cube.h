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
	Cube();
	Cube(glm::mat4 model_transform);
	~Cube();

	const std::vector<glm::vec4> &getPositions() const;
	const std::vector<glm::vec4> &getColors() const;

private:
	std::vector<glm::vec4> m_positions;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec4> m_colors;
	glm::mat4 m_model_transform;
};


#endif //ECE_6122_FINAL_PROJECT_CUBE_H
