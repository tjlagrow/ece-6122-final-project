#ifndef ECE_6122_FINAL_PROJECT_LAYER_H
#define ECE_6122_FINAL_PROJECT_LAYER_H


#include "meshes/Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Layer
{
public:
	Layer(Shader *shader);
	~Layer();

	void add(Mesh *mesh);
	void add(std::vector<Mesh> &meshes);
	void render();

protected:
	Layer(Renderer *renderer, Shader *shader);

	Renderer *m_renderer;
	std::vector<Mesh *> m_shapes;
	Shader *m_shader;
};


#endif //ECE_6122_FINAL_PROJECT_LAYER_H
