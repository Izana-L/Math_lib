#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Math
{

	struct Vector2i
	{
		std::int32_t x, y;

		constexpr Vector2i() noexcept : x(0), y(0){}

		constexpr Vector2i(const std::int32_t _x, const std::int32_t _y) noexcept : x(_x), y(_y) {}
		constexpr explicit Vector2i(const std::int32_t value) noexcept : x(value), y(value) {}

		[[nodiscard]] constexpr std::int32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr const std::int32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr std::int32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::int32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector2i& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y );
		}
		[[nodiscard]] constexpr bool operator!=(const Vector2i& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y );
		}

	};
	static_assert(sizeof(Vector2i) == 2 * sizeof(std::int32_t), "Vector2i must be exactly 2 ints in size");
	static_assert(alignof(Vector2i) == alignof(std::int32_t), "Vector2i must not be aligned more strictly than a ints");
	static_assert(std::is_standard_layout_v<Vector2i>, "Vector2i must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2i>, "Vector2i must be trivially_copyable");
	static_assert(offsetof(Vector2i, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2i, y) == sizeof(std::int32_t), "y must follow x");

}