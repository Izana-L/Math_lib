#include "Check.hpp"
#include "Glm_convert.hpp"
#include <Math/Math.hpp>

namespace
{
	Math::Quaternion Random_rotation()
	{
		const Math::Vector3 axis(Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f));
		const Math::Vector3 safe_axis = Math::Is_normalizable(axis) ? axis : Math::Vector3(0.0f, 1.0f, 0.0f);
		return Math::Quaternion::From_axis_angle(Math::Normalize(safe_axis), Math::Radians(Math_test::Random_float(-3.1f, 3.1f)));
	}

	void Test_against_glm()
	{
		using Math::detail::From_glm;
		using Math::detail::To_glm;

		for (int i = 0; i < 1000; ++i)
		{
			const Math::Quaternion a = Random_rotation();
			const Math::Quaternion b = Random_rotation();
			const float t = Math_test::Random_float(0.0f, 1.0f);

			CHECK(Math::Equals(a * b, From_glm(To_glm(a) * To_glm(b))));
			CHECK(Math::Equals(Math::To_matrix3(a), From_glm(glm::mat3_cast(To_glm(a)))));
			CHECK(Math::Equals(Math::Slerp(a, b, t), From_glm(glm::slerp(To_glm(a), To_glm(b), t)), 1e-5f, 1e-4f));
		}
	}

	// Idas y vueltas: cuaternión -> matriz -> cuaternión. q y -q son la misma rotación.
	void Test_round_trips()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Math::Quaternion rotation = Random_rotation();
			CHECK(Math::Equals(Math::Quaternion::From_matrix3(Math::To_matrix3(rotation)), rotation));
		}

		const Math::Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
		CHECK(Math::Equals(q, Math::Quaternion(0.0f, 0.0f, 0.0f, -1.0f)));
	}
}

int main()
{
	Test_against_glm();
	Test_round_trips();
	return Math_test::Result("Test_quaternion");
}
