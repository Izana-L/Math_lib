#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <Math/Matrix4.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

namespace Math
{

	struct FrustumPlanes
	{
		Vector4 left, right, bottom, top, z_near, z_far;
	};

	static_assert(sizeof(FrustumPlanes) == 6 * sizeof(Vector4), "FrustumPlanes must be exactly 6 Vector4 in size");
	static_assert(alignof(FrustumPlanes) == alignof(Vector4), "FrustumPlanes must not be aligned more strictly than a Vector4");
	static_assert(std::is_standard_layout_v<FrustumPlanes>, "FrustumPlanes must be standard_layout");
	static_assert(std::is_trivially_copyable_v<FrustumPlanes>, "FrustumPlanes must be trivially_copyable");
	static_assert(offsetof(FrustumPlanes, left) == 0, "left must be at offset 0");
	static_assert(offsetof(FrustumPlanes, right) == sizeof(Vector4), "right must follow left");
	static_assert(offsetof(FrustumPlanes, bottom) == 2 * sizeof(Vector4), "bottom must follow right");
	static_assert(offsetof(FrustumPlanes, top) == 3 * sizeof(Vector4), "top must follow bottom");
	static_assert(offsetof(FrustumPlanes, z_near) == 4 * sizeof(Vector4), "z_near must follow top");
	static_assert(offsetof(FrustumPlanes, z_far) == 5 * sizeof(Vector4), "z_far must follow z_near");

	namespace detail
	{

		[[nodiscard]] constexpr Vector4 Row(const Matrix4& matrix, const std::size_t index) noexcept
		{
			assert(index < 4 && "Row: index must be less than 4");
			return Vector4(matrix[0][index], matrix[1][index], matrix[2][index], matrix[3][index]);
		}


		[[nodiscard]] inline Vector4 Normalize_plane(const Vector4 plane) noexcept
		{
			assert(Is_normalizable(plane.Xyz()) && "Normalize_plane: the plane normal cannot be zero or have infinite or NaN values");
			return plane / Length(plane.Xyz());
		}


		[[nodiscard]] inline FrustumPlanes Make_frustum_planes(const Matrix4& transform, const Vector4 near_plane, const Vector4 far_plane) noexcept
		{
			const Vector4 row0 = Row(transform, 0);
			const Vector4 row1 = Row(transform, 1);
			const Vector4 row3 = Row(transform, 3);
			return FrustumPlanes{
				Normalize_plane(row3 + row0),
				Normalize_plane(row3 - row0),
				Normalize_plane(row3 + row1),
				Normalize_plane(row3 - row1),
				Normalize_plane(near_plane),

				Is_normalizable(far_plane.Xyz()) ? Normalize_plane(far_plane) : far_plane };
		}
	}

	[[nodiscard]] inline FrustumPlanes Extract_frustum_planes_standard(const Matrix4& transform) noexcept
	{
		const Vector4 row2 = detail::Row(transform, 2);
		const Vector4 row3 = detail::Row(transform, 3);
		return detail::Make_frustum_planes(transform, row2, row3 - row2);
	}


	[[nodiscard]] inline FrustumPlanes Extract_frustum_planes_reverse_z(const Matrix4& transform) noexcept
	{
		const Vector4 row2 = detail::Row(transform, 2);
		const Vector4 row3 = detail::Row(transform, 3);
		return detail::Make_frustum_planes(transform, row3 - row2, row2);
	}

	[[nodiscard]] constexpr float Plane_distance(const Vector4 plane, const Vector3 point) noexcept
	{
		return Dot(plane, Vector4(point, 1.0f));
	}

	[[nodiscard]] constexpr bool Is_inside(const FrustumPlanes& frustum, const Vector3 point) noexcept
	{
		return Plane_distance(frustum.left, point) >= 0.0f && Plane_distance(frustum.right, point) >= 0.0f &&
			   Plane_distance(frustum.bottom, point) >= 0.0f && Plane_distance(frustum.top, point) >= 0.0f &&
			   Plane_distance(frustum.z_near, point) >= 0.0f && Plane_distance(frustum.z_far, point) >= 0.0f;
	}
}
