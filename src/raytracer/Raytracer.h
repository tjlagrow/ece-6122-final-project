#ifndef ECE_6122_FINAL_PROJECT_RAYTRACER_H
#define ECE_6122_FINAL_PROJECT_RAYTRACER_H


#include "Sphere.h"
#include "Vector3.h"
#include <vector>

class Raytracer
{
public:
	Raytracer(unsigned int width, unsigned int height, float fov);
	~Raytracer();

	void render(const std::vector<Sphere> &spheres);

private:
	unsigned int m_width;
	unsigned int m_height;
	float m_fov;
	float m_aspectRatio;
	float m_invertedWidth;
	float m_invertedHeight;
	float m_angle;

};


#endif //ECE_6122_FINAL_PROJECT_RAYTRACER_H
