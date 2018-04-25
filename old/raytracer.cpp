/*=========================================================================

 ECE 6122 Final Project
 Author: Theodore J. LaGrow
 Date: 3/28/2018
 This is the final project, put up or shut up.
 
 =========================================================================*/


/* these files are always important :) */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>




/* Variable tp help control maximum recursion depth.  Important for speed.
   Without a k-d tree or some type of time improvement, I usually don't go
   over 35. */
#define MAX_RAY_DEPTH 25

/* Setting number of threads */

constexpr unsigned int NUMTHREADS = 64;


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

}; typedef Vector3<float> Vector3f;



/* Here is the geometry of the project. The goal of a bubble was never reached but it is the 
   initial reason I choice a sphere geometry instead of the square.  There was a good amount 
   of time developing this class. I decided to only work with spheres because of how frustrating
   the implimentation of the recursion became.  Although I do not have other geometries, the sphere 
   truely helped illistrate the major concepts.                                                     */
class Sphere
{
public:
    /* initialize */
    Vector3f position;                     /* relative position of the sphere                 */
    float radius, radius2;                 /* sphere radius and radius^2                      */
    Vector3f surfaceColor, emissionColor;  /* surface color and emission - for a light source */
    float transparency, reflection;        /* surface transparency and reflectivity           */
    
    /* initialize the sphere */
    Sphere(
        const Vector3f &c,
        const float &r,
        const Vector3f &sc,
        const float &refl = 0,
        const float &transp = 0,
        const Vector3f &ec = 0) :
        position(c), radius(r), radius2(r*r), surfaceColor(sc), emissionColor(ec), transparency(transp), reflection(refl)
        { /* empty contructor */ }


