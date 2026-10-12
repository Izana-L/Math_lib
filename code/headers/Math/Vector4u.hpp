#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <Math/Vector3u.hpp>
namespace Math
{

	struct Vector4u
	{
		std::uint32_t x, y, z, w;

		constexpr Vector4u() noexcept : x(0), y(0), z(0), w(0) {}

		constexpr Vector4u(const std::uint32_t _x, const std::uint32_t _y, const std::uint32_t _z, const std::uint32_t _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		constexpr Vector4u(Vector3u _vector3, const std::uint32_t _w) noexcept : x(_vector3.x), y(_vector3.y), z(_vector3.z), w(_w) {}
		constexpr explicit Vector4u(const std::uint32_t value) noexcept : x(value), y(value), z(value), w(value) {}

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

		[[nodiscard]] constexpr bool operator==(const Vector4u& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z && this->w == other_vector.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector4u& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z || this->w != other_vector.w);
		}
		[[nodiscard]] constexpr Vector3u Xyz() const noexcept
		{
			return Vector3u(x, y, z);
		}
	};

	static_assert(sizeof(Vector4u) == 4 * sizeof(std::uint32_t), "Vector4u must be exactly 4 uint in size");
	static_assert(alignof(Vector4u) == alignof(std::uint32_t), "Vector4u must not be aligned more strictly than a uint");
	static_assert(std::is_standard_layout_v<Vector4u>, "Vector4u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4u>, "Vector4u must be trivially_copyable");
	static_assert(offsetof(Vector4u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4u, y) == sizeof(std::uint32_t), "y must follow x");
	static_assert(offsetof(Vector4u, z) == 2 * sizeof(std::uint32_t), "z must follow y");
	static_assert(offsetof(Vector4u, w) == 3 * sizeof(std::uint32_t), "w must follow z");
}