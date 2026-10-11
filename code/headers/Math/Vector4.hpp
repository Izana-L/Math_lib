#pragma once 

#include <cstddef>
#include <type_traits>

namespace Math
{


	struct Vector4
	{
		float x, y, z, w;

		constexpr Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		constexpr Vector4(const float _x, const float _y, const float _z, const float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		constexpr explicit Vector4(const float value) noexcept : x(value), y(value), z(value), w(value) {}

		[[nodiscard]] constexpr float& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z :w;
		}
		[[nodiscard]] constexpr const float& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector4& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z && this->w == other_vector.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector4& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z || this->w != other_vector.w);
		}

	};

	static_assert(sizeof(Vector4) == 4 * sizeof(float), "Vector4 must be exactly 4 floats in size");
	static_assert(alignof(Vector4) == alignof(float), "Vector4 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector4>, "Vector4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4>, "Vector4 must be trivially_copyable");
	static_assert(offsetof(Vector4, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Vector4, z) == 2 * sizeof(float), "z must follow y");
	static_assert(offsetof(Vector4, w) == 3 * sizeof(float), "w must follow z");
}