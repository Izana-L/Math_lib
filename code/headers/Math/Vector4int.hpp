#pragma once 

#include <cstddef>
#include <type_traits>
#include <Math/Vector3int.hpp>
namespace Math
{

	struct Vector4int
	{
		std::int32_t x, y, z, w;

		constexpr Vector4int() noexcept : x(0), y(0), z(0), w(0) {}

		constexpr Vector4int(const std::int32_t _x, const std::int32_t _y, const std::int32_t _z, const std::int32_t _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		constexpr Vector4int(Vector3int _vector3, const std::int32_t _w) noexcept : x(_vector3.x), y(_vector3.y), z(_vector3.z), w(_w) {}
		constexpr explicit Vector4int(const std::int32_t value) noexcept : x(value), y(value), z(value), w(value) {}

		[[nodiscard]] constexpr std::int32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z :w;
		}
		[[nodiscard]] constexpr const std::int32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr std::int32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::int32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector4int& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z && this->w == other_vector.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector4int& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z || this->w != other_vector.w);
		}
		[[nodiscard]] constexpr Vector3int Xyz() const noexcept
		{
			return Vector3int(x, y, z);
		}
	};

	static_assert(sizeof(Vector4int) == 4 * sizeof(std::int32_t), "Vector4 must be exactly 4 int in size");
	static_assert(alignof(Vector4int) == alignof(std::int32_t), "Vector4 must not be aligned more strictly than a int");
	static_assert(std::is_standard_layout_v<Vector4int>, "Vector4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4int>, "Vector4 must be trivially_copyable");
	static_assert(offsetof(Vector4int, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4int, y) == sizeof(std::int32_t), "y must follow x");
	static_assert(offsetof(Vector4int, z) == 2 * sizeof(std::int32_t), "z must follow y");
	static_assert(offsetof(Vector4int, w) == 3 * sizeof(std::int32_t), "w must follow z");
}