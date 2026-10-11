#pragma once 

#include <cstddef>
#include <type_traits>
#include <Math/Vector2int.hpp>

namespace Math
{
	

	struct Vector3int
	{
		std::int32_t x, y, z;

		constexpr Vector3int() noexcept : x(0),y(0), z(0){}

		constexpr Vector3int(const std::int32_t _x, const std::int32_t _y, const std::int32_t _z ) noexcept : x(_x), y(_y), z(_z) {}
		constexpr Vector3int(Vector2int _vector2, const std::int32_t _z) noexcept : x(_vector2.x), y(_vector2.y), z(_z) {}
		constexpr explicit Vector3int(const std::int32_t value) noexcept : x(value), y(value), z(value) {}

		[[nodiscard]] constexpr std::int32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr const std::int32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr std::int32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::int32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector3int& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector3int& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z);
		}
		[[nodiscard]] constexpr Vector2int Xy() const noexcept
		{
			return Vector2int(x,y);
		}
	};
	static_assert(sizeof(Vector3int) == 3 * sizeof(std::int32_t), "Vector3int must be exactly 3 ints in size");
	static_assert(alignof(Vector3int) == alignof(std::int32_t), "Vector3int must not be aligned more strictly than a ints");
	static_assert(std::is_standard_layout_v<Vector3int>, "Vector3int must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3int>, "Vector3int must be trivially_copyable");
	static_assert(offsetof(Vector3int, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3int, y) == sizeof(std::int32_t), "y must follow x");
	static_assert(offsetof(Vector3int, z) == 2 * sizeof(std::int32_t), "z must follow y");
}