#include <RayTracer/Camera.h>
#include <RayTracer/Math.h>
#include <RayTracer/Renderer.h>

#include <QImage>


vec3 reflect(const vec3& t, const vec3& n) { return (-t + 2. * n * (t.dot(n))); }

void Renderer::render(QImage& img, const Camera& camera, const Scene& scene)
{
	const int w = img.width();
	const int h = img.height();

	for (int j = 0; j < img.height(); ++j)
	{
		for (int i = 0; i < img.width(); ++i)
		{
			vec3 valClamped = vec3::Zero();

			// color should be the sky, in case we hit nothing
			HitInfo closestHitInfo{.color = vec3::Zero(), .hitDistRay = std::numeric_limits<double>::max()};

			vec3 color = vec3::Zero();
			const int maxBounce = 5;
			vec3 rayOrigin = camera.position();
			vec3 rayDir = camera.rayDirections()[i + j * w];
			for (int b = 0; b < maxBounce; ++b)
			{
				for (const auto& sphere : scene.spheres)
				{
					if (auto hitInfo = computePixelVal(rayOrigin, rayDir, sphere, b))
					{
						if (hitInfo->hitDistRay < closestHitInfo.hitDistRay)
						{
							closestHitInfo = hitInfo.value();
						}
					}
				}
				// if we hit something, we reflect and update the color, otherwise we break
				if (closestHitInfo.hitDistRay < std::numeric_limits<double>::max())
				{
					const double lightAlpha = std::max(-scene.lightDir.dot(closestHitInfo.hitPosNormal), 0.);
					color += lightAlpha * closestHitInfo.color * 1. / (b + 1);
					rayDir = reflect(-rayDir, closestHitInfo.hitPosNormal);
					rayOrigin = closestHitInfo.hitPosWorld + closestHitInfo.hitPosNormal * 0.0001;
				}
				else
				{
					break;
				}
			}

			valClamped = clamp(color, 0., 1.);

			uint32_t argb = uint8_t(255.f) << 24 | uint8_t(valClamped[0] * 255.f) << 16 | uint8_t(valClamped[1] * 255.f) << 8 |
							uint8_t(valClamped[2] * 255.f);
			img.setPixel(i, j, argb);
		}
	}
}


std::optional<HitInfo> Renderer::computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere, int bounceIdx)
{
	const vec3 origin = rayOrigin - sphere.origin;

	const double a = rayDir.dot(rayDir);    //x * x + y * y + rayDir.z() * rayDir.z();
	const double b = 2.0 * rayDir.dot(origin);
	const double c = origin.dot(origin) - sphere.radius * sphere.radius;

	const double delta = b * b - 4.0 * a * c;

	if (delta < 0.0)
		return std::nullopt;

	// closest solution is (-b - sqrt(delta))/(2a)
	double t1 = (-b - std::sqrt(delta)) / (2. * a);
	if (t1 < 0)    // we hit in the opposite direction of the ray
	{
		return std::nullopt;
	}

	vec3 hitPosClosest = origin + t1 * rayDir;           // still in the sphere reference system
	vec3 hitPosWorld = hitPosClosest + sphere.origin;    // add the origin to go back to world coordinates


	return HitInfo{
		.color = sphere.color, .hitPosWorld = hitPosWorld, .hitDistRay = t1, .hitPosNormal = hitPosClosest.normalized()};
}
