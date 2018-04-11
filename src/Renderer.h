#ifndef ECE_6122_FINAL_PROJECT_RENDERER_H
#define ECE_6122_FINAL_PROJECT_RENDERER_H


#include <glm/glm.hpp>
#include <vector>

class Shape;

class Renderer
{
public:
	virtual ~Renderer();

	virtual void begin();
	virtual void end();

	virtual void submit(const Shape *shape) = 0;
	virtual void flush() = 0;

	void push(glm::mat4 matrix, bool override = false);
	void pop();

protected:
	Renderer();

	std::vector<glm::mat4> m_transformations;
	const glm::mat4 *m_back_transform;


};

#endif //ECE_6122_FINAL_PROJECT_RENDERER_H
