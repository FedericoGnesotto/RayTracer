#pragma once

#include <Eigen/Geometry>

using vec3 = Eigen::Vector3d;
using vec3i = Eigen::Vector3i;
using vec4 = Eigen::Vector4d;
using vec4i = Eigen::Vector4i;
using vec2 = Eigen::Vector2d;
using veci = Eigen::Vector2i;

using mat4 = Eigen::Matrix4d;
using mat4i = Eigen::Matrix4i;
using mat3 = Eigen::Matrix3d;
using mat3i = Eigen::Matrix3i;


inline vec4 clamp(vec4 v, double min, double max)
{
	return vec4(std::clamp(v[0], min, max), std::clamp(v[1], min, max), std::clamp(v[2], min, max), std::clamp(v[3], min, max));
}
