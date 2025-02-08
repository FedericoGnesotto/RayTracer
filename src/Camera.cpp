
#include <RayTracer/Camera.h>

#include <iostream>
#include <numbers>
Camera::Camera(double verticalFOV, double nearClipPlane, double farClipPlane)
	: m_verticalFOV{verticalFOV}
	, m_nearClip{nearClipPlane}
	, m_farClip{farClipPlane}
{
	m_forwardDir = vec3{0., 0., -1.};
	m_position = vec3{0., 0., 6.};
	computeView();
	computeProjection();
	computeRayDirections();
}

const std::vector<vec3>& Camera::rayDirections() const { return m_rayDirections; }

const vec3& Camera::position() const { return m_position; }

void Camera::update(const vec3& forward, const vec3& positionDelta)
{
	m_forwardDir = forward;
	m_position += positionDelta;
	computeView();
	computeRayDirections();
}


void Camera::resize(int width, int height)
{
	if (width == m_viewportWidth && m_viewportHeight == height)
		return;

	m_viewportWidth = width;
	m_viewportHeight = height;

	computeProjection();
	computeRayDirections();
}

void Camera::reset()
{
	m_forwardDir = vec3{0., 0., -1.};
	m_position = vec3{0., 0., 6.};
	computeView();
	computeRayDirections();
}

void Camera::computeProjection()
{
	m_projection = perspective(
		m_verticalFOV * std::numbers::pi / 180.0, m_viewportWidth / (double)m_viewportHeight, m_nearClip, m_farClip);
	m_projectionInv = m_projection.inverse();
}

void Camera::computeView()
{
	m_view = lookAt(vec3{0., 1., 0.});
	m_viewInv = m_view.inverse();
}

void Camera::computeRayDirections()
{
	m_rayDirections.resize((uint32_t)m_viewportHeight * (uint32_t)m_viewportWidth);

	for (uint32_t y = 0; y < m_viewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_viewportWidth; x++)
		{
			vec2 coord = {x / (float)m_viewportWidth, y / (double)m_viewportHeight};
			coord = (coord * 2.0) - vec2::Ones();    // NDC

			vec4 target = m_projectionInv * vec4(coord.x(), coord.y(), 1., 1.);

			vec4 v = vec4::Zero();
			v << (target.head(3) / target.w()).normalized();      // View space
			const vec3 rayDirection = (m_viewInv * v).head(3);    // World space
			m_rayDirections[x + y * m_viewportWidth] = rayDirection;
		}
	}
}

mat4 Camera::lookAt(const vec3& upVectorWorld)    // has to be normalized
{
	assert(std::abs(upVectorWorld.norm() - 1.0) < 1e-5);

	m_forwardDir = m_forwardDir.normalized();

	const vec3 cameraRight = m_forwardDir.cross(upVectorWorld).normalized();

	vec3 cameraUp = cameraRight.cross(m_forwardDir).normalized();

	mat4 M{};
	M.row(0) << cameraRight.x(), cameraRight.y(), cameraRight.z(), 0.;
	M.row(1) << cameraUp.x(), cameraUp.y(), cameraUp.z(), 0.;
	M.row(2) << -m_forwardDir.x(), -m_forwardDir.y(), -m_forwardDir.z(), 0.;
	M.row(3) << 0., 0., 0., 1.;

	mat4 T = mat4::Identity();
	T.col(3) = vec4{-m_position.x(), -m_position.y(), -m_position.z(), 1};

	return M * T;
}

mat4 Camera::perspective(double verticalFOVRad, double aspectRatio, double nearClip, double farClip)
{
	assert(aspectRatio != 0.0);
	assert(farClip != nearClip);

	const double tanHalfFovy = std::tan(verticalFOVRad);    // do not divide by two, since we use already the half FOV angle

	mat4 Result = mat4::Zero();
	Result(0, 0) = 1.0 / (aspectRatio * tanHalfFovy);
	Result(1, 1) = 1.0 / (tanHalfFovy);
	Result(2, 2) = -(farClip + nearClip) / (farClip - nearClip);
	Result(2, 3) = -1.0;
	Result(3, 2) = -(2.0 * farClip * nearClip) / (farClip - nearClip);
	return Result;
}
