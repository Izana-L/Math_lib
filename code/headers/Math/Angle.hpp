#pragma once 


#include <Math/Constants.hpp>
#include <concepts>
#include <compare>
#include <type_traits>


namespace Math
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct RadiansT
	{
		using Type = T;

		Type value;

		constexpr RadiansT() noexcept :value(){}

		constexpr explicit RadiansT(const Type _value) noexcept : value(_value) {}

	};


	template <typename T>
	requires std::is_arithmetic_v<T>
	struct DegreesT
	{
		using Type = T;

		Type value;

		constexpr DegreesT() noexcept :value() {}

		constexpr explicit DegreesT(const Type _value) noexcept : value(_value) {}

	};
}
