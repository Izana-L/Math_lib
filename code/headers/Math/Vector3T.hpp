#pragma once 

#include <cstddef>
#include <type_traits>
#include <Math/Vector2T.hpp>

namespace Math
{
	
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct Vector3T
	{
		using Type = T;
		Type x, y, z;

		constexpr Vector3T() noexcept : x(),y(), z(){}
		constexpr Vector3T(const Type _x, const Type _y, const Type _z ) noexcept : x(_x), y(_y), z(_z) {}
		constexpr Vector3T(Vector2T<Type> _vector2, const Type _z) noexcept : x(_vector2.x), y(_vector2.y), z(_z) {}
		constexpr explicit Vector3T(const Type value) noexcept : x(value), y(value), z(value) {}

		[[nodiscard]] constexpr Type& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr const Type& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr Type* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const Type* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector3T<Type>& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector3T<Type>& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z);
		}
		[[nodiscard]] constexpr Vector2T<Type> Xy() const noexcept
		{
			return Vector2T<Type>(x,y);
		}
	};
	using Vector3 = Vector3T<float>;
	using Vector3i = Vector3T<std::int32_t>;
	using Vector3u = Vector3T<std::uint32_t>;

	static_assert(sizeof(Vector3) == 3 * sizeof(float), "Vector3 must be exactly 3 float in size");
	static_assert(alignof(Vector3) == alignof(float), "Vector3 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector3>, "Vector3 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3>, "Vector3 must be trivially_copyable");
	static_assert(offsetof(Vector3, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Vector3, z) == 2 * sizeof(float), "z must follow y");

	static_assert(sizeof(Vector3i) == 3 * sizeof(std::int32_t), "Vector3i must be exactly 3 std::int32_t in size");
	static_assert(alignof(Vector3i) == alignof(std::int32_t), "Vector3i must not be aligned more strictly than a std::int32_t");
	static_assert(std::is_standard_layout_v<Vector3i>, "Vector3i must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3i>, "Vector3i must be trivially_copyable");
	static_assert(offsetof(Vector3i, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3i, y) == sizeof(std::int32_t), "y must follow x");
	static_assert(offsetof(Vector3i, z) == 2 * sizeof(std::int32_t), "z must follow y");

	static_assert(sizeof(Vector3u) == 3 * sizeof(std::uint32_t), "Vector3u must be exactly 3 std::uint32_t in size");
	static_assert(alignof(Vector3u) == alignof(std::uint32_t), "Vector3u must not be aligned more strictly than a std::uint32_t");
	static_assert(std::is_standard_layout_v<Vector3u>, "Vector3u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector3u>, "Vector3u must be trivially_copyable");
	static_assert(offsetof(Vector3u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector3u, y) == sizeof(std::uint32_t), "y must follow x");
	static_assert(offsetof(Vector3u, z) == 2 * sizeof(std::uint32_t), "z must follow y");
}