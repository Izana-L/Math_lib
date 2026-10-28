#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <optional>
#include <cassert>
#include <Math/Scalar.hpp>
#include <Math/Vector2.hpp>

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
		    assert(index < 3 && "operator[]: index must be less than 3");
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr const Type& operator[](std::size_t index) const noexcept
		{
		    assert(index < 3 && "operator[]: index must be less than 3");
			return index == 0 ? x : index == 1 ? y : z;
		}
		[[nodiscard]] constexpr Type* Data() noexcept { return &x; }
		[[nodiscard]] constexpr const Type* Data() const noexcept { return &x; }

		[[nodiscard]] friend constexpr bool operator==(const Vector3T<Type>& vector, const Vector3T<Type>& other_vector)  noexcept = default;
		[[nodiscard]] constexpr Vector2T<Type> Xy() const noexcept
		{
			return Vector2T<Type>(x,y);
		}

		[[nodiscard]] constexpr Vector3T operator-() const noexcept requires (!std::unsigned_integral<Type>)
		{return Vector3T(-x, -y,-z);}
		constexpr Vector3T& operator+=(const Vector3T other_vector) noexcept
		{
    		x+=other_vector.x;
    		y+=other_vector.y;
            z+=other_vector.z;
    		return *this;
		}
		constexpr Vector3T& operator-=(const Vector3T other_vector) noexcept
		{
    		x-=other_vector.x;
    		y-=other_vector.y;
            z-=other_vector.z;
    		return *this;
		}
		constexpr Vector3T& operator*=(const Vector3T other_vector) noexcept
		{
    		x*=other_vector.x;
    		y*=other_vector.y;
            z*=other_vector.z;
    		return *this;
		}
		constexpr Vector3T& operator/=(const Vector3T other_vector) noexcept
		{
    		x/=other_vector.x;
    		y/=other_vector.y;
            z/=other_vector.z;
    		return *this;
		}
		constexpr Vector3T& operator+=(const Type scalar) noexcept
		{
    		x+=scalar;
    		y+=scalar;
            z+=scalar;
    		return *this;
		}
		constexpr Vector3T& operator-=(const Type scalar) noexcept
		{
        	x-=scalar;
        	y-=scalar;
            z-=scalar;
        	return *this;
	    }
		constexpr Vector3T& operator*=(const Type scalar) noexcept
		{
    		x*=scalar;
    		y*=scalar;
            z*=scalar;
    		return *this;
		}
		constexpr Vector3T& operator/=(const Type scalar) noexcept
		{
        	x/=scalar;
        	y/=scalar;
            z/=scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Vector3T operator+( Vector3T vector_a, const Vector3T vector_b) noexcept
		{ return vector_a+=vector_b; }
		[[nodiscard]] friend constexpr Vector3T operator-(Vector3T vector_a, const Vector3T vector_b) noexcept
		{ return  vector_a-=vector_b; }
		[[nodiscard]] friend constexpr Vector3T operator*( Vector3T vector_a, const Vector3T vector_b) noexcept
		{ return vector_a*=vector_b; }
		[[nodiscard]] friend constexpr Vector3T operator/( Vector3T vector_a, const Vector3T vector_b) noexcept
		{ return vector_a/=vector_b; }
		[[nodiscard]] friend constexpr Vector3T operator+(Vector3T vector,const Type scalar) noexcept
		{ return  vector+=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator+(const Type scalar, Vector3T vector) noexcept
		{ return  vector+=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator-(Vector3T vector,const Type scalar) noexcept
		{ return  vector-=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator-(const Type scalar, Vector3T vector) noexcept
		{ return Vector3T(scalar-vector.x, scalar-vector.y, scalar-vector.z);}
		[[nodiscard]] friend constexpr Vector3T operator*(Vector3T vector, const Type scalar) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator*(const Type scalar, Vector3T vector) noexcept
		{ return  vector*=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator/(Vector3T vector,const Type scalar) noexcept
		{ return  vector/=scalar; }
		[[nodiscard]] friend constexpr Vector3T operator/(const Type scalar, Vector3T vector) noexcept
		{ return Vector3T(scalar/vector.x,scalar/vector.y,scalar/vector.z); }

	};
	template <typename Type>
	[[nodiscard]] constexpr Type Dot(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z;
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Length_squared(const Vector3T<Type> vector) noexcept
	{
		return Dot(vector, vector);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Length(const Vector3T<Type> vector) noexcept
	{
		return std::sqrt(Length_squared(vector));
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Distance_squared(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return Length_squared(vector_a - vector_b);
	}
	template <std::floating_point Type>
	[[nodiscard]] Type Distance(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return Length(vector_a - vector_b);
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_finite(const Vector3T<Type> vector) noexcept
	{
		return std::isfinite(vector.x) && std::isfinite(vector.y) && std::isfinite(vector.z);
	}

	template <std::floating_point Type>
	[[nodiscard]] bool Is_normalizable(const Vector3T<Type> vector) noexcept
	{
		return Is_finite(vector) && (vector.x != 0 ||vector.y != 0 ||vector.z != 0 );
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Normalize(const Vector3T<Type> vector) noexcept
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

		const Type largest = std::fmax(std::fmax(std::fabs(vector.x), std::fabs(vector.y)), std::fabs(vector.z));
		const Vector3T<Type> scaled = vector / largest;
		return scaled / Length(scaled);
	}


	template <std::floating_point Type>
	[[nodiscard]] std::optional<Vector3T<Type>> Try_normalize(const Vector3T<Type> vector) noexcept
	{
	    if (!Is_normalizable(vector))
	    {
			return std::nullopt;
		}
		return Normalize(vector);
	}
	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Normalize_or(const Vector3T<Type> vector, const Vector3T<Type> fallback) noexcept
	{
		return Try_normalize(vector).value_or(fallback);
	}
	template <std::floating_point Type>
	[[nodiscard]] constexpr bool Is_normalized(const Vector3T<Type> vector) noexcept
	{
		return Equals(Length_squared(vector), Type(1));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr bool Equals(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return Equals(vector_a.x, vector_b.x) && Equals(vector_a.y, vector_b.y) && Equals(vector_a.z, vector_b.z);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr bool Equals(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b, const std::type_identity_t<Type> absolute_tolerance, const std::type_identity_t<Type> relative_tolerance) noexcept
	{
		return Equals(vector_a.x, vector_b.x, absolute_tolerance, relative_tolerance) && Equals(vector_a.y, vector_b.y, absolute_tolerance, relative_tolerance) &&
			   Equals(vector_a.z, vector_b.z, absolute_tolerance, relative_tolerance);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Project(const Vector3T<Type> vector, const Vector3T<Type> onto) noexcept
	{
		const Type onto_length_squared = Length_squared(onto);
		assert(onto_length_squared > Type(0) && "Project: 'onto' cannot be a zero vector");
		return onto * (Dot(vector, onto) / onto_length_squared);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Reject(const Vector3T<Type> vector, const Vector3T<Type> onto) noexcept
	{
		return vector - Project(vector, onto);
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Reflect(const Vector3T<Type> incident, const Vector3T<Type> normal) noexcept
	{
		assert(Is_normalized(normal) && "Reflect: 'normal' must be normalized");
		return incident - normal * (Type(2) * Dot(incident, normal));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Refract(const Vector3T<Type> incident, const Vector3T<Type> normal, const std::type_identity_t<Type> refraction_ratio) noexcept
	{
		assert(Is_normalized(incident) && Is_normalized(normal) && "Refract: 'incident' and 'normal' must be normalized");
		const Type normal_dot_incident = Dot(normal, incident);
		const Type cos_refracted_squared = Type(1) - refraction_ratio * refraction_ratio * (Type(1) - normal_dot_incident * normal_dot_incident);
		if (cos_refracted_squared < Type(0))
		{
			return Vector3T<Type>();
		}
		return incident * refraction_ratio - normal * (refraction_ratio * normal_dot_incident + std::sqrt(cos_refracted_squared));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr bool Is_normalized(const Vector3T<Type> vector, const Type tolerance) noexcept
	{
		return Equals(Length_squared(vector), Type(1), tolerance, Type(0));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector3T<Type> Min(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return Vector3T<Type>(std::min(vector_a.x, vector_b.x), std::min(vector_a.y, vector_b.y), std::min(vector_a.z, vector_b.z));
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector3T<Type> Max(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
	    return Vector3T<Type>(std::max(vector_a.x, vector_b.x), std::max(vector_a.y, vector_b.y), std::max(vector_a.z, vector_b.z));
	}
	template <typename Type>
	[[nodiscard]] constexpr Vector3T<Type> Clamp(const Vector3T<Type> vector, const Vector3T<Type> lower, const Vector3T<Type> upper) noexcept
	{
	    assert(lower.x <= upper.x && lower.y <= upper.y && lower.z <= upper.z && "Clamp: 'lower' must be less than or equal to 'upper' in every component");
		return Vector3T<Type>(std::clamp(vector.x, lower.x, upper.x), std::clamp(vector.y, lower.y, upper.y), std::clamp(vector.z, lower.z, upper.z));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector3T<Type> Clamp(const Vector3T<Type> vector, const std::type_identity_t<Type> lower, const std::type_identity_t<Type> upper) noexcept
	{
	    assert( lower <= upper && "Clamp: 'lower' must be less than or equal to 'upper' in every component");
		return Vector3T<Type>(std::clamp(vector.x, lower, upper), std::clamp(vector.y, lower, upper), std::clamp(vector.z, lower, upper));
	}

	template <typename Type>
	[[nodiscard]] constexpr Type Min_component(const Vector3T<Type> vector) noexcept
	{
		const Type smallest_xy = vector.x < vector.y ? vector.x : vector.y;
		return smallest_xy < vector.z ? smallest_xy : vector.z;
	}
	template <typename Type>
	[[nodiscard]] constexpr Type Max_component(const Vector3T<Type> vector) noexcept
	{
		const Type biggest_xy = vector.x > vector.y ? vector.x : vector.y;
		return biggest_xy > vector.z ? biggest_xy : vector.z;
	}

	template <typename Type>
	requires (!std::unsigned_integral<Type>)
	[[nodiscard]] constexpr Vector3T<Type> Abs(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(detail::Abs_of(vector.x), detail::Abs_of(vector.y), detail::Abs_of(vector.z));
	}

	template <typename Type>
	[[nodiscard]] constexpr Vector3T<Type> Sign(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(detail::Sign_of(vector.x), detail::Sign_of(vector.y), detail::Sign_of(vector.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Saturate(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(Saturate(vector.x), Saturate(vector.y), Saturate(vector.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Floor(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(std::floor(vector.x), std::floor(vector.y), std::floor(vector.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Ceil(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(std::ceil(vector.x), std::ceil(vector.y), std::ceil(vector.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Fract(const Vector3T<Type> vector) noexcept
	{
		return Vector3T<Type>(Fract(vector.x), Fract(vector.y), Fract(vector.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Lerp(const Vector3T<Type> from, const Vector3T<Type> to, const std::type_identity_t<Type> factor) noexcept
	{
		return Vector3T<Type>(Lerp(from.x, to.x, factor), Lerp(from.y, to.y, factor), Lerp(from.z, to.z, factor));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Lerp(const Vector3T<Type> from, const Vector3T<Type> to, const Vector3T<Type> factor) noexcept
	{
		return Vector3T<Type>(Lerp(from.x, to.x, factor.x), Lerp(from.y, to.y, factor.y), Lerp(from.z, to.z, factor.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Smoothstep(const std::type_identity_t<Type> lower_edge, const std::type_identity_t<Type> upper_edge, const Vector3T<Type> value) noexcept
	{
		return Vector3T<Type>(Smoothstep(lower_edge, upper_edge, value.x), Smoothstep(lower_edge, upper_edge, value.y), Smoothstep(lower_edge, upper_edge, value.z));
	}

	template <std::floating_point Type>
	[[nodiscard]] constexpr Vector3T<Type> Smoothstep(const Vector3T<Type> lower_edge, const Vector3T<Type> upper_edge, const Vector3T<Type> value) noexcept
	{
		return Vector3T<Type>(Smoothstep(lower_edge.x, upper_edge.x, value.x), Smoothstep(lower_edge.y, upper_edge.y, value.y), Smoothstep(lower_edge.z, upper_edge.z, value.z));
	}

	template <typename Type>
	requires (!std::unsigned_integral<Type>)
	[[nodiscard]] constexpr Vector3T<Type> Cross(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
		return Vector3T<Type>(vector_a.y * vector_b.z - vector_a.z * vector_b.y, vector_a.z * vector_b.x - vector_a.x * vector_b.z, vector_a.x * vector_b.y - vector_a.y * vector_b.x);
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Any_perpendicular(const Vector3T<Type> vector) noexcept
	{
	    assert(Is_normalizable(vector) && "Any_perpendicular: cannot find a perpendicular of a zero vector or infinites values");
		const Vector3T<Type> perpendicular = detail::Abs_of(vector.x) > detail::Abs_of(vector.z)
                                             ? Vector3T<Type>(-vector.y, vector.x, Type(0))
                                             : Vector3T<Type>(Type(0), -vector.z, vector.y);
		return Normalize(perpendicular);
	}

	template <std::floating_point T>
	struct OrthonormalBasis3T
	{
		using Type = T;
		Vector3T<Type> tangent, bitangent, normal;
	};

	template <std::floating_point Type>
	[[nodiscard]] OrthonormalBasis3T<Type> Build_orthonormal_basis(const Vector3T<Type> normal) noexcept
	{
	    assert(Is_normalized(normal) && "Build_orthonormal_basis: 'normal' must be normalized");
		const Type sign = std::copysign(Type(1), normal.z);
		const Type factor = Type(-1) / (sign + normal.z);
		const Type xy_factor = normal.x * normal.y * factor;

		const Vector3T<Type> tangent(Type(1) + sign * normal.x * normal.x * factor, sign * xy_factor, -sign * normal.x);
		const Vector3T<Type> bitangent(xy_factor, sign + normal.y * normal.y * factor, -normal.y);
		return OrthonormalBasis3T<Type>{tangent, bitangent, normal};
	}

	template <std::floating_point Type>
	[[nodiscard]] Radians Angle_between(const Vector3T<Type> vector_a, const Vector3T<Type> vector_b) noexcept
	{
	    assert(Is_normalizable(vector_a) && Is_normalizable(vector_b) && "Angle_between: cannot measure the angle of a zero vector or infinites values");
		const Vector3T<Type> direction_a = Normalize(vector_a);
		const Vector3T<Type> direction_b = Normalize(vector_b);
		return Radians(static_cast<float>(std::atan2(Length(Cross(direction_a, direction_b)), Dot(direction_a, direction_b))));
	}

	template <std::floating_point Type>
	[[nodiscard]] Vector3T<Type> Slerp(const Vector3T<Type> from, const Vector3T<Type> to, const std::type_identity_t<Type> factor) noexcept
	{
		assert(Is_normalized(from) && Is_normalized(to) && "Slerp: 'from' and 'to' must be normalized");
		const Vector3T<Type> orthogonal = Reject(to, from);
		const Vector3T<Type> reorthogonal = Reject(orthogonal, from);
		const Type sine = Length(orthogonal);

		const Vector3T<Type> tangent = Type(2) * Length(reorthogonal) > sine ? Normalize(reorthogonal) : Any_perpendicular(from);
		const Type angle = std::atan2(sine, Dot(from, to)) * factor;
		return from * std::cos(angle) + tangent * std::sin(angle);
	}

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
