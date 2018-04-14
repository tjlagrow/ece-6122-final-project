#ifndef ECE_6122_FINAL_PROJECT_LAYER_H
#define ECE_6122_FINAL_PROJECT_LAYER_H


#include "Shape.h"
#include "BatchRenderer.h"
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
	Layer(BatchRenderer *renderer, Shader *shader);

	BatchRenderer *m_renderer;
	std::vector<Shape *> m_shapes;
	Shader *m_shader;
};


#endif //ECE_6122_FINAL_PROJECT_LAYER_H
