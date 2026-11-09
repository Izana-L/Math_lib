#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>
#include <Math/Angle.hpp>
#include <Math/Constants.hpp>
#include <Math/Matrix3.hpp>
#include <Math/Scalar.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>


namespace Math
{
	struct Matrix4;
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
		[[nodiscard]] static Quaternion From_matrix3(const Matrix3& matrix) noexcept;
		[[nodiscard]] static Quaternion From_matrix4(const Matrix4& matrix) noexcept;
		[[nodiscard]] static Quaternion From_axis_angle(const Vector3 axis, const Radians angle) noexcept;
		[[nodiscard]] static Quaternion From_euler(const Radians yaw, const Radians pitch, const Radians roll) noexcept;
		[[nodiscard]] static Quaternion From_to_rotation(const Vector3 from, const Vector3 to) noexcept;
		[[nodiscard]] static Quaternion Look_rotation(const Vector3 forward, const Vector3 up) noexcept;
		[[nodiscard]] friend constexpr bool operator==(const Quaternion& vector, const Quaternion& other_vector)  noexcept = default;


		constexpr Quaternion& operator*=(const Quaternion other_quaternion) noexcept
		{
			const float result_x = w * other_quaternion.x + x * other_quaternion.w + y * other_quaternion.z - z * other_quaternion.y;
			const float result_y = w * other_quaternion.y - x * other_quaternion.z + y * other_quaternion.w + z * other_quaternion.x;
			const float result_z = w * other_quaternion.z + x * other_quaternion.y - y * other_quaternion.x + z * other_quaternion.w;
			const float result_w = w * other_quaternion.w - x * other_quaternion.x - y * other_quaternion.y - z * other_quaternion.z;
			x = result_x;
			y = result_y;
			z = result_z;
			w = result_w;
			return *this;
		}
		[[nodiscard]] friend constexpr Quaternion operator*(Quaternion quaternion_a, const Quaternion quaternion_b) noexcept
		{ return quaternion_a *= quaternion_b; }

	};
	static_assert(sizeof(Quaternion) == 4 * sizeof(float), "Quaternion must be exactly 4 floats in size");
	static_assert(alignof(Quaternion) == alignof(float), "Quaternion must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Quaternion>, "Quaternion must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Quaternion>, "Quaternion  must be trivially_copyable");
	static_assert(offsetof(Quaternion, x) == 0, "x must be at offset 0");
	static_assert(offsetof(Quaternion, y) == sizeof(float), "y must follow x");
	static_assert(offsetof(Quaternion, z) == 2 * sizeof(float), "z must follow y");
	static_assert(offsetof(Quaternion, w) == 3 * sizeof(float), "w must follow z");


	struct EulerAngles
	{
		Radians yaw, pitch, roll;
	};


	struct AxisAngle
	{
		Vector3 axis;
		Radians angle;
	};

	static_assert(sizeof(EulerAngles) == 3 * sizeof(float), "EulerAngles must be exactly 3 float in size");
	static_assert(alignof(EulerAngles) == alignof(float), "EulerAngles must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<EulerAngles>, "EulerAngles must be standard_layout");
	static_assert(std::is_trivially_copyable_v<EulerAngles>, "EulerAngles must be trivially_copyable");
	static_assert(offsetof(EulerAngles, yaw) == 0, "yaw must be at offset 0");
	static_assert(offsetof(EulerAngles, pitch) == sizeof(float), "pitch must follow yaw");
	static_assert(offsetof(EulerAngles, roll) == 2 * sizeof(float), "roll must follow pitch");

	static_assert(sizeof(AxisAngle) == 4 * sizeof(float), "AxisAngle must be exactly 4 float in size");
	static_assert(alignof(AxisAngle) == alignof(float), "AxisAngle must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<AxisAngle>, "AxisAngle must be standard_layout");
	static_assert(std::is_trivially_copyable_v<AxisAngle>, "AxisAngle must be trivially_copyable");
	static_assert(offsetof(AxisAngle, axis) == 0, "axis must be at offset 0");
	static_assert(offsetof(AxisAngle, angle) == 3 * sizeof(float), "angle must follow axis");

	namespace detail
	{

		[[nodiscard]] constexpr bool Is_unit_quaternion(const Quaternion quaternion) noexcept
		{
			return Equals(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w, 1.0f);
		}

		[[nodiscard]] constexpr Vector4 To_vector4(const Quaternion quaternion) noexcept
		{
			return Vector4(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		}

		// atan2 devuelve -pi cuando recibe -0: el rango de los ángulos de To_euler es (-pi, pi].
		[[nodiscard]] constexpr float Canonical_angle(const float angle) noexcept
		{
			return angle <= -Pi ? Pi : angle;
		}
	}


	[[nodiscard]] constexpr Matrix3 To_matrix3(const Quaternion rotation) noexcept
	{
		assert(detail::Is_unit_quaternion(rotation) && "To_matrix3: 'rotation' must be a normalized quaternion");
		const float xx = rotation.x * rotation.x;
		const float yy = rotation.y * rotation.y;
		const float zz = rotation.z * rotation.z;
		const float xy = rotation.x * rotation.y;
		const float xz = rotation.x * rotation.z;
		const float yz = rotation.y * rotation.z;
		const float wx = rotation.w * rotation.x;
		const float wy = rotation.w * rotation.y;
		const float wz = rotation.w * rotation.z;
		return Matrix3(Vector3(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy)),
					   Vector3(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx)),
					   Vector3(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy)));
	}

	[[nodiscard]] constexpr float Dot(const Quaternion quaternion_a, const Quaternion quaternion_b) noexcept
	{
		return quaternion_a.x * quaternion_b.x + quaternion_a.y * quaternion_b.y + quaternion_a.z * quaternion_b.z + quaternion_a.w * quaternion_b.w;
	}

	[[nodiscard]] constexpr float Length_squared(const Quaternion quaternion) noexcept
	{
		return Dot(quaternion, quaternion);
	}

	[[nodiscard]] inline float Length(const Quaternion quaternion) noexcept
	{
		return std::sqrt(Length_squared(quaternion));
	}

	[[nodiscard]] constexpr Quaternion Conjugate(const Quaternion quaternion) noexcept
	{
		return Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
	}

	[[nodiscard]] inline bool Is_invertible(const Quaternion quaternion) noexcept
	{
		return std::isnormal(Length_squared(quaternion));
	}


	[[nodiscard]] inline Quaternion Inverse(const Quaternion quaternion) noexcept
	{
		assert(Is_invertible(quaternion) && "Inverse: the quaternion must have a normal (finite, non-zero) squared length");
		const float inverse_length_squared = 1.0f / Length_squared(quaternion);
		return Quaternion(-quaternion.x * inverse_length_squared, -quaternion.y * inverse_length_squared, -quaternion.z * inverse_length_squared, quaternion.w * inverse_length_squared);
	}

	[[nodiscard]] inline bool Is_normalizable(const Quaternion quaternion) noexcept
	{
		return Is_normalizable(detail::To_vector4(quaternion));
	}

	[[nodiscard]] inline Quaternion Normalize(const Quaternion quaternion) noexcept
	{
		assert(Is_normalizable(quaternion) && "Normalize: cannot normalize a zero quaternion or infinites values");
		const Vector4 normalized = Normalize(detail::To_vector4(quaternion));
		return Quaternion(normalized.x, normalized.y, normalized.z, normalized.w);
	}


	[[nodiscard]] constexpr bool Equals(const Quaternion quaternion_a, const Quaternion quaternion_b, const float absolute_tolerance, const float relative_tolerance) noexcept
	{
		return (Equals(quaternion_a.x, quaternion_b.x, absolute_tolerance, relative_tolerance) && Equals(quaternion_a.y, quaternion_b.y, absolute_tolerance, relative_tolerance) &&
				Equals(quaternion_a.z, quaternion_b.z, absolute_tolerance, relative_tolerance) && Equals(quaternion_a.w, quaternion_b.w, absolute_tolerance, relative_tolerance)) ||
			   (Equals(quaternion_a.x, -quaternion_b.x, absolute_tolerance, relative_tolerance) && Equals(quaternion_a.y, -quaternion_b.y, absolute_tolerance, relative_tolerance) &&
				Equals(quaternion_a.z, -quaternion_b.z, absolute_tolerance, relative_tolerance) && Equals(quaternion_a.w, -quaternion_b.w, absolute_tolerance, relative_tolerance));
	}

	[[nodiscard]] constexpr bool Equals(const Quaternion quaternion_a, const Quaternion quaternion_b) noexcept
	{
		return Equals(quaternion_a, quaternion_b, Epsilon_absolute, Epsilon_relative);
	}


	[[nodiscard]] constexpr bool Is_identity(const Quaternion quaternion) noexcept
	{
		return Equals(quaternion, Quaternion::Identity());
	}


	[[nodiscard]] constexpr Vector3 Rotate_vector(const Quaternion rotation, const Vector3 vector) noexcept
	{
		assert(detail::Is_unit_quaternion(rotation) && "Rotate_vector: 'rotation' must be a normalized quaternion");
		const Vector3 axis_part(rotation.x, rotation.y, rotation.z);
		const Vector3 twice_cross = Cross(axis_part, vector) * 2.0f;
		return vector + twice_cross * rotation.w + Cross(axis_part, twice_cross);
	}


	[[nodiscard]] constexpr Vector3 Right(const Quaternion rotation) noexcept { return Rotate_vector(rotation, Vector3(1.0f, 0.0f, 0.0f)); }
	[[nodiscard]] constexpr Vector3 Up(const Quaternion rotation) noexcept { return Rotate_vector(rotation, Vector3(0.0f, 1.0f, 0.0f)); }
	[[nodiscard]] constexpr Vector3 Forward(const Quaternion rotation) noexcept { return Rotate_vector(rotation, Vector3(0.0f, 0.0f, -1.0f)); }


	[[nodiscard]] inline Radians Angle_between(const Quaternion from, const Quaternion to) noexcept
	{
		assert(detail::Is_unit_quaternion(from) && detail::Is_unit_quaternion(to) && "Angle_between: 'from' and 'to' must be normalized quaternions");
		const Quaternion relative = Conjugate(from) * to;
		return Radians(2.0f * std::atan2(Length(Vector3(relative.x, relative.y, relative.z)), detail::Abs_of(relative.w)));
	}


	[[nodiscard]] constexpr Quaternion Lerp(const Quaternion from, const Quaternion to, const float factor) noexcept
	{
		const float sign = Dot(from, to) < 0.0f ? -1.0f : 1.0f;
		return Quaternion(Lerp(from.x, sign * to.x, factor), Lerp(from.y, sign * to.y, factor), Lerp(from.z, sign * to.z, factor), Lerp(from.w, sign * to.w, factor));
	}

	[[nodiscard]] inline Quaternion Nlerp(const Quaternion from, const Quaternion to, const float factor) noexcept
	{
		assert(detail::Is_unit_quaternion(from) && detail::Is_unit_quaternion(to) && "Nlerp: 'from' and 'to' must be normalized quaternions");
		return Normalize(Lerp(from, to, factor));
	}

	[[nodiscard]] inline Quaternion Slerp(const Quaternion from, const Quaternion to, const float factor) noexcept
	{
		assert(detail::Is_unit_quaternion(from) && detail::Is_unit_quaternion(to) && "Slerp: 'from' and 'to' must be normalized quaternions");
		const float cosine = Dot(from, to);
		const float sign = cosine < 0.0f ? -1.0f : 1.0f;
		const float short_cosine = sign * cosine;
		if (short_cosine > 1.0f - Epsilon_absolute)
		{
			return Nlerp(from, to, factor);
		}
		const float arc = std::acos(short_cosine);
		const float inverse_sine = 1.0f / std::sin(arc);
		const float from_weight = std::sin((1.0f - factor) * arc) * inverse_sine;
		const float to_weight = sign * std::sin(factor * arc) * inverse_sine;
		return Quaternion(from.x * from_weight + to.x * to_weight, from.y * from_weight + to.y * to_weight,
						  from.z * from_weight + to.z * to_weight, from.w * from_weight + to.w * to_weight);
	}

	[[nodiscard]] inline Quaternion Rotate_towards(const Quaternion from, const Quaternion to, const Radians max_angle) noexcept
	{
		assert(detail::Is_unit_quaternion(from) && detail::Is_unit_quaternion(to) && "Rotate_towards: 'from' and 'to' must be normalized quaternions");
		assert(max_angle.value >= 0.0f && "Rotate_towards: 'max_angle' must be more or equal than 0");
		const Radians angle = Angle_between(from, to);
		if (angle <= max_angle)
		{
			return to;
		}
		return Slerp(from, to, max_angle.value / angle.value);
	}


	[[nodiscard]] inline AxisAngle To_axis_angle(const Quaternion rotation) noexcept
	{
		assert(detail::Is_unit_quaternion(rotation) && "To_axis_angle: 'rotation' must be a normalized quaternion");
		const float sign = rotation.w < 0.0f ? -1.0f : 1.0f;
		const Vector3 axis_part(sign * rotation.x, sign * rotation.y, sign * rotation.z);
		if (!Is_normalizable(axis_part))
		{
			return AxisAngle{Vector3(1.0f, 0.0f, 0.0f), Radians(0.0f)};
		}
		return AxisAngle{Normalize(axis_part), Radians(2.0f * std::atan2(Length(axis_part), sign * rotation.w))};
	}

	[[nodiscard]] inline EulerAngles To_euler(const Quaternion rotation) noexcept
	{
		assert(detail::Is_unit_quaternion(rotation) && "To_euler: 'rotation' must be a normalized quaternion");
		const float xx = rotation.x * rotation.x;
		const float yy = rotation.y * rotation.y;
		const float zz = rotation.z * rotation.z;
		const float xy = rotation.x * rotation.y;
		const float xz = rotation.x * rotation.z;
		const float yz = rotation.y * rotation.z;
		const float wx = rotation.w * rotation.x;
		const float wy = rotation.w * rotation.y;
		const float wz = rotation.w * rotation.z;


		const float r02 = 2.0f * (xz + wy);
		const float r12 = 2.0f * (yz - wx);
		const float r22 = 1.0f - 2.0f * (xx + yy);
		const Radians pitch(std::atan2(-r12, std::sqrt(r02 * r02 + r22 * r22)));

		if (detail::Abs_of(r12) >= 1.0f - Epsilon_absolute)
		{
			const float r00 = 1.0f - 2.0f * (yy + zz);
			const float r20 = 2.0f * (xz - wy);
			return EulerAngles{Radians(detail::Canonical_angle(std::atan2(-r20, r00))), pitch, Radians(0.0f)};
		}

		const float r10 = 2.0f * (xy + wz);
		const float r11 = 1.0f - 2.0f * (xx + zz);
		return EulerAngles{Radians(detail::Canonical_angle(std::atan2(r02, r22))), pitch, Radians(detail::Canonical_angle(std::atan2(r10, r11)))};
	}


	inline Quaternion Quaternion::From_axis_angle(const Vector3 axis, const Radians angle) noexcept
	{
		assert(Is_normalized(axis) && "From_axis_angle: 'axis' must be normalized");
		const float half_angle = angle.value * 0.5f;
		const float sine = std::sin(half_angle);
		return Quaternion(axis.x * sine, axis.y * sine, axis.z * sine, std::cos(half_angle));
	}


	inline Quaternion Quaternion::From_euler(const Radians yaw, const Radians pitch, const Radians roll) noexcept
	{
		const float sin_yaw = std::sin(yaw.value * 0.5f);
		const float cos_yaw = std::cos(yaw.value * 0.5f);
		const float sin_pitch = std::sin(pitch.value * 0.5f);
		const float cos_pitch = std::cos(pitch.value * 0.5f);
		const float sin_roll = std::sin(roll.value * 0.5f);
		const float cos_roll = std::cos(roll.value * 0.5f);
		return Quaternion(cos_yaw * sin_pitch * cos_roll + sin_yaw * cos_pitch * sin_roll,
						  sin_yaw * cos_pitch * cos_roll - cos_yaw * sin_pitch * sin_roll,
						  cos_yaw * cos_pitch * sin_roll - sin_yaw * sin_pitch * cos_roll,
						  cos_yaw * cos_pitch * cos_roll + sin_yaw * sin_pitch * sin_roll);
	}


	inline Quaternion Quaternion::From_to_rotation(const Vector3 from, const Vector3 to) noexcept
	{
		assert(Is_normalizable(from) && Is_normalizable(to) && "From_to_rotation: 'from' and 'to' cannot be zero vectors or have infinite or NaN values");
		const Vector3 direction_from = Normalize(from);
		const Vector3 direction_to = Normalize(to);
		const float cosine = Dot(direction_from, direction_to);
		if (cosine < -1.0f + Epsilon_absolute)
		{
			const Vector3 axis = Any_perpendicular(direction_from);
			return Quaternion(axis.x, axis.y, axis.z, 0.0f);
		}
		const Vector3 axis = Cross(direction_from, direction_to);
		return Normalize(Quaternion(axis.x, axis.y, axis.z, 1.0f + cosine));
	}


	inline Quaternion Quaternion::Look_rotation(const Vector3 forward, const Vector3 up) noexcept
	{
		assert(Is_normalizable(forward) && Is_normalizable(up) && "Look_rotation: 'forward' and 'up' cannot be zero vectors or have infinite or NaN values");
		const Vector3 direction = Normalize(forward);
		const Vector3 side = Cross(direction, Normalize(up));
		assert(Is_normalizable(side) && "Look_rotation: 'forward' and 'up' cannot be parallel");
		const Vector3 right = Normalize(side);
		return From_matrix3(Matrix3(right, Cross(right, direction), -direction));
	}
}
