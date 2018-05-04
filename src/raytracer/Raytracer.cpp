#include "Raytracer.h"
#include "Sphere.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

/* Variable tp help control maximum recursion depth.  Important for speed.
   Without a k-d tree or some type of time improvement, I usually don't go
   over 35. */
#define MAX_RAY_DEPTH  30

static unsigned int imageIndex = 0;

static Sphere light(
	Vector3f(0, 0, 100), // Origin position of light source
	100, // Radius
	Vector3f(0.00, 0.00, 0.00), // Surface color
	0, // Reflection
	0.0, // Transparency
	Vector3f(1.5) // Emission color
);

static Sphere ground(
	Vector3f(0, 0, -60000),
	60000,
	Vector3f(222.0f/255.0f, 184.0f/255.0f, 135.0f/255.0f),
	0,
	0,
	0
);

/**
 * Helper function for color mixing based on weight
 * @param a TODO Document
 * @param b TODO Document
 * @param weight TODO Document
 * @return TODO Document
 */
float weightDistribution(const float &a, const float &b, const float &weight)
{
	return a * (1 - weight) + b * weight;
}

/**
 * The main trace function! It takes a standard ray as argument which is
 * defined by its origin and direction. There is a test if this ray
 * intersects any of the geometry in the scene (the geometry will be created
 * later). If the ray intersects an object, the intersection point is
 * computed, the normal at the intersection point is computed, and thus shade
 * this point using this newly calculated information. Shading depends on
 * the surface property of the primitive which is transparency,
 * reflectiveness, diffuseablity. The function returns a color for the ray.
 * If the ray intersects an object that is the color of the object at the
 * intersection point, otherwise it returns the background color.
 * @param rayOrigin TODO Document
 * @param rayDirection TODO Document
 * @param spheres TODO Document
 * @param depth TODO Document
 * @return TODO Document
 */
Vector3f trace(
	const Vector3f &rayOrigin,
	const Vector3f &rayDirection,
	const std::vector<Sphere> &spheres,
	const int &depth)
{
	float tnear = INFINITY;
	const Sphere* sphere = NULL;

	// Test if this ray intersects any object in the scene
	for (unsigned i = 0; i < spheres.size(); ++i)
	{
		float t0 = INFINITY;
		float t1 = INFINITY;

		if (spheres[i].intersect(rayOrigin, rayDirection, t0, t1))
		{
			if (t0 < 0) t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				sphere = &spheres[i];
			}
		}
	}

	// Return black or background color if there is no intersection
	if (! sphere)
	{
		return Vector3f(0.7f);
	}

	Vector3f surfaceColor = 0;  // Intialize color of the object intersected by the ray
	Vector3f pointOfIntersection = rayOrigin + rayDirection * tnear;
	Vector3f normalOfIntersection = pointOfIntersection - sphere->position;
	normalOfIntersection.normalize(); // Normalize the normal

	/* Start the checks and headaches */

	/* If the normal and the view direction are not opposite to each other,
	 * reverse the normal direction. That means we are inside the sphere...
	 * so set the inside bool to true. Finally reverse the sign of IdotN
	 * which we want positive.
	 */

	// Add some bias to the point from which we will be tracing to retain some color   */
	float bias = 1e-4;
	bool inside = false; /* initialize */
	if (rayDirection.dot(normalOfIntersection) > 0)
	{
		normalOfIntersection = -normalOfIntersection;
		inside = true;
	}

	if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH)
	{
		float facingRatio = -rayDirection.dot(normalOfIntersection);

		/* change the color mixing value to maximize the effect */
		float fresnelEffect = weightDistribution(pow(1 - facingRatio, 3), 1, 0.5);

		/* since all of the vectors are already normalized, the reflection direction is computed  */
		Vector3f reflectionDirection = rayDirection - normalOfIntersection * 2 * rayDirection.dot(normalOfIntersection);
		reflectionDirection.normalize(); /* normalize */
		Vector3f reflection = trace(pointOfIntersection + normalOfIntersection * bias, reflectionDirection, spheres, depth + 1);
		Vector3f refraction = 0;

		/* if the sphere is also transparent compute refraction ray (transmission) */
		if (sphere->transparency)
		{
			float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
			float co = -normalOfIntersection.dot(rayDirection);
			float k = 1 - eta * eta * (1 - co * co);
			Vector3f refractionDirection = rayDirection * eta + normalOfIntersection * (eta *  co - sqrt(k));
			refractionDirection.normalize(); /* normalize */
			refraction = trace(pointOfIntersection - normalOfIntersection * bias, refractionDirection, spheres, depth + 1);
		}
		/* the result is a weighted color distrobution value of reflection and refraction (if the sphere is transparent) */
		surfaceColor = (reflection * fresnelEffect + refraction * (1 - fresnelEffect) * sphere->transparency) * sphere->surfaceColor;
	}
	else
	{
		/* a diffuse object, we can stop marching */
		for (unsigned i = 0; i < spheres.size(); ++i)
		{
			if (spheres[i].emissionColor.x > 0)
			{
				// this is a light
				Vector3f transmission = 1;
				Vector3f lightDirection = spheres[i].position - pointOfIntersection;
				lightDirection.normalize(); /* normalize */
				for (unsigned j = 0; j < spheres.size(); ++j)
				{
					if (i != j)
					{
						float t0, t1;
						if (spheres[j].intersect(pointOfIntersection + normalOfIntersection * bias, lightDirection, t0, t1))
						{
							transmission = 0;
							break;
						}
					}
				}
				surfaceColor += sphere->surfaceColor * transmission *
					std::max(float(0), normalOfIntersection.dot(lightDirection)) * spheres[i].emissionColor;
			}
		}
	}

	return surfaceColor + sphere->emissionColor;
}

