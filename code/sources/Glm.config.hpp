#pragma once

// Header PRIVADO: el único punto de entrada a glm. Lo incluyen los .cpp de code/sources
// (a través de Glm_convert.hpp) y los tests. Nunca un header de code/headers.
//
// Las macros de configuración NO se definen aquí, sino en el target Math_glm de
// CMakeLists.txt. Así todas las unidades de traducción que ven glm reciben exactamente
// la misma configuración: si un .cpp incluyera glm sin pasar antes por aquí, tendría
// otra configuración y el programa incumpliría la ODR. Este header solo comprueba que
// las macros han llegado y que glm se comporta como dice CONVENTIONS.md.

// 1. Las macros llegan por la línea de comandos, antes de cualquier header de glm.
#if !defined(GLM_FORCE_DEPTH_ZERO_TO_ONE) || !defined(GLM_FORCE_QUAT_DATA_XYZW)
#error "Glm_config.hpp: faltan las macros de Math_glm. Enlaza el target Math_glm en vez de añadir la ruta de glm a mano."
#endif

// 2. Macros que contradicen CONVENTIONS.md (§2, §6) o cambian el layout de los tipos.
#if defined(GLM_FORCE_LEFT_HANDED) || defined(GLM_FORCE_QUAT_DATA_WXYZ) || \
    defined(GLM_FORCE_DEFAULT_ALIGNED_GENTYPES) || defined(GLM_FORCE_ALIGNED)
#error "Glm_config.hpp: hay una macro de glm que contradice CONVENTIONS.md (mano izquierda, cuaternion w,x,y,z o tipos alineados)."
#endif

#include <cstddef>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Math::detail
{
	// Versión: las comprobaciones de abajo están pensadas para glm 1.0.x. Si actualizas glm,
	// revísalas y cambia esta línea.
	static_assert(GLM_VERSION_MAJOR == 1 && GLM_VERSION_MINOR == 0, "Glm_config.hpp: written for glm 1.0.x, review it after updating glm");

	// Clip space (CONVENTIONS.md §2 y §4): diestro y profundidad 0..1. Afecta a las funciones
	// de glm que se usan como oráculo (perspective, ortho, lookAt...). Reverse-Z y el volteo de
	// Y no son cosa de glm: los aplica Math en sus propias proyecciones.
	static_assert((GLM_CONFIG_CLIP_CONTROL & GLM_CLIP_CONTROL_RH_BIT) != 0, "glm must be right-handed (CONVENTIONS.md §2)");
	static_assert((GLM_CONFIG_CLIP_CONTROL & GLM_CLIP_CONTROL_ZO_BIT) != 0, "glm depth must be 0..1 (CONVENTIONS.md §4)");

	// Cuaternión (CONVENTIONS.md §6): x, y, z, w en memoria...
	static_assert(sizeof(glm::quat) == 4 * sizeof(float), "glm::quat must be exactly 4 floats");
	static_assert(std::is_standard_layout_v<glm::quat>, "glm::quat must be standard_layout");
	static_assert(offsetof(glm::quat, x) == 0 * sizeof(float), "glm::quat memory order must be x, y, z, w");
	static_assert(offsetof(glm::quat, y) == 1 * sizeof(float), "glm::quat memory order must be x, y, z, w");
	static_assert(offsetof(glm::quat, z) == 2 * sizeof(float), "glm::quat memory order must be x, y, z, w");
	static_assert(offsetof(glm::quat, w) == 3 * sizeof(float), "glm::quat memory order must be x, y, z, w");

	// ...y también en el constructor. Esto es lo que quita la trampa de glm 1.0.3.
	inline constexpr glm::quat Quaternion_order_probe(1.0f, 2.0f, 3.0f, 4.0f);
	static_assert(Quaternion_order_probe.x == 1.0f && Quaternion_order_probe.y == 2.0f &&
	              Quaternion_order_probe.z == 3.0f && Quaternion_order_probe.w == 4.0f,
	              "glm::quat constructor must take (x, y, z, w): define GLM_FORCE_QUAT_DATA_XYZW");

	// Sin alineación extra ni relleno (los tipos de Math están alineados como un float).
	static_assert(sizeof(glm::vec3) == 3 * sizeof(float) && alignof(glm::vec3) == alignof(float), "glm::vec3 must be 3 packed floats");
	static_assert(sizeof(glm::vec4) == 4 * sizeof(float) && alignof(glm::vec4) == alignof(float), "glm::vec4 must be 4 packed floats");
	static_assert(sizeof(glm::mat4) == 16 * sizeof(float) && alignof(glm::mat4) == alignof(float), "glm::mat4 must be 16 packed floats");
}
