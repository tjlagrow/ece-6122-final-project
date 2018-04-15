#ifndef ECE_6122_FINAL_PROJECT_SHAPELOADER_H
#define ECE_6122_FINAL_PROJECT_SHAPELOADER_H


#include "Mesh.h"
#include <vector>

class MeshLoader
{
public:
	static std::vector<Mesh> loadFromFile(const char *filepath);
private:
	MeshLoader();
};


#endif //ECE_6122_FINAL_PROJECT_SHAPELOADER_H
