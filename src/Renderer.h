#ifndef ECE_6122_FINAL_PROJECT_RENDERER_H
#define ECE_6122_FINAL_PROJECT_RENDERER_H

#include "Vertex.h"
#include "Shader.h"
#include "objects/Material.h"
#include "objects/Object.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "objects/Mesh.h"

#define MESH_MAX_INDICES         1000000
#define MESH_MAX_BUFFER_BYTES    1000000

#define SHADER_INDEX_POSITION    0
#define SHADER_INDEX_UV          1
#define SHADER_INDEX_COLOR       2
#define SHADER_INDEX_NORMAL      3

class Object;

class Renderer
{
public:
	Renderer(Shader *shader);
	~Renderer();

	void calculateMeshStuff(const Object *object, std::vector<Vertex> &gpuVertices,std::vector<GLuint> &gpuIndices, std::vector<Mesh> mesh,
							std::vector<Material> materials);
	void submit(const Object *object);
	void flush(const glm::vec3 &eyePosition);

private:

	struct MeshInfo
	{
		unsigned int numIndices;
		unsigned int materialIndex;
	};

	Shader *m_shader;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	unsigned int m_numGpuVertices;
	unsigned int m_numGpuIndices;
	std::vector<MeshInfo> m_meshInfo;
	std::vector<Material> m_materials;
	std::vector<glm::mat4> m_transformations;

};


#endif //ECE_6122_FINAL_PROJECT_RENDERER_H
