#pragma once
#pragma once

#include <RayTracer/Scene.h>
#include <RayTracer/Sphere.h>

#include <optional>
#include <vector>

class Camera;
class QImage;

struct HitInfo
{
	vec3 color;
	vec3 hitPosWorld;
	double hitDistRay;    // hitDistance along ray
	vec3 hitPosNormal;
};

class Renderer
{
public:
	void render(QImage& img, const Camera& camera, const Scene& scene);

private:
	std::optional<HitInfo> computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere, int bounceIdx);
};
