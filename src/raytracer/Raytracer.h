#ifndef ECE_6122_FINAL_PROJECT_RAYTRACER_H
#define ECE_6122_FINAL_PROJECT_RAYTRACER_H


#include "Sphere.h"
#include "Vector3.h"
#include <vector>

class Raytracer
{
public:
	static void render(
		const std::vector<Sphere> &spheres,
		unsigned int width = 3240, // 3840x2160 for 16:9
		unsigned int height = 1080, // 3840x2160 for 16:9
		float fov = 30
	);

private:

};


#endif //ECE_6122_FINAL_PROJECT_RAYTRACER_H
