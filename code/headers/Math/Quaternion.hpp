#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <Math/Matrix3.hpp>


namespace Math
{
	struct Matrix4;
	struct Quaternion
	{
		float x, y, z, w;

		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Quaternion(const float _x, const float _y, const float _z, const float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}

		[[nodiscard]] constexpr float& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr const float& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr static Quaternion Identity() noexcept { return Quaternion(); }
		[[nodiscard]] static Quaternion From_matrix3(const Matrix3& matrix) noexcept;
		[[nodiscard]] static Quaternion From_matrix4(const Matrix4& matrix) noexcept;
		[[nodiscard]] friend constexpr bool operator==(const Quaternion& vector, const Quaternion& other_vector)  noexcept = default;

	};
	static_assert(sizeof(Quaternion) == 4 * sizeof(float), "Quaternion must be exactly 4 floats in size");
	static_assert(alignof(Quaternion) == alignof(float), "Quaternion must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Quaternion>, "Quaternion must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Quaternion>, "Quaternion  must be trivially_copyable");
	static_assert(offsetof(Quaternion, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Quaternion, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Quaternion, z) == 2 * sizeof(float), "z must follow y");
	static_assert(offsetof(Quaternion, w) == 3 * sizeof(float), "w must follow z");

	namespace detail
	{

		[[nodiscard]] constexpr bool Is_unit_quaternion(const Quaternion quaternion) noexcept
		{
			return Equals(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w, 1.0f);
		}
	}


	[[nodiscard]] constexpr Matrix3 To_matrix3(const Quaternion rotation) noexcept
	{
		assert(detail::Is_unit_quaternion(rotation) && "To_matrix3: 'rotation' must be a normalized quaternion");
		const float xx = rotation.x * rotation.x;
		const float yy = rotation.y * rotation.y;
		const float zz = rotation.z * rotation.z;
		const float xy = rotation.x * rotation.y;
		const float xz = rotation.x * rotation.z;
		const float yz = rotation.y * rotation.z;
		const float wx = rotation.w * rotation.x;
		const float wy = rotation.w * rotation.y;
		const float wz = rotation.w * rotation.z;
		return Matrix3(Vector3(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy)),
					   Vector3(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx)),
					   Vector3(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy)));
	}
}
