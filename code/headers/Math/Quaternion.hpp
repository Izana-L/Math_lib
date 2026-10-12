#pragma once 

#include <cstddef>
#include <type_traits>


namespace Math
{

	struct Quaternion
	{
		float x, y, z, w;

		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Quaternion(const float _x, const float _y, const float _z, const float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}

		[[nodiscard]] constexpr float& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr const float& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr static Quaternion Identity() noexcept { return Quaternion(); }

		[[nodiscard]] constexpr bool operator==(const Quaternion& other_quaternion) const noexcept
		{
			return(this->x == other_quaternion.x && this->y == other_quaternion.y && this->z == other_quaternion.z && this->w == other_quaternion.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Quaternion& other_quaternion) const noexcept
		{
			return(this->x != other_quaternion.x || this->y != other_quaternion.y || this->z != other_quaternion.z || this->w != other_quaternion.w);
		}

	};
	static_assert(sizeof(Quaternion) == 4 * sizeof(float), "Quaternion must be exactly 4 floats in size");
	static_assert(alignof(Quaternion) == alignof(float), "Quaternion must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Quaternion>, "Quaternion must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Quaternion>, "Quaternio  must be trivially_copyable");
	static_assert(offsetof(Quaternion, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Quaternion, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Quaternion, z) == 2 * sizeof(float), "z must follow y");
	static_assert(offsetof(Quaternion, w) == 3 * sizeof(float), "w must follow z");
}