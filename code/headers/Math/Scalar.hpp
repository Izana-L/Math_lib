#pragma once

#include <Math/Angle.hpp>
#include <Math/Constants.hpp>
#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>

namespace Math
{
	namespace detail
	{
		template <std::floating_point T>
		inline constexpr T Epsilon_scale_v = std::numeric_limits<T>::epsilon() / std::numeric_limits<float>::epsilon();

		template <typename Type>
		[[nodiscard]] constexpr Type Abs_of(const Type value) noexcept
		{
			if constexpr (std::floating_point<Type>)
			{
				return std::abs(value);
			}
			else
			{
				return value < Type(0) ? -value : value;
			}
		}

		// -1, 0 o +1 (con tipos sin signo, 0 o 1). Con NaN da 0.
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
		return (value - from) / (to - from);
	}
	template <std::floating_point T>
	[[nodiscard]] constexpr T Remap(const T value, const T from_min, const T from_max, const T to_min, const T to_max) noexcept
	{
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
		const T t = Saturate(Inverse_lerp(lower_edge, upper_edge, value));
		return t * t * (T(3) - T(2) * t);
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr T Move_towards(const T current, const T target, const T max_step) noexcept
	{
		const T difference = target - current;
		const T distance = difference < T(0) ? -difference : difference;

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

		const T abs_a = a < T(0) ? -a : a;
		const T abs_b = b < T(0) ? -b : b;
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
		const T result = value - std::floor(value / length) * length;

		return (result < T(0) || result >= length) ? T(0) : result;
	}
	template <std::floating_point T>
	[[nodiscard]] T Wrap(const T value, const T minimum, const T maximum) noexcept
	{
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
}
