#ifndef ECE_6122_FINAL_PROJECT_VECTOR3_H
#define ECE_6122_FINAL_PROJECT_VECTOR3_H


#include <cmath>
#include <iostream>

/* constructor */
template <typename Tvalue>

/* This class handles the matrix operations for the vectors.  This is extremely
   important for ray tracing because this is going to be recursively called
   upon.  This took a good chunk of my time to make sure this is ironed out.   */
class Vector3
{
public:
	/* initialize */
	Tvalue x, y, z;
	Vector3() : x(Tvalue(0)), y(Tvalue(0)), z(Tvalue(0)) {}
	Vector3(Tvalue xx) : x(xx), y(xx), z(xx) {}
	Vector3(Tvalue xx, Tvalue yy, Tvalue zz) : x(xx), y(yy), z(zz) {}

	/* normalize function */
	Vector3& normalize()
	{
		Tvalue normal = length2();
		if (normal > 0) {
			x *= (1 / sqrt(normal)),
				y *= (1 / sqrt(normal)),
				z *= (1 / sqrt(normal));
		}
		return *this;
	}

	/* helper functions for operators when dealing with 3D vectors.  Makes it earier for implimentation and logic later on */
	Vector3<Tvalue> operator * ( const Tvalue &mul ) const { return Vector3<Tvalue>( x*mul, y*mul, z*mul ); }

	Vector3<Tvalue> operator * ( const Vector3<Tvalue> &mulv ) const { return Vector3<Tvalue>( x*mulv.x, y*mulv.y, z*mulv.z ); }

	Tvalue dot( const Vector3<Tvalue> &d ) const { return x*d.x + y*d.y + z*d.z; }

	Vector3<Tvalue> operator - ( const Vector3<Tvalue> &sub ) const { return Vector3<Tvalue>( x-sub.x, y-sub.y, z-sub.z ); }

	Vector3<Tvalue> operator + ( const Vector3<Tvalue> &p ) const { return Vector3<Tvalue>( x+p.x, y+p.y, z+p.z ); }

	Vector3<Tvalue>& operator += ( const Vector3<Tvalue> &pe ) { x+=pe.x, y+=pe.y, z+=pe.z; return *this; }

	Vector3<Tvalue>& operator *= ( const Vector3<Tvalue> &mule ) { x*=mule.x, y*=mule.y, z*=mule.z; return *this; }

	Vector3<Tvalue> operator - () const { return Vector3<Tvalue>(-x, -y, -z ); }

	Tvalue length2() const { return x*x + y*y + z*z; }

	Tvalue length() const { return sqrt(length2()); }

	friend std::ostream & operator << (std::ostream &os, const Vector3<Tvalue> &tv)
	{
		os << "[" << tv.x << " " << tv.y << " " << tv.z << "]";
		return os;
	}

};

typedef Vector3<float> Vector3f;


#endif //ECE_6122_FINAL_PROJECT_VECTOR3_H
