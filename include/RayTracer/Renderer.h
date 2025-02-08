#pragma once

#include <RayTracer/Sphere.h>

#include <vector>

class Camera;
class QImage;

class Renderer
{
public:
	void render(QImage& img, const Camera& camera, const std::vector<Sphere>& spheres);

private:
	vec3 computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere);
};
