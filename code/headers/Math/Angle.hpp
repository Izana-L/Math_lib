#pragma once 


#include <Math/Constants.hpp>
#include <concepts>
#include <compare>
#include <type_traits>


namespace Math
{
	template <std::floating_point T>
	struct RadiansT
	{
		using Type = T;

		Type value;

		constexpr RadiansT() noexcept :value(){}

		constexpr explicit RadiansT(const Type _value) noexcept : value(_value) {}

		[[nodiscard]]  constexpr bool operator==(const RadiansT<Type>&, const RadiansT<Type>&) noexcept = default;
		[[nodiscard]]  constexpr auto operator<=>(const RadiansT<Type>&, const RadiansT<Type>&) noexcept = default;

		[[nodiscard]]  constexpr RadiansT operator+(RadiansT a, RadiansT b) noexcept { return RadiansT(a.value + b.value); }
		[[nodiscard]]  constexpr RadiansT operator-(RadiansT a, RadiansT b) noexcept { return RadiansT(a.value - b.value); }
		[[nodiscard]]  constexpr RadiansT operator-(RadiansT a) noexcept { return RadiansT(-a.value); }
		[[nodiscard]]  constexpr RadiansT operator*(RadiansT a, Type scalar) noexcept { return RadiansT(a.value * scalar); }
		[[nodiscard]]  constexpr RadiansT operator*(Type scalar, RadiansT a) noexcept { return RadiansT(scalar * a.value); }
		[[nodiscard]]  constexpr RadiansT operator/(RadiansT a, Type scalar) noexcept { return RadiansT(a.value / scalar); }

		constexpr RadiansT& operator+=(RadiansT other) noexcept { value += other.value; return *this; }
		constexpr RadiansT& operator-=(RadiansT other) noexcept { value -= other.value; return *this; }
	};
	using Radians = RadiansT<float>;

	static_assert(sizeof(Radians) == sizeof(float), "Radians must be exactly one float in size");
	static_assert(alignof(Radians) == alignof(float), "Radians must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Radians>, "Radians must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Radians>, "Radians must be trivially_copyable");
	static_assert(offsetof(Radians, value) == 0, "value must be at offset 0");

	template <std::floating_point T>
	struct DegreesT
	{
		using Type = T;

		Type value;

		constexpr DegreesT() noexcept :value() {}

		constexpr explicit DegreesT(const Type _value) noexcept : value(_value) {}

		[[nodiscard]]  constexpr bool operator==(const DegreesT&, const DegreesT&) noexcept = default;
		[[nodiscard]]  constexpr auto operator<=>(const DegreesT&, const DegreesT&) noexcept = default;

		[[nodiscard]]  constexpr DegreesT operator+(DegreesT a, DegreesT b) noexcept { return DegreesT(a.value + b.value); }
		[[nodiscard]]  constexpr DegreesT operator-(DegreesT a, DegreesT b) noexcept { return DegreesT(a.value - b.value); }
		[[nodiscard]]  constexpr DegreesT operator-(DegreesT a) noexcept { return DegreesT(-a.value); }
		[[nodiscard]]  constexpr DegreesT operator*(DegreesT a, Type scalar) noexcept { return DegreesT(a.value * scalar); }
		[[nodiscard]]  constexpr DegreesT operator*(Type scalar, DegreesT a) noexcept { return DegreesT(scalar * a.value); }
		[[nodiscard]]  constexpr DegreesT operator/(DegreesT a, Type scalar) noexcept { return DegreesT(a.value / scalar); }

		constexpr DegreesT& operator+=(DegreesT other) noexcept { value += other.value; return *this; }
		constexpr DegreesT& operator-=(DegreesT other) noexcept { value -= other.value; return *this; }
	};
	using Degrees = DegreesT<float>;

	static_assert(sizeof(Degrees) == sizeof(float), "Degrees must be exactly one float in size");
	static_assert(alignof(Degrees) == alignof(float), "Degrees must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Degrees>, "Degrees must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Degrees>, "Degrees must be trivially_copyable");
	static_assert(offsetof(Degrees, value) == 0, "value must be at offset 0");

	template <std::floating_point T>
	[[nodiscard]] constexpr DegreesT<T> To_degrees(RadiansT<T> radians) noexcept { return DegreesT<T>(radians.value * Rad_to_deg_v<T>); }
	template <std::floating_point T>
	[[nodiscard]] constexpr RadiansT<T> To_redians(DegreesT<T> radians) noexcept { return RadiansT<T>(radians.value * Deg_to_rad_v<Type>); }

	inline namespace literals
	{
		[[nodiscard]] constexpr Degrees operator""_deg(long double value) noexcept { return Degrees(static_cast<float>(value)); }
		[[nodiscard]] constexpr Degrees operator""_deg(unsigned long long value) noexcept { return Degrees(static_cast<float>(value)); }
		[[nodiscard]] constexpr Radians operator""_rad(long double value) noexcept { return Radians(static_cast<float>(value)); }
		[[nodiscard]] constexpr Radians operator""_rad(unsigned long long value) noexcept { return Radians(static_cast<float>(value)); }
	}
}
