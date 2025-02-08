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
				//double aspectRatio = w / double(h);
				//const double x = 2.0 * i / double(img.width()) - 1.0;    // normalize between -1 and 1
				//double y = 2.0 * j / (double)img.height() - 1.0;         // normalize between -1 and 1
				//y = y / aspectRatio;

				vec4 val = clamp(computePixelVal(camera.position(), camera.rayDirections()[i + j * w], sphere), 0., 1.);

				uint32_t argb = uint8_t(val[3] * 255.f) << 24 | uint8_t(val[0] * 255.f) << 16 | uint8_t(val[1] * 255.f) << 8 |
								uint8_t(val[2] * 255.f);
				img.setPixel(i, j, argb);
			}
		}
	}
}

vec4 Renderer::computePixelVal(double x, double y, const Sphere& sphere)
{
	const vec3 rayOrigin = {0., 0., 2.};
	const vec3 origin = rayOrigin - sphere.origin;

	const vec3 rayDir = {(double)x - sphere.origin.x(), (double)y - sphere.origin.y(), -1.0};

	const double a = rayDir.dot(rayDir);    //x * x + y * y + rayDir.z() * rayDir.z();
	const double b = 2.0 * rayDir.dot(origin);
	const double c = origin.dot(origin) - sphere.radius * sphere.radius;

	const double delta = b * b - 4.0 * a * c;

	if (delta < 0.0)
		return vec4{0, 0, 0, 1};

	// closest solution is (-b - sqrt(delta))/(2a)
	double t1 = (-b - std::sqrt(delta)) / (2. * a);

	vec3 hitPosClosest = origin + t1 * rayDir;
	vec3 hitPosNormal = hitPosClosest - sphere.origin;
	hitPosNormal.normalize();

	vec3 lightDir = vec3(1., 1.0, -1.);
	lightDir.normalize();

	const double lightAlpha = std::max(-lightDir.dot(hitPosNormal), 0.);
	const vec3 color = lightAlpha * vec3(1.0, 0., 1.0);
	return color.homogeneous();
}

vec4 Renderer::computePixelVal(const vec3& rayOrigin, const vec3& rayDir, const Sphere& sphere)
{
	const vec3 origin = rayOrigin - sphere.origin;

	const double a = rayDir.dot(rayDir);    //x * x + y * y + rayDir.z() * rayDir.z();
	const double b = 2.0 * rayDir.dot(origin);
	const double c = origin.dot(origin) - sphere.radius * sphere.radius;

	const double delta = b * b - 4.0 * a * c;

	if (delta < 0.0)
		return vec4{0, 0, 0, 1};

	// closest solution is (-b - sqrt(delta))/(2a)
	double t1 = (-b - std::sqrt(delta)) / (2. * a);

	vec3 hitPosClosest = origin + t1 * rayDir;
	vec3 hitPosNormal = hitPosClosest - sphere.origin;
	hitPosNormal.normalize();

	vec3 lightDir = vec3(1., 1.0, -1.);
	lightDir.normalize();

	const double lightAlpha = std::max(-lightDir.dot(hitPosNormal), 0.);
	const vec3 color = lightAlpha * vec3(1.0, 0., 1.0);
	return color.homogeneous();
}