/**
 * Constructor
 * @param width TODO Document
 * @param height TODO Document
 * @param fov TODO Document
 */
Raytracer::Raytracer(unsigned int width, unsigned int height, float fov)
{
	m_width = width;
	m_height = height;
	m_fov = fov;
	m_aspectRatio = (float)width / (float)height;
	m_invertedWidth = 1 / float(width);
	m_invertedHeight = 1 / float(height);
	m_image = (Vector3f *)calloc(width * height, sizeof(Vector3f));
	m_angle = tan(M_PI * 0.5 * m_fov / 180.);
}

/**
 * Destructor
 */
Raytracer::~Raytracer()
{
	free(m_image);
}

/**
 * This is the rendering function. A camera ray for each pixel of the image
 * is computed and then traced and finally returned with a color. If the ray
 * hits a sphere, the color of the sphere at the intersection point is
 * returned, else we return the background color.
 * @param camPos TODO Document
 * @param lookAt TODO Document
 * @param objects A vector of objects in the scene
 */
void Raytracer::render(
	glm::vec3 camPos,
	glm::vec3 lookAt,
	const std::vector<Object *> &objects)
{
	Vector3f *pixel = &m_image[0];

	std::vector<Sphere> spheres;
	spheres.push_back(ground);

	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		glm::vec3 origin = objects[i]->getWorldOrigin();
		Vector3f originVec3f(origin.x, origin.z, origin.y);

		float radius = objects[i]->getSize().x / 2.0f;
		glm::vec3 color = objects[i]->getMaterials()[1].getDiffuse();
		Vector3f surfaceColor(color.x, color.y, color.z);
		float reflection = 1.0e-3;
		float transparency = 0;
		float emissionColor = 0;

		spheres.push_back(Sphere(
			originVec3f,
			radius,
			surfaceColor,
			reflection,
			transparency,
			emissionColor
		));
	}

	// Push back light ball
	light.position.z += camPos.y;
	spheres.push_back(light);

	// Loop over the row pixels in the image
	for (unsigned y = 0; y < m_height; ++y)
	{
		// Loop over the column pixels in the image
		for (unsigned x = 0; x < m_width; ++x, ++pixel)
		{
			float xx = (2 * ((x + 0.5) * m_invertedWidth) - 1) * m_angle * m_aspectRatio;
			float yy = (1 - 2 * ((y + 0.5) * m_invertedHeight)) * m_angle;

			Vector3f rayDirection(xx, yy, -1);
			rayDirection.normalize();

			*pixel = trace(Vector3f(0, 0, camPos.y), rayDirection, spheres, 0);
		}
	}

	/* Save result to a pmm image. This method of saving works for mac,
	 * but you need to save to .ppm for window/linux
	 */
	char imageFilename[32];
	snprintf(imageFilename, sizeof(imageFilename), "./image%04u.ppm", imageIndex);
	std::ofstream ppm(imageFilename, std::ios::out | std::ios::binary);
	ppm << "P6\n" << m_width << " " << m_height << "\n255\n";
	for (unsigned i = 0; i < m_width * m_height; ++i)
	{
		ppm << (unsigned char)(std::min(float(1), m_image[i].x) * 255) <<
			(unsigned char)(std::min(float(1), m_image[i].y) * 255) <<
			(unsigned char)(std::min(float(1), m_image[i].z) * 255);
	}
	ppm.close();
	imageIndex++;
}


// FOR REFERENCE ONLY ////////////////////////////////////////////////////////

/**
 * Its finally the main function! Whoot whoot!. This is where the geometries
 * (primatives - in our case, spheres) will be created! Since I wanted to show
 * the shadows, I added a *huge* sphere to subsitute as a floor and a sphere
 * as the light source.
 */
//int main(int argc, char **argv)
//{
//	/* Step on up! You've come to see the show! */
//	std::vector<Sphere> spheres;
//
//	/* Sphere order: position(x,y,z), radius, surface color(r,g,b), reflectivity, transparency, emission color */
//
//	/* *huge* base sphere, I went with neon green to show the light coming back up on the other spheres */
//	spheres.push_back(Sphere(Vector3f( 0.0, -60004, -20), 60000, Vector3f(0.22, 1.00, 0.07), 0, 0.0));
//
//	/* the main actors! */
//	spheres.push_back(Sphere(Vector3f( 0.0,  0,   -20),    4.0, Vector3f(1.00, 0.32, 0.36), 1, 0.5));  /* Red    */
//	spheres.push_back(Sphere(Vector3f( 5.0, -1,   -15),    2.0, Vector3f(0.99, 0.81, 0.14), 1, 0.0));  /* Yellow */
//	spheres.push_back(Sphere(Vector3f( 5.7, -0.8, -12),    0.7, Vector3f(0.00, 0.75, 1.00), 0, 0.0));  /* Blue   */
//	spheres.push_back(Sphere(Vector3f( 12.0, 2.3, -25),    5.2, Vector3f(0.46, 0.05, 0.67), 0, 0.0));  /* Purple */
//	spheres.push_back(Sphere(Vector3f(-5.5,  0,   -15),    2.8, Vector3f(0.90, 0.90, 0.90), 1, 0.0));  /* Black  */
//
//	/* this is the light ball! */
//	spheres.push_back(Sphere(Vector3f( 10,  20,  10),      4, Vector3f(0.00, 0.00, 0.00), 0, 0.0, Vector3f(3)));
//
//	/* GOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!  And they are off to the races! */
//	render(spheres);
//
//	/* annnndd... finish :') */
//	return 0;
//}

