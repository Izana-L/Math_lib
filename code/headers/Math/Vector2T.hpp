#pragma once 

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace Math
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct Vector2T
	{
		using Type = T;
		Type x, y;

		constexpr Vector2T() noexcept : x(), y(){}

		constexpr Vector2T(const Type _x, const Type _y) noexcept : x(_x), y(_y) {}
		constexpr explicit Vector2T(const Type value) noexcept : x(value), y(value) {}

		[[nodiscard]] constexpr Type& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr const Type& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr Type* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const Type* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector2T<Type>& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y );
		}
		[[nodiscard]] constexpr bool operator!=(const Vector2T<Type>& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y );
		}

	};
	using Vector2 = Vector2T<float>;
	using Vector2i = Vector2T<std::int32_t>;
	using Vector2u = Vector2T<std::uint32_t>;

	static_assert(sizeof(Vector2) == 2 * sizeof(float), "Vector2 must be exactly 2 floats in size");
	static_assert(alignof(Vector2) == alignof(float), "Vector2 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector2>, "Vector2 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2>, "Vector2 must be trivially_copyable");
	static_assert(offsetof(Vector2, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2, y) == sizeof(float), "y must follow x");

	static_assert(sizeof(Vector2i) == 2 * sizeof(std::int32_t), "Vector2i must be exactly 2 int32_t in size");
	static_assert(alignof(Vector2i) == alignof(std::int32_t), "Vector2i must not be aligned more strictly than an int32_t");
	static_assert(std::is_standard_layout_v<Vector2i>, "Vector2i must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2i>, "Vector2i must be trivially_copyable");
	static_assert(offsetof(Vector2i, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2i, y) == sizeof(std::int32_t), "y must follow x");

	static_assert(sizeof(Vector2u) == 2 * sizeof(std::uint32_t), "Vector2u must be exactly 2 uint32_t in size");
	static_assert(alignof(Vector2u) == alignof(std::uint32_t), "Vector2u must not be aligned more strictly than a uint32_t");
	static_assert(std::is_standard_layout_v<Vector2u>, "Vector2u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector2u>, "Vector2u must be trivially_copyable");
	static_assert(offsetof(Vector2u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector2u, y) == sizeof(std::uint32_t), "y must follow x");

}