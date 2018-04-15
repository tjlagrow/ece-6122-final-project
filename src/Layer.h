#ifndef ECE_6122_FINAL_PROJECT_LAYER_H
#define ECE_6122_FINAL_PROJECT_LAYER_H


#include "meshes/RigidObject.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Layer
{
public:
	Layer(Shader *shader);
	~Layer();

	void add(RigidObject *object);
//	void add(std::vector<RigidObject> &objects);
	void render();

protected:
	Layer(Renderer *renderer, Shader *shader);

	Renderer *m_renderer;
	std::vector<RigidObject *> m_objects;
	Shader *m_shader;
};


#endif //ECE_6122_FINAL_PROJECT_LAYER_H
