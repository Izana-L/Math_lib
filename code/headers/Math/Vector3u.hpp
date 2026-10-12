#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <Math/Vector2u.hpp>

namespace Math
{
	

	struct Vector3u
	{
		std::uint32_t x, y, z;

		constexpr Vector3u() noexcept : x(0),y(0), z(0){}

		constexpr Vector3u(const std::uint32_t _x, const std::uint32_t _y, const std::uint32_t _z ) noexcept : x(_x), y(_y), z(_z) {}
		constexpr Vector3u(Vector2u _vector2, const std::uint32_t _z) noexcept : x(_vector2.x), y(_vector2.y), z(_z) {}
		constexpr explicit Vector3u(const std::uint32_t value) noexcept : x(value), y(value), z(value) {}

		[[nodiscard]] constexpr std::uint32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr const std::uint32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr std::uint32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::uint32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector3u& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector3u& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z);
		}
		[[nodiscard]] constexpr Vector2u Xy() const noexcept
		{
			return Vector2u(x,y);
		}
	};
	static_assert(sizeof(Vector3u) == 3 * sizeof(std::uint32_t), "Vector3u must be exactly 3 uint in size");
	static_assert(alignof(Vector3u) == alignof(std::uint32_t), "Vector3u must not be aligned more strictly than a uint");
	static_assert(std::is_standard_layout_v<Vector3u>, "Vector3u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3u>, "Vector3u must be trivially_copyable");
	static_assert(offsetof(Vector3u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3u, y) == sizeof(std::uint32_t), "y must follow x");
	static_assert(offsetof(Vector3u, z) == 2 * sizeof(std::uint32_t), "z must follow y");
}