    /* Compute a ray-sphere intersection  */
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


/* Helper function for color mixing based on weight */
float weightDistrobution(const float &a, const float &b, const float &weightDistrobution)
{
    return a * (1 - weightDistrobution) + b * weightDistrobution;
}


/* The main trace function! It takes a standard ray as argument which is defined by 
its origin and direction. There is a test if this ray intersects any of the geometry 
in the scene (the geometry will be created later). If the ray intersects an object, 
the intersection point is computed, the normal at the intersection point is computed, 
and thus shade this point using this newly calculated information. Shading depends on 
the surface property of the primitive which is transparency, reflectiveness, 
diffuseablity. The function returns a color for the ray. If the ray intersects an 
object that is the color of the object at the intersection point, otherwise it 
returns the background color.                                                        */
Vector3f trace(

    const Vector3f &rayOrigin,
    const Vector3f &rayDirection,
    const std::vector<Sphere> &spheres,
    const int &depth
    )
{
    /* initialize */
    float tnear = INFINITY;
    const Sphere* sphere = NULL;
	
    /* find intersection of this ray with the sphere in the scene */
    for (unsigned i = 0; i < spheres.size(); ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayOrigin, rayDirection, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }


    /* return black or background color if there is no intersection */
    if (!sphere) {
        return Vector3f(2);
    }


    Vector3f surfaceColor = 0;  /* intialize color of the ray/surfaceof the object intersected by the ray */
    Vector3f pointOfIntersection = rayOrigin + rayDirection * tnear; /* point of intersection */
    Vector3f normalOfIntersection = pointOfIntersection - sphere->position; /* normal at the intersection */
    normalOfIntersection.normalize(); /* normalize */

    /* Start the checks and headaches */

    /* If the normal and the view direction are not opposite to each other, reverse the normal direction. 
       That means we are inside the sphere... so set the inside bool to true. Finally reverse the sign 
       of IdotN which we want positive.                                                                   */ 
    
    float bias = 1e-4; /* add some bias to the point from which we will be tracing to retain some color   */
    bool inside = false; /* initalize */
    if (rayDirection.dot(normalOfIntersection) > 0) 
    {
        normalOfIntersection = -normalOfIntersection, inside = true;
    }

    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {
        float facingRatio = -rayDirection.dot(normalOfIntersection);
        /* change the color mixing value to maximize the effect */
        float fresnelEffect = weightDistrobution(pow(1 - facingRatio, 3), 1, 0.1); /* this equation is everywhere */
        
        /* since all of the vectors are already normalized, the reflection direction is computed  */
        Vector3f reflectionDirection = rayDirection - normalOfIntersection * 2 * rayDirection.dot(normalOfIntersection);
        reflectionDirection.normalize(); /* normalize */
        Vector3f reflection = trace(pointOfIntersection + normalOfIntersection * bias, reflectionDirection, spheres, depth + 1);
        Vector3f refraction = 0;
        
        /* if the sphere is also transparent compute refraction ray (transmission) */
        if (sphere->transparency) {
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
    else {
        /* a diffuse object, we can stop marching */
        for (unsigned i = 0; i < spheres.size(); ++i) {
            if (spheres[i].emissionColor.x > 0) {
                // this is a light
                Vector3f transmission = 1;
                Vector3f lightDirection = spheres[i].position - pointOfIntersection;
                lightDirection.normalize(); /* normalize */
                for (unsigned j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(pointOfIntersection + normalOfIntersection * bias, lightDirection, t0, t1)) {
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission * std::max(float(0), normalOfIntersection.dot(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }
    
    return surfaceColor + sphere->emissionColor;
}


/* This is a helper function used to parallelize some of the operations in the function render */

void threading_func(Vector3f *p, const int &width, const unsigned &threaID, const int &numPerThread, const int &extra,
                     const float &invertedWidth, const float &invertedHeight, const float &angle, const std::vector<Sphere> &spheres, const float &aspectRatio ){
    if(threaID != (NUMTHREADS-1)){
        for(unsigned y = threaID*numPerThread; y < (threaID+1)*numPerThread; ++y){
            for(unsigned x = 0; x < width; ++x, ++p){
                float xx = (2 * ((x + 0.5) * invertedWidth) - 1) * angle * aspectRatio;
                float yy = (1 - 2 * ((y + 0.5) * invertedHeight)) * angle;
                Vector3f rayDirection(xx, yy, -1);
                rayDirection.normalize();
                *p = trace(Vector3f(0), rayDirection, spheres, 0);
            }
        }
    }else{
        for(unsigned y = threaID*numPerThread; y < ((threaID+1)*numPerThread)+extra; ++y){
            for(unsigned x = 0; x < width; ++x, ++p){
                float xx = (2 * ((x + 0.5) * invertedWidth) - 1) * angle * aspectRatio;
                float yy = (1 - 2 * ((y + 0.5) * invertedHeight)) * angle;
                Vector3f rayDirection(xx, yy, -1);
                rayDirection.normalize();
                *p = trace(Vector3f(0), rayDirection, spheres, 0);
            }
        }
    }
}

/* This is the rendering function! A camera ray for each pixel of the image is computed and then
   traced and finally returned with a color. If the ray hits a sphere, the color of the
   sphere at the intersection point is returned, else we return the background color. */


void render(const std::vector<Sphere> &spheres, const int &parallel)
{
    auto start = std::chrono::high_resolution_clock::now();

    //unsigned width = 3840, height = 2160; /* 16:9 aspect ratio, 4K image */
    unsigned width = 3240, height = 1080; /* 3:1 aspect ratio */
    Vector3f *image = new Vector3f[width * height], *pixel = image;
    float invertedWidth = 1 / float(width), invertedHeight = 1 / float(height);
    float fov = 30, aspectRatio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    if(!parallel) {
        for (unsigned y = 0; y < height; ++y) {
            for (unsigned x = 0; x < width; ++x, ++pixel) {
                float xx = (2 * ((x + 0.5) * invertedWidth) - 1) * angle * aspectRatio;
                float yy = (1 - 2 * ((y + 0.5) * invertedHeight)) * angle;
                Vector3f rayDirection(xx, yy, -1);
                rayDirection.normalize();
                *pixel = trace(Vector3f(0), rayDirection, spheres, 0);
            }
        }
    }else {
        std::thread threads[NUMTHREADS];
        int numPerThread = height / NUMTHREADS;
        int extra = height % NUMTHREADS;

        for (int i = 0; i < NUMTHREADS; ++i) {
            threads[i] = std::thread(threading_func, pixel + (i * numPerThread * width), width, i, numPerThread, extra,
                                     invertedWidth, invertedHeight, angle, spheres, aspectRatio);
        }

        for (int i = 0; i < NUMTHREADS; ++i) {
            threads[i].join();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    /* Save result to a pmm image!                                                       
       This method of saving works for mac, but you need to save to .ppm for window/linux */
    std::ofstream ppm("./sphere_primitive_example.ppm", std::ios::out | std::ios::binary);
    ppm << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ppm << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
               (unsigned char)(std::min(float(1), image[i].y) * 255) <<
               (unsigned char)(std::min(float(1), image[i].z) * 255);
    }
    ppm.close();
    delete [] image;
}


/* Its finally the main function! Whoot whoot!. This is where the geometries (primatives - in 
   our case, spheres) will be created! Since I wanted to show the shadows, I added a *huge* 
   sphere to subsitute as a floor and a sphere as the light source. */
int main(int argc, char **argv)
{
    /* Step on up! You've come to see the show! */
    std::vector<Sphere> spheres;
    
    /* Sphere order: position(x,y,z), radius, surface color(r,g,b), reflectivity, transparency, emission color */

    /* *huge* base sphere, I went with neon green to show the light coming back up on the other spheres */
    spheres.push_back(Sphere(Vector3f( 0.0, -60004, -20), 60000, Vector3f(0.22, 1.00, 0.07), 0, 0.0));
    
    /* the main actors! */
    spheres.push_back(Sphere(Vector3f( 0.0,  0,   -20),    4.0, Vector3f(1.00, 0.32, 0.36), 1, 0.5));  /* Red    */
    spheres.push_back(Sphere(Vector3f( 5.0, -1,   -15),    2.0, Vector3f(0.99, 0.81, 0.14), 1, 0.0));  /* Yellow */
    spheres.push_back(Sphere(Vector3f( 5.7, -0.8, -12),    0.7, Vector3f(0.00, 0.75, 1.00), 0, 0.0));  /* Blue   */
    spheres.push_back(Sphere(Vector3f( 12.0, 2.3, -25),    5.2, Vector3f(0.46, 0.05, 0.67), 0, 0.0));  /* Purple */
    spheres.push_back(Sphere(Vector3f(-5.5,  0,   -15),    2.8, Vector3f(0.90, 0.90, 0.90), 1, 0.0));  /* Black  */ 

    /* this is the light ball! */
    spheres.push_back(Sphere(Vector3f( 10,  20,  10),      4, Vector3f(0.00, 0.00, 0.00), 0, 0.0, Vector3f(3)));
    
    /* GOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!  And they are off to the races! */
    int parallel = 1;
    render(spheres, parallel);
    
    /* annnndd... finish :') */
    return 0;
}



