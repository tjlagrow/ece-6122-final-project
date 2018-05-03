#ifndef ECE_6122_FINAL_PROJECT_SPHERE_H
#define ECE_6122_FINAL_PROJECT_SPHERE_H


#include "Vector3.h"

/**
 * Here is the geometry of the project. The goal of a bubble was never reached
 * but it is the initial reason I choice a sphere geometry instead of the
 * square.  There was a good amount of time developing this class. I decided
 * to only work with spheres because of how frustrating the implimentation of
 * the recursion became.  Although I do not have other geometries, the sphere
 * truely helped illistrate the major concepts.
 */
class Sphere
{
public:
	/* initialize */
	Vector3f position;                     // relative position of the sphere
	float radius, radius2;                 // sphere radius and radius^2
	Vector3f surfaceColor, emissionColor;  // surface color and emission - for a light source
	float transparency, reflection;        // surface transparency and reflectivity

	/**
	 * Constructor to initialize the sphere
	 * @param c Center position of the sphere
	 * @param r Radius of the sphere
	 * @param sc Surface color of the sphere (RGB values 0-to-1)
	 * @param refl Reflectivity of the sphere
	 * @param transp Transparency of the sphere (0=opaque, 1=transparent)
	 * @param ec Emission color of the sphere (0 except for light sources)
	 */
	Sphere(
		const Vector3f &c,
		const float &r,
		const Vector3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const Vector3f &ec = 0) :
		position(c),
		radius(r),
		radius2(r*r),
		surfaceColor(sc),
		emissionColor(ec),
		transparency(transp),
		reflection(refl)
	{ /* empty contructor */ }

	/**
	 * Compute a ray-sphere intersection
	 * @param rayOrigin TODO Document
	 * @param rayDirection TODO Document
	 * @param t0 TODO Document
	 * @param t1 TODO Document
	 * @return TODO Document
	 */
	bool intersect(const Vector3f &rayOrigin, const Vector3f &rayDirection, float &t0, float &t1) const
	{
		Vector3f rayLen = position - rayOrigin;

		float DOTrayDirection = rayLen.dot(rayDirection);
		if (DOTrayDirection < 0)
		{
			return false;
		}

		float dist = rayLen.dot(rayLen) - DOTrayDirection * DOTrayDirection;
		if (dist > radius2)
		{
			return false;
		}

		float dist2 = sqrt(radius2 - dist);
		t0 = DOTrayDirection - dist2;
		t1 = DOTrayDirection + dist2;

		return true;
	}

};


#endif //ECE_6122_FINAL_PROJECT_SPHERE_H
