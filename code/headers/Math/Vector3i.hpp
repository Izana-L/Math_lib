#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <Math/Vector2i.hpp>

namespace Math
{
	

	struct Vector3i
	{
		std::int32_t x, y, z;

		constexpr Vector3i() noexcept : x(0),y(0), z(0){}

		constexpr Vector3i(const std::int32_t _x, const std::int32_t _y, const std::int32_t _z ) noexcept : x(_x), y(_y), z(_z) {}
		constexpr Vector3i(Vector2i _vector2, const std::int32_t _z) noexcept : x(_vector2.x), y(_vector2.y), z(_z) {}
		constexpr explicit Vector3i(const std::int32_t value) noexcept : x(value), y(value), z(value) {}

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

		[[nodiscard]] constexpr bool operator==(const Vector3i& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector3i& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z);
		}
		[[nodiscard]] constexpr Vector2i Xy() const noexcept
		{
			return Vector2i(x,y);
		}
	};
	static_assert(sizeof(Vector3i) == 3 * sizeof(std::int32_t), "Vector3i must be exactly 3 ints in size");
	static_assert(alignof(Vector3i) == alignof(std::int32_t), "Vector3i must not be aligned more strictly than a ints");
	static_assert(std::is_standard_layout_v<Vector3i>, "Vector3i must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3i>, "Vector3i must be trivially_copyable");
	static_assert(offsetof(Vector3i, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3i, y) == sizeof(std::int32_t), "y must follow x");
	static_assert(offsetof(Vector3i, z) == 2 * sizeof(std::int32_t), "z must follow y");
}