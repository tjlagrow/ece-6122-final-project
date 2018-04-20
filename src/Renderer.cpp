#include <map>
#include <algorithm>
#include <numeric>
#include <cstring>
#include "Renderer.h"
#include "meshes/Color.h"

void deleteme(const char *func)
{
	GLenum errnum = glGetError();
	if (errnum != GL_NO_ERROR)
	{
		printf("ERROR: %s: OpenGL %d: ", func, errnum);
		switch (errnum)
		{
			case GL_INVALID_ENUM:
				printf("Invalid enum\n");
				break;
			case GL_INVALID_VALUE:
				printf("Invalid value\n");
				break;
			case GL_INVALID_OPERATION:
				printf("Invalid operation\n");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("Invalid framebuffer operation\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("Out of memory\n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("Stack underflow\n");
				break;
			case GL_STACK_OVERFLOW:
				printf("Stack overflow\n");
				break;
			default:
				printf("\n");
		}
	}
}

/**
 * Constructor
 */
Renderer::Renderer()
{
	m_vao = 0;
	m_vbo = 0;
	m_ibo = 0;
	m_numGpuVertices = 0;
	m_numGpuIndices = 0;
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, MESH_MAX_BUFFER_BYTES, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_INDEX_POSITION);
	glEnableVertexAttribArray(SHADER_INDEX_UV);
	glEnableVertexAttribArray(SHADER_INDEX_COLOR);
	glEnableVertexAttribArray(SHADER_INDEX_NORMAL);

	glVertexAttribPointer(
		SHADER_INDEX_POSITION,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::position))
	);
	glVertexAttribPointer(
		SHADER_INDEX_UV,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::uv))
	);
	glVertexAttribPointer(
		SHADER_INDEX_COLOR,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::color)));
	glVertexAttribPointer(
		SHADER_INDEX_NORMAL,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH_MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

/**
 * Destructor
 */
Renderer::~Renderer()
{
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

/**
 * TODO
 * @param object TODO
 */
void Renderer::submit(const RigidObject *object)
{
	std::vector<Vertex> gpuVertices;
	std::vector<GLuint> gpuIndices;
	std::vector<Mesh> meshes = object->getMeshes();

	// Loop over the meshes in this object and extract the relevant vertices
	// and matching index positions into their respective arrays. These arrays
	// will be sent to the GPU next.
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		unsigned int thisMeshesIndicesCount = 0;
		glm::mat4 mt = meshes[i].getModelTransform();
		std::vector<Face> faces = meshes[i].getFaces();

		// Each mesh has a face
		for (unsigned int j = 0; j < faces.size(); ++j)
		{
			// Each face has 3 vertices
			for (unsigned int k = 0; k < 3; k++)
			{
				Position pos;
				TexCoord tex;
				Color col;
				Normal nrm;

				const Position *ptrPosition = object->getPosition(faces[j].positionIndices[k]);
				if (!ptrPosition)
					continue;
				pos = *ptrPosition;

				const TexCoord *ptrTexCoord = object->getTexCoord(faces[j].texcoordIndices[k]);
				if (! ptrTexCoord)
					tex = TexCoord();
				else
					tex = *ptrTexCoord;

				col = Color(1.0f, 1.0f, 1.0f, 1.0f);

				const Normal *ptrNormal = object->getNormal(faces[j].normalIndices[k]);
				if (! ptrNormal)
					nrm = Normal();
				else
					nrm = *ptrNormal;

				Vertex v(mt * pos.getGlmVec4(), tex, col, nrm);
				unsigned int index = 0;

				// If the vertices array is empty, then it's the first time
				// we've been through here and thus we just need to add the
				// Vertex we created above
				if (! gpuVertices.empty())
				{
					// Look and see if this vertex already exists in our array
					// If so, no need to add it and duplicate it. That will waste
					// space and time on the GPU. Instead, we'll just make note
					// and store the index of the existing Vertex
					// NOTE: If this thing below looks weird it's because it
					// is kind of weird. It's called a "lambda" function.
					auto it = std::find_if(gpuVertices.begin(), gpuVertices.end(),
						[&v](const Vertex &obj) // lambda function begin
						{
							return (v == obj);
						}
					);

					// Check if we found the object
					if (it != gpuVertices.end())
					{
						// We found it. Grab the index
						index = std::distance(gpuVertices.begin(), it);
					}
					else
					{
						// Did not find it. New index is the last element + 1
						// NOTE: Must do in this order!
						index = gpuVertices.size();
						gpuVertices.push_back(v);
					}
				}
				else
				{
					// Array was empty, so just add it
					gpuVertices.push_back(v);
				}
				gpuIndices.push_back(index + m_numGpuVertices);
				thisMeshesIndicesCount++;
			}
		}

		m_meshIndicesCount.push_back(thisMeshesIndicesCount);
	}

	unsigned long vertBytes = gpuVertices.size() * sizeof(Vertex);
	GLintptr vertOffset = m_numGpuVertices * sizeof(Vertex);

	unsigned long indicesBytes = gpuIndices.size() * sizeof(GLuint);
	GLintptr indicesOffset = m_numGpuIndices * sizeof(GLuint);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, vertOffset, vertBytes, gpuVertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesBytes, gpuIndices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Keep a running tally of the total number of
	// indices and vertices written to the GPU
	m_numGpuIndices += gpuIndices.size();
	m_numGpuVertices += gpuVertices.size();
}

/**
 * Draws the triangles in this batch to the back buffer
 */
void Renderer::flush()
{
	unsigned int count = 0;
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	for (unsigned int i = 0; i < m_meshIndicesCount.size(); ++i)
	{
		glDrawElements(GL_TRIANGLES, m_meshIndicesCount[i], GL_UNSIGNED_INT, (void *)(count * sizeof(GLuint)));
		count += m_meshIndicesCount[i];
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_numGpuVertices = 0;
	m_numGpuIndices = 0;
	m_meshIndicesCount.clear();
}

/**
 * Pushes a transformation matrix. For more information, see:
 * <https://www.youtube.com/watch?v=16GtFaXwMSo&index=17&list=PLlrATfBNZ98fqE45g3jZA_hLGUrD4bo6_>
 * @param matrix TODO
 * @param override TODO
 */
void Renderer::push(glm::mat4 matrix, bool override)
{
	if (override)
		m_transformations.push_back(matrix);
	else
		// For more information, see:
		m_transformations.push_back(m_transformations.back() * matrix);

	m_back_transform = &m_transformations.back();
}

/**
 * Pops a transformation matrix
 */
void Renderer::pop()
{
	if (m_transformations.size() > 1)
		m_transformations.pop_back();

	m_back_transform = &m_transformations.back();
}

