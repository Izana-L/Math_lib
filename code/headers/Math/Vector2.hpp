#pragma once 

#include <cstddef>
#include <type_traits>

namespace Math
{

	struct Vector2
	{
		float x, y;

		constexpr Vector2() noexcept : x(0.0f), y(0.0f){}

		constexpr Vector2(const float _x, const float _y) noexcept : x(_x), y(_y) {}
		constexpr explicit Vector2(const float value) noexcept : x(value), y(value) {}

		[[nodiscard]] constexpr float& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr const float& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector2& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y );
		}
		[[nodiscard]] constexpr bool operator!=(const Vector2& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y );
		}

	};
	static_assert(sizeof(Vector2) == 2 * sizeof(float), "Vector2 must be exactly 2 floats in size");
	static_assert(alignof(Vector2) == alignof(float), "Vector2 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector2>, "Vector2 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2>, "Vector2 must be trivially_copyable");
	static_assert(offsetof(Vector2, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2, y) == sizeof(float), "y must follow x");

}