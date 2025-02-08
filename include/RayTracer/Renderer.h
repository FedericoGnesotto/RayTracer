#pragma once

#include <RayTracer/Sphere.h>

#include <optional>
#include <vector>

class Camera;
class QImage;

struct HitInfo
{
	vec3 color;
	double hitDistanceSquared;
};

class Renderer
{
public:
	void render(QImage& img, const Camera& camera, const std::vector<Sphere>& spheres);

private:
	std::optional<HitInfo> computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere);
};
