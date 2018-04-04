#ifndef ECE_6122_FINAL_PROJECT_TERRAIN_H
#define ECE_6122_FINAL_PROJECT_TERRAIN_H

#include <GL/glew.h>

class Terrain
{
public:
	Terrain();
	~Terrain();
private:
	GLuint m_positions;
	GLuint m_normals;
	GLuint m_indices;
};


#endif //ECE_6122_FINAL_PROJECT_TERRAIN_H
