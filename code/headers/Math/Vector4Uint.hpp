#pragma once 

#include <cstddef>
#include <type_traits>
#include <Math/Vector3Uint.hpp>
namespace Math
{

	struct Vector4Uint
	{
		std::uint32_t x, y, z, w;

		constexpr Vector4Uint() noexcept : x(0), y(0), z(0), w(0) {}

		constexpr Vector4Uint(const std::uint32_t _x, const std::uint32_t _y, const std::uint32_t _z, const std::uint32_t _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		constexpr Vector4Uint(Vector3Uint _vector3, const std::uint32_t _w) noexcept : x(_vector3.x), y(_vector3.y), z(_vector3.z), w(_w) {}
		constexpr explicit Vector4Uint(const std::uint32_t value) noexcept : x(value), y(value), z(value), w(value) {}

		[[nodiscard]] constexpr std::uint32_t& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z :w;
		}
		[[nodiscard]] constexpr const std::uint32_t& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr std::uint32_t* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const std::uint32_t* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector4Uint& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z && this->w == other_vector.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector4Uint& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z || this->w != other_vector.w);
		}
		[[nodiscard]] constexpr Vector3Uint Xyz() const noexcept
		{
			return Vector3Uint(x, y, z);
		}
	};

	static_assert(sizeof(Vector4Uint) == 4 * sizeof(std::uint32_t), "Vector4Uint must be exactly 4 uint in size");
	static_assert(alignof(Vector4Uint) == alignof(std::uint32_t), "Vector4Uint must not be aligned more strictly than a uint");
	static_assert(std::is_standard_layout_v<Vector4Uint>, "Vector4Uint must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4Uint>, "Vector4Uint must be trivially_copyable");
	static_assert(offsetof(Vector4Uint, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4Uint, y) == sizeof(std::uint32_t), "y must follow x");
	static_assert(offsetof(Vector4Uint, z) == 2 * sizeof(std::uint32_t), "z must follow y");
	static_assert(offsetof(Vector4Uint, w) == 3 * sizeof(std::uint32_t), "w must follow z");
}