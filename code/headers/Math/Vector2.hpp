#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <optional>
#include <cassert>
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
		    assert(index < 2 && "operator[]: index must be less than 2");
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr const Type& operator[](std::size_t index) const noexcept
		{
		    assert(index <= 2 && "operator[]: index must be less than 2");
			return index == 0 ? x : y;
		}
		[[nodiscard]] constexpr Type* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const Type* Data() const noexcept { return &x; }

		[[nodiscard]] friend constexpr bool operator==(const Vector2T<Type>& vector, const Vector2T<Type>& other_vector)  noexcept = default;

		[[nodiscard]] constexpr Vector2T operator-() const noexcept requires (!std::unsigned_integral<Type>)
		{return Vector2T(-x, -y);}
		constexpr Vector2T& operator+=(const Vector2T other_vector) noexcept
		{
    		x+=other_vector.x;
    		y+=other_vector.y;
    		return *this;
		}
		constexpr Vector2T& operator-=(const Vector2T other_vector) noexcept
		{
    		x-=other_vector.x;
    		y-=other_vector.y;
    		return *this;
		}
		constexpr Vector2T& operator*=( const Vector2T other_vector) noexcept
		{
		    x*=other_vector.x;
    		y*=other_vector.y;
    		return *this;
		}
		constexpr Vector2T& operator/=( const Vector2T other_vector) noexcept
		{
		    x/=other_vector.x;
    		y/=other_vector.y;
        	return *this;
	    }

		constexpr Vector2T& operator*=( const Type scalar) noexcept
		{
    		x*=scalar;
    		y*=scalar;
    		return *this;
		}
		constexpr Vector2T& operator/=( const Type scalar) noexcept
		{
        	x/=scalar;
        	y/=scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Vector2T operator+( Vector2T vector_a, const Vector2T vector_b) noexcept
		{ return vector_a+=vector_b; }
		[[nodiscard]] friend constexpr Vector2T operator-(Vector2T vector_a, const Vector2T vector_b) noexcept
		{ return  vector_a-=vector_b; }
		[[nodiscard]] friend constexpr Vector2T operator*(Vector2T vector, Type scalar) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator*(Type scalar, Vector2T vector) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator/(Vector2T vector, Type scalar) noexcept
		{ return  vector/=scalar; }
		[[nodiscard]] friend constexpr Vector2T operator*(const Vector2T vector_a, const Vector2T vector_b) noexcept
		{ return vector_a*=vector_b; }
		[[nodiscard]] friend constexpr Vector2T operator/(const Vector2T vector_a, const Vector2T vector_b) noexcept
		{ return vector_a/=vector_b; }



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
	[[nodiscard]] bool Is_finite(const Vector2T<Type> vector) noexcept
	{
		return std::isfinite(vector.x) && std::isfinite(vector.y);
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalizable(const Vector2T<Type> vector) noexcept
	{
		return Is_finite(vector) && (vector.x != 0 ||vector.y != 0 );
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Normalize(const Vector2T<Type> vector) noexcept
	{
	    assert(Is_normalizable(vector) && "Normalize: cannot normalize a zero vector or infinites values");
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
        if (!Is_normalizable(vector))
        {
        	return std::nullopt;
        }
		return Normalize(vector);
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
	[[nodiscard]] constexpr bool Equals(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Equals(vector_a.x, vector_b.x) && Equals(vector_a.y, vector_b.y);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr bool Equals(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b, const std::type_identity_t<Type> absolute_tolerance, const std::type_identity_t<Type> relative_tolerance) noexcept
	{
		return Equals(vector_a.x, vector_b.x, absolute_tolerance, relative_tolerance) && Equals(vector_a.y, vector_b.y, absolute_tolerance, relative_tolerance);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Project(const Vector2T<Type> vector, const Vector2T<Type> onto) noexcept
	{
		const Type onto_length_squared = Length_squared(onto);
		assert(onto_length_squared > Type(0) && "Project: 'onto' cannot be a zero vector");
		return onto * (Dot(vector, onto) / onto_length_squared);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Reject(const Vector2T<Type> vector, const Vector2T<Type> onto) noexcept
	{
		return vector - Project(vector, onto);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector2T<Type> Reflect(const Vector2T<Type> incident, const Vector2T<Type> normal) noexcept
	{
		assert(Equals(Length_squared(normal), Type(1)) && "Reflect: 'normal' must be normalized");
		return incident - normal * (Type(2) * Dot(incident, normal));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Refract(const Vector2T<Type> incident, const Vector2T<Type> normal, const std::type_identity_t<Type> refraction_ratio) noexcept
	{
		assert(Is_normalized(incident) && Is_normalized(normal) && "Refract: 'incident' and 'normal' must be normalized");
		const Type normal_dot_incident = Dot(normal, incident);
		const Type cos_refracted_squared = Type(1) - refraction_ratio * refraction_ratio * (Type(1) - normal_dot_incident * normal_dot_incident);
		if (cos_refracted_squared < Type(0))
		{
			return Vector2T<Type>();
		}
		return incident * refraction_ratio - normal * (refraction_ratio * normal_dot_incident + std::sqrt(cos_refracted_squared));
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalized(const Vector2T<Type> vector, const Type tolerance) noexcept
	{
		return Equals(Length_squared(vector), Type(1), tolerance, Type(0));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Min(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Vector2T<Type>(std::min(vector_a.x, vector_b.x), std::min(vector_a.y, vector_b.y));
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Max(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return Vector2T<Type>(std::max(vector_a.x, vector_b.x), std::max(vector_a.y, vector_b.y));
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const Vector2T<Type> lower, const Vector2T<Type> upper) noexcept
	{
	    assert(lower.x <= upper.x && lower.y <= upper.y && "Clamp: 'lower' must be less than or equal to 'upper' in every component");
		return Vector2T<Type>(std::clamp(vector.x, lower.x, upper.x), std::clamp(vector.y, lower.y, upper.y));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector2T<Type> Clamp(const Vector2T<Type> vector, const std::type_identity_t<Type> lower, const std::type_identity_t<Type> upper) noexcept
	{
	    assert( lower <= upper && "Clamp: 'lower' must be less than or equal to 'upper' in every component");
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

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Fract(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(Fract(vector.x), Fract(vector.y));
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

	template <typename Type>
	requires (!std::unsigned_integral<Type>)
	[[nodiscard]] constexpr Type Cross(const Vector2T<Type> vector_a, const Vector2T<Type> vector_b) noexcept
	{
		return vector_a.x * vector_b.y - vector_a.y * vector_b.x;
	}

	template <typename Type>
	requires (!std::unsigned_integral<Type>)
	[[nodiscard]] constexpr Vector2T<Type> Perpendicular(const Vector2T<Type> vector) noexcept
	{
		return Vector2T<Type>(-vector.y, vector.x);
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector2T<Type> Rotate(const Vector2T<Type> vector, const Radians angle) noexcept
	{
		const Type cosine = std::cos(static_cast<Type>(angle.value));
		const Type sine = std::sin(static_cast<Type>(angle.value));
		return Vector2T<Type>(vector.x * cosine - vector.y * sine, vector.x * sine + vector.y * cosine);
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
