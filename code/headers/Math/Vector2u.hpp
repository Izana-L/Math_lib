#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Math
{

	struct Vector2u
	{
		std::uint32_t x, y;

		constexpr Vector2u() noexcept : x(0), y(0){}

		constexpr Vector2u(const std::uint32_t _x, const std::uint32_t _y) noexcept : x(_x), y(_y) {}
		constexpr explicit Vector2u(const std::uint32_t value) noexcept : x(value), y(value) {}

		[[nodiscard]] constexpr std::uint32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr const std::uint32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr std::uint32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::uint32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector2u& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y );
		}
		[[nodiscard]] constexpr bool operator!=(const Vector2u& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y );
		}

	};
	static_assert(sizeof(Vector2u) == 2 * sizeof(std::uint32_t), "Vector2u must be exactly 2 uint in size");
	static_assert(alignof(Vector2u) == alignof(std::uint32_t), "Vector2u must not be aligned more strictly than a uint");
	static_assert(std::is_standard_layout_v<Vector2u>, "Vector2u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2u>, "Vector2u must be trivially_copyable");
	static_assert(offsetof(Vector2u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2u, y) == sizeof(std::uint32_t), "y must follow x");

}