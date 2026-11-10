#pragma once

#include <cassert>
#include <cmath>
#include <Math/Angle.hpp>
#include <Math/Constants.hpp>
#include <Math/Matrix4.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>



namespace Math
{

	[[nodiscard]] inline bool Is_valid_perspective_infinite(const Radians fov_y, const float aspect, const float z_near) noexcept
	{
		return fov_y.value > 0.0f && fov_y.value < Pi &&
			   std::isfinite(aspect) && aspect > 0.0f &&
			   std::isfinite(z_near) && z_near > 0.0f;
	}

	[[nodiscard]] inline bool Is_valid_perspective(const Radians fov_y, const float aspect, const float z_near, const float z_far) noexcept
	{
		return Is_valid_perspective_infinite(fov_y, aspect, z_near) && std::isfinite(z_far) && z_far > z_near;
	}


	[[nodiscard]] inline bool Is_valid_orthographic(const float left, const float right, const float bottom, const float top, const float z_near, const float z_far) noexcept
	{
		return std::isfinite(left) && std::isfinite(right) && std::isfinite(bottom) && std::isfinite(top) &&
			   std::isfinite(z_near) && std::isfinite(z_far) &&
			   right != left && top != bottom && z_far != z_near;
	}

	namespace detail
	{
		[[nodiscard]] inline Matrix4 Perspective_from_depth(const Radians fov_y, const float aspect, const float depth_scale, const float depth_offset) noexcept
		{
			const float focal = 1.0f / std::tan(fov_y.value * 0.5f);
			return Matrix4(Vector4(focal / aspect, 0.0f, 0.0f, 0.0f),
						   Vector4(0.0f, focal, 0.0f, 0.0f),
						   Vector4(0.0f, 0.0f, depth_scale, -1.0f),
						   Vector4(0.0f, 0.0f, depth_offset, 0.0f));
		}

		[[nodiscard]] constexpr Matrix4 Orthographic_from_depth(const float left, const float right, const float bottom, const float top, const float depth_scale, const float depth_offset) noexcept
		{
			return Matrix4(Vector4(2.0f / (right - left), 0.0f, 0.0f, 0.0f),
						   Vector4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f),
						   Vector4(0.0f, 0.0f, depth_scale, 0.0f),
						   Vector4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), depth_offset, 1.0f));
		}

		[[nodiscard]] constexpr Matrix4 Flip_y(const Matrix4& projection) noexcept
		{
			return Matrix4(Vector4(projection[0].x, -projection[0].y, projection[0].z, projection[0].w),
						   Vector4(projection[1].x, -projection[1].y, projection[1].z, projection[1].w),
						   Vector4(projection[2].x, -projection[2].y, projection[2].z, projection[2].w),
						   Vector4(projection[3].x, -projection[3].y, projection[3].z, projection[3].w));
		}
	}


	[[nodiscard]] inline Matrix4 Perspective_standard(const Radians fov_y, const float aspect, const float z_near, const float z_far) noexcept
	{
		assert(Is_valid_perspective(fov_y, aspect, z_near, z_far) && "Perspective_standard: requires 0 < fov_y < pi, aspect > 0 and 0 < z_near < z_far");
		return detail::Perspective_from_depth(fov_y, aspect, z_far / (z_near - z_far), z_near * z_far / (z_near - z_far));
	}

	[[nodiscard]] inline Matrix4 Perspective_reverse_z(const Radians fov_y, const float aspect, const float z_near, const float z_far) noexcept
	{
		assert(Is_valid_perspective(fov_y, aspect, z_near, z_far) && "Perspective_reverse_z: requires 0 < fov_y < pi, aspect > 0 and 0 < z_near < z_far");
		return detail::Perspective_from_depth(fov_y, aspect, z_near / (z_far - z_near), z_near * z_far / (z_far - z_near));
	}


	[[nodiscard]] inline Matrix4 Perspective_reverse_z_infinite(const Radians fov_y, const float aspect, const float z_near) noexcept
	{
		assert(Is_valid_perspective_infinite(fov_y, aspect, z_near) && "Perspective_reverse_z_infinite: requires 0 < fov_y < pi, aspect > 0 and z_near > 0");
		return detail::Perspective_from_depth(fov_y, aspect, 0.0f, z_near);
	}

	[[nodiscard]] inline Matrix4 Perspective_vulkan(const Radians fov_y, const float aspect, const float z_near, const float z_far) noexcept
	{
		return detail::Flip_y(Perspective_reverse_z(fov_y, aspect, z_near, z_far));
	}

	[[nodiscard]] inline Matrix4 Perspective_vulkan_infinite(const Radians fov_y, const float aspect, const float z_near) noexcept
	{
		return detail::Flip_y(Perspective_reverse_z_infinite(fov_y, aspect, z_near));
	}

	[[nodiscard]] inline Matrix4 Orthographic_standard(const float left, const float right, const float bottom, const float top, const float z_near, const float z_far) noexcept
	{
		assert(Is_valid_orthographic(left, right, bottom, top, z_near, z_far) && "Orthographic_standard: requires finite values with right != left, top != bottom and z_far != z_near");
		return detail::Orthographic_from_depth(left, right, bottom, top, 1.0f / (z_near - z_far), z_near / (z_near - z_far));
	}

	[[nodiscard]] inline Matrix4 Orthographic_reverse_z(const float left, const float right, const float bottom, const float top, const float z_near, const float z_far) noexcept
	{
		assert(Is_valid_orthographic(left, right, bottom, top, z_near, z_far) && "Orthographic_reverse_z: requires finite values with right != left, top != bottom and z_far != z_near");
		return detail::Orthographic_from_depth(left, right, bottom, top, 1.0f / (z_far - z_near), z_far / (z_far - z_near));
	}

	[[nodiscard]] inline Matrix4 Orthographic_vulkan(const float left, const float right, const float bottom, const float top, const float z_near, const float z_far) noexcept
	{
		return detail::Flip_y(Orthographic_reverse_z(left, right, bottom, top, z_near, z_far));
	}

	[[nodiscard]] constexpr Vector3 Project_to_ndc(const Vector3 point, const Matrix4& transform) noexcept
	{
		const Vector4 clip = transform * Vector4(point, 1.0f);
		assert(clip.w != 0.0f && "Project_to_ndc: the point cannot be on the camera plane (w = 0)");
		return clip.Xyz() / clip.w;
	}

	[[nodiscard]] constexpr Vector3 Unproject(const Vector3 ndc, const Matrix4& inverse_transform) noexcept
	{
		const Vector4 point = inverse_transform * Vector4(ndc, 1.0f);
		assert(point.w != 0.0f && "Unproject: the point cannot be at infinity (w = 0)");
		return point.Xyz() / point.w;
	}
}
