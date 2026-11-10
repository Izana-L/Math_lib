#pragma once

// Header PRIVADO: solo lo incluyen los .cpp de code/sources y los tests. Nunca desde code/headers.

#include <bit>
#include <type_traits>
#include "Glm_config.hpp" // IWYU pragma: keep
#include <Math/Math.hpp>

namespace Math::detail
{
	// Mismo tamaño y misma memoria: la conversión es una copia de bits (std::bit_cast).
	// Las matrices son column-major con acceso [columna][fila] en los dos: no hay que transponer.
	// El cuaternión es x, y, z, w en los dos (Glm_config.hpp lo comprueba): no hay que reordenar.
	template <typename Math_type, typename Glm_type>
	inline constexpr bool Same_layout_v = sizeof(Math_type) == sizeof(Glm_type) &&
	                                      std::is_trivially_copyable_v<Math_type> &&
	                                      std::is_trivially_copyable_v<Glm_type>;

	static_assert(Same_layout_v<Vector2, glm::vec2>, "Vector2 and glm::vec2 must have the same layout");
	static_assert(Same_layout_v<Vector3, glm::vec3>, "Vector3 and glm::vec3 must have the same layout");
	static_assert(Same_layout_v<Vector4, glm::vec4>, "Vector4 and glm::vec4 must have the same layout");
	static_assert(Same_layout_v<Matrix3, glm::mat3>, "Matrix3 and glm::mat3 must have the same layout");
	static_assert(Same_layout_v<Matrix4, glm::mat4>, "Matrix4 and glm::mat4 must have the same layout");
	static_assert(Same_layout_v<Quaternion, glm::quat>, "Quaternion and glm::quat must have the same layout");

	[[nodiscard]] inline glm::vec2 To_glm(const Vector2 vector) noexcept { return std::bit_cast<glm::vec2>(vector); }
	[[nodiscard]] inline glm::vec3 To_glm(const Vector3 vector) noexcept { return std::bit_cast<glm::vec3>(vector); }
	[[nodiscard]] inline glm::vec4 To_glm(const Vector4 vector) noexcept { return std::bit_cast<glm::vec4>(vector); }
	[[nodiscard]] inline glm::mat3 To_glm(const Matrix3& matrix) noexcept { return std::bit_cast<glm::mat3>(matrix); }
	[[nodiscard]] inline glm::mat4 To_glm(const Matrix4& matrix) noexcept { return std::bit_cast<glm::mat4>(matrix); }
	[[nodiscard]] inline glm::quat To_glm(const Quaternion quaternion) noexcept { return std::bit_cast<glm::quat>(quaternion); }

	[[nodiscard]] inline Vector2 From_glm(const glm::vec2& vector) noexcept { return std::bit_cast<Vector2>(vector); }
	[[nodiscard]] inline Vector3 From_glm(const glm::vec3& vector) noexcept { return std::bit_cast<Vector3>(vector); }
	[[nodiscard]] inline Vector4 From_glm(const glm::vec4& vector) noexcept { return std::bit_cast<Vector4>(vector); }
	[[nodiscard]] inline Matrix3 From_glm(const glm::mat3& matrix) noexcept { return std::bit_cast<Matrix3>(matrix); }
	[[nodiscard]] inline Matrix4 From_glm(const glm::mat4& matrix) noexcept { return std::bit_cast<Matrix4>(matrix); }
	[[nodiscard]] inline Quaternion From_glm(const glm::quat& quaternion) noexcept { return std::bit_cast<Quaternion>(quaternion); }
}
