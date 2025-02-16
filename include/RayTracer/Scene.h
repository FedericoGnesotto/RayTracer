#pragma once

#include <RayTracer/Math.h>
#include <RayTracer/Sphere.h>

struct Scene
{
	std::vector<Sphere> spheres;
	vec3 lightDir;
};