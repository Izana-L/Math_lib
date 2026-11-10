#pragma once

// Mini arnés de tests de Math. Cada archivo Test_*.cpp de esta carpeta es un ejecutable
// con su propio main, y CMake lo registra en ctest con add_test. Un test pasa si su
// main devuelve 0.
//
// CHECK no usa assert: sigue funcionando en Release (con NDEBUG).

#include <cstdio>
#include <random>

namespace Math_test
{
	inline int total_checks = 0;
	inline int failed_checks = 0;

	inline void Check(const bool passed, const char* expression, const char* file, const int line) noexcept
	{
		++total_checks;
		if (!passed)
		{
			++failed_checks;
			std::fprintf(stderr, "%s:%d: FALLO: %s\n", file, line, expression);
		}
	}

	// Devuelve el código de salida del test: 0 si todo ha ido bien.
	[[nodiscard]] inline int Result(const char* test_name) noexcept
	{
		std::printf("%s: %d de %d comprobaciones correctas\n", test_name, total_checks - failed_checks, total_checks);
		return failed_checks == 0 ? 0 : 1;
	}

	// Generador con semilla fija: el mismo ejecutable da siempre las mismas entradas y un fallo
	// se puede repetir. (La secuencia puede cambiar entre compiladores: std::uniform_real_distribution
	// no está fijada por el estándar.)
	inline std::mt19937& Random_engine() noexcept
	{
		static std::mt19937 engine(12345u);
		return engine;
	}

	[[nodiscard]] inline float Random_float(const float min, const float max) noexcept
	{
		return std::uniform_real_distribution<float>(min, max)(Random_engine());
	}
}

// "? true : false" convierte a bool igual que un if (vale para std::optional) sin avisos de cast inútil.
#define CHECK(...) ::Math_test::Check((__VA_ARGS__) ? true : false, #__VA_ARGS__, __FILE__, __LINE__)
