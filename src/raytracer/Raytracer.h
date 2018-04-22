#ifndef ECE_6122_FINAL_PROJECT_RAYTRACER_H
#define ECE_6122_FINAL_PROJECT_RAYTRACER_H


#include "Sphere.h"
#include "Vector3.h"
#include <vector>

class Raytracer
{
public:
	static void render(const std::vector<Sphere> &spheres);

private:

};


#endif //ECE_6122_FINAL_PROJECT_RAYTRACER_H
