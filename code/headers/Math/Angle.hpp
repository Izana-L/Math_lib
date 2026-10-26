#pragma once


#include <Math/Constants.hpp>
#include <concepts>
#include <compare> // IWYU pragma: keep
#include <type_traits>
#include <cstddef>

namespace Math
{



    struct Radians;

	struct Degrees
	{

		float value;

		constexpr Degrees() noexcept :value(0.0f) {}
		constexpr Degrees(const Radians _rad);
		constexpr explicit Degrees(const float _value) noexcept : value(_value) {}

		[[nodiscard]] friend constexpr bool operator==(const Degrees&, const Degrees&) noexcept = default;
		[[nodiscard]] friend constexpr auto operator<=>(const Degrees&, const Degrees&) noexcept = default;

		[[nodiscard]] friend constexpr Degrees operator+(Degrees a, Degrees b) noexcept { return Degrees(a.value + b.value); }
		[[nodiscard]] friend constexpr Degrees operator-(Degrees a, Degrees b) noexcept { return Degrees(a.value - b.value); }
		[[nodiscard]] friend constexpr Degrees operator-(Degrees a) noexcept { return Degrees(-a.value); }
		[[nodiscard]] friend constexpr Degrees operator*(Degrees a, float scalar) noexcept { return Degrees(a.value * scalar); }
		[[nodiscard]] friend constexpr Degrees operator*(float scalar, Degrees a) noexcept { return Degrees(scalar * a.value); }
		[[nodiscard]] friend constexpr Degrees operator/(Degrees a, float scalar) noexcept { return Degrees(a.value / scalar); }


		constexpr Degrees& operator+=(Degrees other) noexcept { value += other.value; return *this; }
		constexpr Degrees& operator-=(Degrees other) noexcept { value -= other.value; return *this; }
	};


	static_assert(sizeof(Degrees) == sizeof(float), "Degrees must be exactly one float in size");
	static_assert(alignof(Degrees) == alignof(float), "Degrees must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Degrees>, "Degrees must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Degrees>, "Degrees must be trivially_copyable");
	static_assert(offsetof(Degrees, value) == 0, "value must be at offset 0");

	struct Radians
	{


		float value;

		constexpr Radians() noexcept :value(0.0f){}

		constexpr explicit Radians(const float _value) noexcept : value(_value * Deg_to_rad) {}
		constexpr Radians(const Degrees _degrees) noexcept : value(_degrees.value) {}
		[[nodiscard]] friend constexpr bool operator==(const Radians&, const Radians&) noexcept = default;
		[[nodiscard]] friend constexpr auto operator<=>(const Radians&, const Radians&) noexcept = default;


		[[nodiscard]] friend constexpr Radians operator+(Radians a, Radians b) noexcept { return Radians(a.value + b.value); }
		[[nodiscard]] friend constexpr Radians operator-(Radians a, Radians b) noexcept { return Radians(a.value - b.value); }
		[[nodiscard]] friend constexpr Radians operator-(Radians a) noexcept { return Radians(-a.value); }
		[[nodiscard]] friend constexpr Radians operator*(Radians a, float scalar) noexcept { return Radians(a.value * scalar); }
		[[nodiscard]] friend constexpr Radians operator*(float scalar, Radians a) noexcept { return Radians(scalar * a.value); }
		[[nodiscard]] friend constexpr Radians operator/(Radians a, float scalar) noexcept { return Radians(a.value / scalar); }

		constexpr Radians& operator+=(Radians other) noexcept { value += other.value; return *this; }
		constexpr Radians& operator-=(Radians other) noexcept { value -= other.value; return *this; }
	};


	static_assert(sizeof(Radians) == sizeof(float), "Radians must be exactly one float in size");
	static_assert(alignof(Radians) == alignof(float), "Radians must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Radians>, "Radians must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Radians>, "Radians must be trivially_copyable");
	static_assert(offsetof(Radians, value) == 0, "value must be at offset 0");


	constexpr Degrees::Degrees(const Radians _rad) : value(_rad.value * Rad_to_deg){}


	inline namespace literals
	{
		[[nodiscard]] constexpr Degrees operator""_deg(long double value) noexcept { return Degrees(static_cast<float>(value)); }
		[[nodiscard]] constexpr Degrees operator""_deg(unsigned long long value) noexcept { return Degrees(static_cast<float>(value)); }
		[[nodiscard]] constexpr Radians operator""_rad(long double value) noexcept { return Radians(static_cast<float>(value)); }
		[[nodiscard]] constexpr Radians operator""_rad(unsigned long long value) noexcept { return Radians(static_cast<float>(value)); }
	}
}
