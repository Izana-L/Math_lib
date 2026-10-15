#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <optional>
#include <Math/Scalar.hpp>
#include <Math/Vector3T.hpp>

namespace Math
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct Vector4T
	{
		using Type = T;
		Type x, y, z, w;

		constexpr Vector4T() noexcept : x(), y(), z(), w() {}

		constexpr Vector4T(const Type _x, const Type _y, const Type _z, const Type _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		constexpr Vector4T(Vector3T<Type> _vector3, const Type _w) noexcept : x(_vector3.x), y(_vector3.y), z(_vector3.z), w(_w) {}
		constexpr explicit Vector4T(const Type value) noexcept : x(value), y(value), z(value), w(value) {}

		[[nodiscard]] constexpr Type& operator[](std::size_t index) noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z :w;
		}
		[[nodiscard]] constexpr const Type& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
		}
		[[nodiscard]] constexpr Type* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const Type* Data() const noexcept { return &x; }

		[[nodiscard]] constexpr bool operator==(const Vector4T<Type>& other_vector) const noexcept
		{
			return(this->x == other_vector.x && this->y == other_vector.y && this->z == other_vector.z && this->w == other_vector.w);
		}
		[[nodiscard]] constexpr bool operator!=(const Vector4T<Type>& other_vector) const noexcept
		{
			return(this->x != other_vector.x || this->y != other_vector.y || this->z != other_vector.z || this->w != other_vector.w);
		}
		[[nodiscard]] constexpr Vector3T<Type> Xyz() const noexcept
		{
			return Vector3T<Type>(x, y, z);
		}

		[[nodiscard]] constexpr Vector4T operator-() const noexcept requires (!std::unsigned_integral<Type>)
		{return Vector4T(-x, -y,-z,-w);}
		constexpr Vector4T& operator+=(const Vector4T& other_vector) noexcept
		{
    		x+=other_vector.x;
    		y+=other_vector.y;
            z+=other_vector.z;
            w+=other_vector.w;
    		return *this;
		}
		constexpr Vector4T& operator-=(const Vector4T& other_vector) noexcept
		{
    		x-=other_vector.x;
    		y-=other_vector.y;
            z-=other_vector.z;
            w-=other_vector.w;
    		return *this;
		}
		constexpr Vector4T& operator*=( Type scalar) noexcept
		{
    		x*=scalar;
    		y*=scalar;
            z*=scalar;
            w*=scalar;
    		return *this;
		}
		constexpr Vector4T& operator/=( Type scalar) noexcept
		{
        	x/=scalar;
        	y/=scalar;
            z/=scalar;
            w/=scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Vector4T operator+( Vector4T vector_a, const Vector4T& vector_b) noexcept
		{ return vector_a+=vector_b; }
		[[nodiscard]] friend constexpr Vector4T operator-(Vector4T vector_a, const Vector4T& vector_b) noexcept
		{ return  vector_a-=vector_b; }
		[[nodiscard]] friend constexpr Vector4T operator*(Vector4T vector, Type scalar) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector4T operator*(Type scalar, Vector4T vector) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector4T operator/(Vector4T vector, Type scalar) noexcept
		{ return  vector/=scalar; }
		[[nodiscard]] friend constexpr Vector4T operator*(const Vector4T& vector_a, const Vector4T& vector_b) noexcept
		{ return Vector4T(vector_a.x*vector_b.x, vector_a.y*vector_b.y, vector_a.z*vector_b.z, vector_a.w*vector_b.w); }
		[[nodiscard]] friend constexpr Vector4T operator/(const Vector4T& vector_a, const Vector4T& vector_b) noexcept
		{ return Vector4T(vector_a.x/vector_b.x, vector_a.y/vector_b.y, vector_a.z/vector_b.z, vector_a.w/vector_b.w); }
	};

	template <typename Type>
	[[nodiscard]] constexpr Type Dot(const Vector4T<Type> vector_a, const Vector4T<Type> vector_b) noexcept
	{
		return vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z + vector_a.w * vector_b.w;
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Length_squared(const Vector4T<Type> vector) noexcept
	{
		return Dot(vector, vector);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Length(const Vector4T<Type> vector) noexcept
	{
		return std::sqrt(Length_squared(vector));
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Distance_squared(const Vector4T<Type> vector_a, const Vector4T<Type> vector_b) noexcept
	{
		return Length_squared(vector_a - vector_b);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Distance(const Vector4T<Type> vector_a, const Vector4T<Type> vector_b) noexcept
	{
		return Length(vector_a - vector_b);
	}
	template <std::floating_point Type>
	[[nodiscard]] Vector4T<Type> Normalize(const Vector4T<Type> vector) noexcept
	{
		constexpr Type epsilon = std::numeric_limits<Type>::epsilon();
		constexpr Type lower_limit = std::numeric_limits<Type>::min() / epsilon;
		constexpr Type upper_limit = std::numeric_limits<Type>::max() * epsilon;

		const Type length_squared = Length_squared(vector);
		if (length_squared >= lower_limit && length_squared <= upper_limit)
		{
			return vector / std::sqrt(length_squared);
		}

		const Type largest = std::fmax(std::fmax(std::fabs(vector.x), std::fabs(vector.y)), std::fmax(std::fabs(vector.z), std::fabs(vector.w)));
		const Vector4T<Type> scaled = vector / largest;
		return scaled / Length(scaled);
	}


	template <std::floating_point Type>
	[[nodiscard]] std::optional<Vector4T<Type>> Try_normalize(const Vector4T<Type> vector) noexcept
	{
		const Vector4T<Type> result = Normalize(vector);
		if (std::isfinite(result.x) && std::isfinite(result.y) && std::isfinite(result.z) && std::isfinite(result.w))
		{
			return result;
		}
		return std::nullopt;
	}
	template <std::floating_point Type>
	[[nodiscard]] Vector4T<Type> Normalize_or(const Vector4T<Type> vector, const Vector4T<Type> fallback) noexcept
	{
		return Try_normalize(vector).value_or(fallback);
	}
	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalized(const Vector4T<Type> vector) noexcept
	{
		return Equals(Length_squared(vector), Type(1));
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalized(const Vector4T<Type> vector, const Type tolerance) noexcept
	{
		return Equals(Length_squared(vector), Type(1), tolerance, Type(0));
	}
	using Vector4 = Vector4T<float>;
	using Vector4i = Vector4T<std::int32_t>;
	using Vector4u = Vector4T<std::uint32_t>;

	static_assert(sizeof(Vector4) == 4 * sizeof(float), "Vector4 must be exactly 4 float in size");
	static_assert(alignof(Vector4) == alignof(float), "Vector4 must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Vector4>, "Vector4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4>, "Vector4 must be trivially_copyable");
	static_assert(offsetof(Vector4, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Vector4, z) == 2 * sizeof(float), "z must follow y");
	static_assert(offsetof(Vector4, w) == 3 * sizeof(float), "w must follow z");

	static_assert(sizeof(Vector4i) == 4 * sizeof(std::int32_t), "Vector4i must be exactly 4 std::int32_t in size");
	static_assert(alignof(Vector4i) == alignof(std::int32_t), "Vector4i must not be aligned more strictly than a std::int32_t");
	static_assert(std::is_standard_layout_v<Vector4i>, "Vector4i must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4i>, "Vector4i must be trivially_copyable");
	static_assert(offsetof(Vector4i, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4i, y) == sizeof(std::int32_t), "y must follow x");
	static_assert(offsetof(Vector4i, z) == 2 * sizeof(std::int32_t), "z must follow y");
	static_assert(offsetof(Vector4i, w) == 3 * sizeof(std::int32_t), "w must follow z");

	static_assert(sizeof(Vector4u) == 4 * sizeof(std::uint32_t), "Vector4u must be exactly 4 std::uint32_t in size");
	static_assert(alignof(Vector4u) == alignof(std::uint32_t), "Vector4u must not be aligned more strictly than a std::uint32_t");
	static_assert(std::is_standard_layout_v<Vector4u>, "Vector4u must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Vector4u>, "Vector4u must be trivially_copyable");
	static_assert(offsetof(Vector4u, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Vector4u, y) == sizeof(std::uint32_t), "y must follow x");
	static_assert(offsetof(Vector4u, z) == 2 * sizeof(std::uint32_t), "z must follow y");
	static_assert(offsetof(Vector4u, w) == 3 * sizeof(std::uint32_t), "w must follow z");
}
