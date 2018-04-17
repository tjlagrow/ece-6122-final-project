#ifndef ECE_6122_FINAL_PROJECT_MESHLOADER_H
#define ECE_6122_FINAL_PROJECT_MESHLOADER_H


#include "RigidObject.h"
#include <vector>


class ObjectLoader
{
public:
	static void loadFromFile(const char *filepath, RigidObject *object);

private:
	ObjectLoader();
};


#endif //ECE_6122_FINAL_PROJECT_MESHLOADER_H
