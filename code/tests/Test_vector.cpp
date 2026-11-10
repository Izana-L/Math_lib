#include "Check.hpp"
#include "Glm_convert.hpp"
#include <Math/Math.hpp>

namespace
{
	Math::Vector3 Random_vector3()
	{
		return Math::Vector3(Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f));
	}

	// glm como oráculo, con entradas aleatorias.
	void Test_against_glm()
	{
		using Math::detail::From_glm;
		using Math::detail::To_glm;

		for (int i = 0; i < 1000; ++i)
		{
			const Math::Vector3 a = Random_vector3();
			const Math::Vector3 b = Random_vector3();

			CHECK(Math::Equals(Math::Dot(a, b), glm::dot(To_glm(a), To_glm(b))));
			CHECK(Math::Equals(Math::Cross(a, b), From_glm(glm::cross(To_glm(a), To_glm(b)))));
			CHECK(Math::Equals(Math::Length(a), glm::length(To_glm(a))));
			if (Math::Is_normalizable(a))
			{
				CHECK(Math::Equals(Math::Normalize(a), From_glm(glm::normalize(To_glm(a)))));
			}
		}
	}

	// Casos límite (ROADMAP, Tests).
	void Test_edge_cases()
	{
		CHECK(!Math::Try_normalize(Math::Vector3()).has_value());
		CHECK(Math::Is_normalized(Math::Normalize(Math::Vector3(1e-30f, 0.0f, 0.0f))));
		CHECK(Math::Is_normalized(Math::Normalize(Math::Vector3(1e30f, 1e30f, 1e30f))));
	}
}

int main()
{
	Test_against_glm();
	Test_edge_cases();
	return Math_test::Result("Test_vector");
}
