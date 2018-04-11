#ifndef ECE_6122_FINAL_PROJECT_LAYER_H
#define ECE_6122_FINAL_PROJECT_LAYER_H


#include "Shape.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Layer
{
public:
	Layer(Shader *shader);
	virtual ~Layer();

	virtual void add(Shape *shape);
	virtual void render();

protected:
	Layer(Renderer *renderer, Shader *shader, const glm::mat4 &pmat);

	Renderer *m_renderer;
	std::vector<Shape *> m_shapes;
	Shader *m_shader;
	glm::mat4 m_pmat;
};


#endif //ECE_6122_FINAL_PROJECT_LAYER_H
