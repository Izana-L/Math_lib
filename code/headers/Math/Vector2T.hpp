#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <optional>
#include <Math/Scalar.hpp>

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

		[[nodiscard]] constexpr Vector2T operator-() const noexcept requires (!std::unsigned_integral<Type>)
		{return Vector2T(-x, -y);}
		constexpr Vector2T& operator+=(const Vector2T& other_vector) noexcept
		{
    		x+=other_vector.x;
    		y+=other_vector.y;
    		return *this;
		}
		constexpr Vector2T& operator-=(const Vector2T& other_vector) noexcept
		{
    		x-=other_vector.x;
    		y-=other_vector.y;
    		return *this;
		}
		constexpr Vector2T& operator*=( Type scalar) noexcept
		{
    		x*=scalar;
    		y*=scalar;
    		return *this;
		}
		constexpr Vector2T& operator/=( Type scalar) noexcept
		{
        	x/=scalar;
        	y/=scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Vector2T operator+( Vector2T vector_a, const Vector2T& vector_b) noexcept
		{ return vector_a+=vector_b; }
		[[nodiscard]] friend constexpr Vector2T operator-(Vector2T vector_a, const Vector2T& vector_b) noexcept
		{ return  vector_a-=vector_b; }
		[[nodiscard]] friend constexpr Vector2T operator*(Vector2T vector, Type scalar) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator*(Type scalar, Vector2T vector) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator/(Vector2T vector, Type scalar) noexcept
		{ return  vector/=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator*(const Vector2T& vector_a, const Vector2T& vector_b) noexcept
		{ return Vector2T(vector_a.x*vector_b.x,vector_a.y*vector_b.y); }
		[[nodiscard]] friend constexpr Vector2T operator/(const Vector2T& vector_a, const Vector2T& vector_b) noexcept
		{ return Vector2T(vector_a.x/vector_b.x,vector_a.y/vector_b.y); }



	};

	template <typename Type>
	[[nodiscard]] constexpr Type Dot(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return vector_a.x * vector_b.x + vector_a.y * vector_b.y;
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Length_squared(const Vector2T<Type> vector) noexcept
	{
		return Dot(vector, vector);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Length(const Vector2T<Type> vector) noexcept
	{
		return std::sqrt(Length_squared(vector));
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Distance_squared(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Length_squared(vector_a - vector_b);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Distance(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Length(vector_a - vector_b);
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Normalize(const Vector2T<Type> vector) noexcept
	{
		constexpr Type epsilon = std::numeric_limits<Type>::epsilon();
		constexpr Type lower_limit = std::numeric_limits<Type>::min() / epsilon;
		constexpr Type upper_limit = std::numeric_limits<Type>::max() * epsilon;

		const Type length_squared = Length_squared(vector);
		if (length_squared >= lower_limit && length_squared <= upper_limit)
		{
			return vector / std::sqrt(length_squared);
		}

		const Type largest = std::fmax(std::fabs(vector.x), std::fabs(vector.y));
		const Vector2T<Type> scaled = vector / largest;
		return scaled / Length(scaled);
	}


	template <std::floating_point Type>
	[[nodiscard]] std::optional<Vector2T<Type>> Try_normalize(const Vector2T<Type> vector) noexcept
	{
		const Vector2T<Type> result = Normalize(vector);
		if (std::isfinite(result.x) && std::isfinite(result.y))
		{
			return result;
		}
		return std::nullopt;
	}
	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Normalize_or(const Vector2T<Type> vector, const Vector2T<Type> fallback) noexcept
	{
		return Try_normalize(vector).value_or(fallback);
	}
	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalized(const Vector2T<Type> vector) noexcept
	{
		return Equals(Length_squared(vector), Type(1));
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalized(const Vector2T<Type> vector, const Type tolerance) noexcept
	{
		return Equals(Length_squared(vector), Type(1), tolerance, Type(0));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Min(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return  Vector2T<Type>(vector_a.x < vector_b.x ? vector_a.x : vector_b.x,vector_a.y < vector_b.y ? vector_a.y : vector_b.y);
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Max(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Vector2T<Type>(vector_a.x > vector_b.x ? vector_a.x : vector_b.x,vector_a.y > vector_b.y ? vector_a.y : vector_b.y);
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const Vector2T<Type> lower, const Vector2T<Type> upper) noexcept
	{
		return Vector2T<Type>(std::clamp(vector.x, lower.x, upper.x), std::clamp(vector.y, lower.y, upper.y));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const std::type_identity_t<Type> lower, const std::type_identity_t<Type> upper) noexcept
	{
		return Vector2T<Type>(std::clamp(vector.x, lower, upper), std::clamp(vector.y, lower, upper));
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Min_component(const Vector2T<Type> vector) noexcept
	{
		return vector.x < vector.y ? vector.x : vector.y;
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Max_component(const Vector2T<Type> vector) noexcept
	{
		return vector.x > vector.y ? vector.x : vector.y;
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const Vector2T<Type> lower, const Vector2T<Type> upper) noexcept
	{
		return Vector2T<Type>(std::clamp(vector.x, lower.x, upper.x), std::clamp(vector.y, lower.y, upper.y));
	}


	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const std::type_identity_t<Type> lower, const std::type_identity_t<Type> upper) noexcept
	{
		return Vector2T<Type>(std::clamp(vector.x, lower, upper), std::clamp(vector.y, lower, upper));
	}


	template <typename Type>
	requires (!std::unsigned_integral<Type>)
	[[nodiscard]] constexpr Vector2T<Type> Abs(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(detail::Abs_of(vector.x), detail::Abs_of(vector.y));
	}


	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Sign(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(detail::Sign_of(vector.x), detail::Sign_of(vector.y));
	}


	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Saturate(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(Saturate(vector.x), Saturate(vector.y));
	}


	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Floor(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(std::floor(vector.x), std::floor(vector.y));
	}


	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Ceil(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(std::ceil(vector.x), std::ceil(vector.y));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Fract(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(Repeat(vector.x, Type(1)), Repeat(vector.y, Type(1)));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Lerp(const Vector2T<Type> from, const Vector2T<Type> to, const std::type_identity_t<Type> factor) noexcept
	{
		return Vector2T<Type>(Lerp(from.x, to.x, factor), Lerp(from.y, to.y, factor));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Lerp(const Vector2T<Type> from, const Vector2T<Type> to, const Vector2T<Type> factor) noexcept
	{
		return Vector2T<Type>(Lerp(from.x, to.x, factor.x), Lerp(from.y, to.y, factor.y));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Smoothstep(const std::type_identity_t<Type> lower_edge, const std::type_identity_t<Type> upper_edge, const Vector2T<Type> value) noexcept
	{
		return Vector2T<Type>(Smoothstep(lower_edge, upper_edge, value.x), Smoothstep(lower_edge, upper_edge, value.y));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Smoothstep(const Vector2T<Type> lower_edge, const Vector2T<Type> upper_edge, const Vector2T<Type> value) noexcept
	{
		return Vector2T<Type>(Smoothstep(lower_edge.x, upper_edge.x, value.x), Smoothstep(lower_edge.y, upper_edge.y, value.y));
	}



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
