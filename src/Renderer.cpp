#include <RayTracer/Camera.h>
#include <RayTracer/Math.h>
#include <RayTracer/Renderer.h>

#include <QImage>

void Renderer::render(QImage& img, const Camera& camera, const std::vector<Sphere>& spheres)
{
	const int w = img.width();
	const int h = img.height();

	for (const auto& sphere : spheres)
	{
		for (int j = 0; j < img.height(); ++j)
		{
			for (int i = 0; i < img.width(); ++i)
			{
				vec3 val = clamp(computePixelVal(camera.position(), camera.rayDirections()[i + j * w], sphere), 0., 1.);

				uint32_t argb = uint8_t(255.f) << 24 | uint8_t(val[0] * 255.f) << 16 | uint8_t(val[1] * 255.f) << 8 |
								uint8_t(val[2] * 255.f);
				img.setPixel(i, j, argb);
			}
		}
	}
}


vec3 Renderer::computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere)
{
	const vec3 origin = rayOrigin - sphere.origin;

	const double a = rayDir.dot(rayDir);    //x * x + y * y + rayDir.z() * rayDir.z();
	const double b = 2.0 * rayDir.dot(origin);
	const double c = origin.dot(origin) - sphere.radius * sphere.radius;

	const double delta = b * b - 4.0 * a * c;

	if (delta < 0.0)
		return vec3{0, 0, 0};

	// closest solution is (-b - sqrt(delta))/(2a)
	double t1 = (-b - std::sqrt(delta)) / (2. * a);

	vec3 hitPosClosest = origin + t1 * rayDir;
	vec3 hitPosNormal = hitPosClosest - sphere.origin;
	hitPosNormal.normalize();

	vec3 lightDir = vec3(1., 1.0, -1.);
	lightDir.normalize();

	const double lightAlpha = std::max(-lightDir.dot(hitPosNormal), 0.);
	const vec3 color = lightAlpha * sphere.color;
	return color;
}
