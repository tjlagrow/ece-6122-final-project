#ifndef ECE_6122_FINAL_PROJECT_MESHLOADER_H
#define ECE_6122_FINAL_PROJECT_MESHLOADER_H


#include "Object.h"
#include <vector>


class ObjectLoader
{
public:
	static void loadFromFile(const char *filepath, Object *object);

private:
	ObjectLoader();
};


#endif //ECE_6122_FINAL_PROJECT_MESHLOADER_H
