#pragma once

#include <Math/Angle.hpp>
#include <Math/Constants.hpp>
#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <cassert>
#include <bit>
#include <type_traits>
namespace Math
{
	namespace detail
	{
		template <std::floating_point T>
		inline constexpr T Epsilon_scale_v = std::numeric_limits<T>::epsilon() / std::numeric_limits<float>::epsilon();

		template <typename Type>
		[[nodiscard]] constexpr Type Abs_of(const Type value) noexcept
		{
			return value >= Type(0) ? value : -value;
		}
		template <typename Type>
		concept Unsigned_integer = std::unsigned_integral<Type> && !std::same_as<Type, bool>;

		template <typename Type>
		[[nodiscard]] constexpr Type Sign_of(const Type value) noexcept
		{
			if constexpr (std::unsigned_integral<Type>)
			{
				return value > Type(0) ? Type(1) : Type(0);
			}
			else
			{
				return static_cast<Type>((Type(0) < value) - (value < Type(0)));
			}
		}
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr T Lerp(const T from, const T to, const T factor) noexcept
	{
		return (T(1) - factor) * from + factor * to;
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr T Inverse_lerp(const T from, const T to, const T value) noexcept
	{
	    assert(from != to && "Inverse_lerp: 'from' and 'to' must be different");
		return (value - from) / (to - from);
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr T Remap(const T value, const T from_min, const T from_max, const T to_min, const T to_max) noexcept
	{
	    assert(from_min != from_max && "Remap: 'from_min' and 'from_max' must be different");
		return Lerp(to_min, to_max, Inverse_lerp(from_min, from_max, value));
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr T Saturate(const T value) noexcept
	{
		return std::clamp(value, T(0), T(1));
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr T Smoothstep(const T lower_edge, const T upper_edge, const T value) noexcept
	{
	    assert(lower_edge < upper_edge && "Smoothstep: 'lower_edge' must be less than 'upper_edge'");
		const T t = Saturate(Inverse_lerp(lower_edge, upper_edge, value));
		return t * t * (T(3) - T(2) * t);
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr T Move_towards(const T current, const T target, const T max_step) noexcept
	{
	    assert(max_step >= 0 && "Move_towards: 'max_step' must be more or equal than 0");
		const T difference = target - current;
		const T distance = detail::Abs_of(difference);

		if (distance <= max_step)
		{
			return target;
		}
		return difference > T(0) ? current + max_step : current - max_step;
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr bool Equals(const T a, const T b, const T absolute_tolerance, const T relative_tolerance) noexcept
	{
		if (a == b)
		{
			return true;
		}

		const T abs_a = detail::Abs_of(a);
		const T abs_b = detail::Abs_of(b);
		const T largest_finite = std::numeric_limits<T>::max();


		if (!(abs_a <= largest_finite) || !(abs_b <= largest_finite))
		{
			return false;
		}

		const T difference = a > b ? a - b : b - a;
		const T largest = abs_a > abs_b ? abs_a : abs_b;

		return difference <= std::max(absolute_tolerance, relative_tolerance * largest);
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr bool Equals(const T a, const T b) noexcept
	{
		return Equals(a, b,static_cast<T>(Epsilon_absolute) * detail::Epsilon_scale_v<T>,static_cast<T>(Epsilon_relative) * detail::Epsilon_scale_v<T>);
	}

	template <std::floating_point T>
	[[nodiscard]] T Repeat(const T value, const T length) noexcept
	{
	    assert(length > T(0) && "Repeat: 'length' must be greater than 0");
		const T result = value - std::floor(value / length) * length;

		return (result < T(0) || result >= length) ? T(0) : result;
	}
	template <std::floating_point T>
	[[nodiscard]] T Wrap(const T value, const T minimum, const T maximum) noexcept
	{
	    assert(minimum < maximum && "Wrap: 'minimum' must be less than 'maximum'");
		return minimum + Repeat(value - minimum, maximum - minimum);
	}

	[[nodiscard]] inline Radians Delta_angle(const Radians from, const Radians to) noexcept
	{
	    const float r = Repeat(to.value - from.value, Two_pi);
		return r > Pi ? Radians(r - Two_pi) : Radians(r);
	}

	template <std::floating_point T>
	[[nodiscard]] T Exp_decay(const T current, const T target, const T decay_rate, const T delta_time) noexcept
	{
		return target + (current - target) * std::exp(-decay_rate * delta_time);
	}
	template <std::floating_point T>
	[[nodiscard]] T Fract(const T value) noexcept
	{
		return value - std::floor(value);
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr bool Is_power_of_two(const T value) noexcept
	{
		return std::has_single_bit(value);
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr T Next_power_of_two(const T value) noexcept
	{
		assert(value <= static_cast<T>(T(1) << (std::numeric_limits<T>::digits - 1)) && "Next_power_of_two: the result does not fit in the type");
		return std::bit_ceil(value);
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr T Align_up(const T value, const std::type_identity_t<T> alignment) noexcept
	{
		assert(Is_power_of_two(alignment) && "Align_up: 'alignment' must be a power of two");
		const T mask = static_cast<T>(alignment - 1);
		assert(value <= std::numeric_limits<T>::max() - mask && "Align_up: the result does not fit in the type");
		return static_cast<T>((value + mask) & static_cast<T>(~mask));
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr T Align_down(const T value, const std::type_identity_t<T> alignment) noexcept
	{
		assert(Is_power_of_two(alignment) && "Align_down: 'alignment' must be a power of two");
		const T mask = static_cast<T>(alignment - 1);
		return static_cast<T>(value & static_cast<T>(~mask));
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr T Div_ceil(const T numerator, const std::type_identity_t<T> denominator) noexcept
	{
		assert(denominator != 0 && "Div_ceil: 'denominator' must not be zero");
		return static_cast<T>(numerator / denominator + (numerator % denominator != 0 ? 1 : 0));
	}

	template <detail::Unsigned_integer T>
	[[nodiscard]] constexpr T Mip_levels(const T width, const std::type_identity_t<T> height) noexcept
	{
		assert(width >= 1 && height >= 1 && "Mip_levels: 'width' and 'height' must be at least 1");
		return static_cast<T>(std::bit_width(width > height ? width : height));
	}
}
