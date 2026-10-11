#pragma once 

#include <cstddef>
#include <type_traits>

namespace Math
{
	static_assert(sizeof(Vector3) == 3 * sizeof(float), "Vector3 must be exactly 3 floats in size");
	static_assert(alignof(Vector3) == alignof(float), "Vector3 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector3>, "Vector3 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3>, "Vector3 must be trivially_copyable");
	static_assert(offsetof(Vector3, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Vector3, z) == 2 * sizeof(float), "z must follow y");

	struct Vector3
	{
		float x, y, z;

		constexpr Vector3() : x(0.0f),y(0.0f), z(0.0f){}

		constexpr Vector3(const float _x, const float _y, const float _z ) : x(_x), y(_y), z(_z) {}
		constexpr explicit Vector3(const float value) : x(value), y(value), z(value) {}

		[[nodiscard]] constexpr float& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr const float& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &x; }

		bool operator==(const Vector3& other_vector) const
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z);
		}
		bool operator!=(const Vector3& other_vector) const
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z);
		}

	};

}