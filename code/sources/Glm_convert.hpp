#pragma once

// Header PRIVADO: solo lo incluyen los .cpp de code/sources. Nunca desde code/headers.

#include <bit>
#include <type_traits>
#include <glm/glm.hpp>
#include <Math/Matrix3.hpp>
#include <Math/Matrix4.hpp>

namespace Math::detail
{
	// Los dos son column-major con acceso [columna][fila]: misma memoria, no hay que transponer.
	static_assert(sizeof(glm::mat3) == sizeof(Matrix3), "glm::mat3 and Matrix3 must have the same size");
	static_assert(std::is_trivially_copyable_v<glm::mat3>, "glm::mat3 must be trivially_copyable");
	static_assert(sizeof(glm::mat4) == sizeof(Matrix4), "glm::mat4 and Matrix4 must have the same size");
	static_assert(std::is_trivially_copyable_v<glm::mat4>, "glm::mat4 must be trivially_copyable");

	[[nodiscard]] inline glm::mat3 To_glm(const Matrix3& matrix) noexcept
	{
		return std::bit_cast<glm::mat3>(matrix);
	}

	[[nodiscard]] inline Matrix3 From_glm(const glm::mat3& matrix) noexcept
	{
		return std::bit_cast<Matrix3>(matrix);
	}

	[[nodiscard]] inline glm::mat4 To_glm(const Matrix4& matrix) noexcept
	{
		return std::bit_cast<glm::mat4>(matrix);
	}

	[[nodiscard]] inline Matrix4 From_glm(const glm::mat4& matrix) noexcept
	{
		return std::bit_cast<Matrix4>(matrix);
	}
}
