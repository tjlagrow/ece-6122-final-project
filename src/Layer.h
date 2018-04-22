#ifndef ECE_6122_FINAL_PROJECT_LAYER_H
#define ECE_6122_FINAL_PROJECT_LAYER_H


#include "meshes/Object.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Layer
{
public:
	Layer(Shader *shader);
	~Layer();

	void add(Object *object);
	void render();

	const std::vector<Object *> &getObjects();
	Object *getObject(int index);

protected:
	Layer(Renderer *renderer, Shader *shader);

	Renderer *m_renderer;
	std::vector<Object *> m_objects;
	Shader *m_shader;
};


#endif //ECE_6122_FINAL_PROJECT_LAYER_H
