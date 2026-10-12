#pragma once 

#include <numbers>
#include <concepts>


namespace Math
{
	template <std::floating_point T> inline constexpr T Pi_v = std::numbers::pi_v<T>;
	template <std::floating_point T> inline constexpr T Two_pi_v = T(2) * std::numbers::pi_v<T>;
	template <std::floating_point T> inline constexpr T Half_pi_v = std::numbers::pi_v<T> / T(2);
	template <std::floating_point T> inline constexpr T Deg_to_rad_v = std::numbers::pi_v<T> / T(180);
	template <std::floating_point T> inline constexpr T Rad_to_deg_v = T(180) / std::numbers::pi_v<T>;

	inline constexpr float Pi = Pi_v<float>;
	inline constexpr float Two_pi = Two_pi_v<float>;
	inline constexpr float Half_pi = Half_pi_v<float>;
	inline constexpr float Deg_to_rad = Deg_to_rad_v<float>;
	inline constexpr float Rad_to_deg = Rad_to_deg_v<float>;

	inline constexpr float Epsilon_absolute = 1e-6f;
	inline constexpr float Epsilon_relative = 1e-5f;
}
