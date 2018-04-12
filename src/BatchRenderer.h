#ifndef ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
#define ECE_6122_FINAL_PROJECT_BATCHRENDERER_H

#include "Shape.h"
#include "Renderer.h"

#define RENDERER_MAX_SHAPES      1000
#define RENDERER_MAX_INDICES     10000
#define RENDERER_VERTEX_BYTES    (sizeof(Vertex))
#define RENDERER_BUFFER_BYTES    (10 * RENDERER_VERTEX_BYTES * RENDERER_MAX_SHAPES)

#define SHADER_INDEX_POSITION    0
#define SHADER_INDEX_UV          1
#define SHADER_INDEX_COLOR       2

class BatchRenderer : public Renderer
{
public:
	BatchRenderer();
	~BatchRenderer();

	void begin() override;
	void end() override;
	void submit(const Shape *shape) override;
	void flush() override;

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLsizei m_index_count;

};


#endif //ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
