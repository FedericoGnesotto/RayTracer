#pragma once

#include <RayTracer/Math.h>

#include <vector>

class Camera
{
public:
	Camera(double verticalFOV, double nearClipPlane, double farClipPlane);
	Camera() = default;
	const std::vector<vec3>& rayDirections() const;
	const vec3& position() const;
	const vec3& forwardDir() const { return m_forwardDir; }
	void update(const vec3& forward, const vec3& positionDelta);
	void resize(int width, int height);
	void reset();

private:
	void computeProjection();
	void computeView();
	void computeRayDirections();

	mat4 lookAt(const vec3& upVectorWorld);
	mat4 perspective(double verticalFOV, double aspectRatio, double nearClip, double farClip);

	mat4 m_projection = mat4::Zero();
	mat4 m_view = mat4::Zero();

	mat4 m_projectionInv = mat4::Zero();
	mat4 m_viewInv = mat4::Zero();

	double m_verticalFOV = 45.0;    // this is half of the total FOV
	double m_nearClip = 0.1;
	double m_farClip = 100.0;

	vec3 m_forwardDir = vec3::Zero();
	vec3 m_position = vec3::Zero();

	int m_viewportWidth = 0, m_viewportHeight = 0;

	std::vector<vec3> m_rayDirections;
};