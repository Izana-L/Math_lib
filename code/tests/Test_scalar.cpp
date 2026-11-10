#include "Check.hpp"
#include <Glm_config.hpp>
#include <glm/gtc/constants.hpp>
#include <Math/Math.hpp>

namespace
{
	void Test_constants()
	{
		CHECK(Math::Pi == glm::pi<float>());
		CHECK(Math::Two_pi == glm::two_pi<float>());
		CHECK(Math::Half_pi == glm::half_pi<float>());
	}

	void Test_lerp_and_smoothstep()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const float a = Math_test::Random_float(-10.0f, 10.0f);
			const float b = Math_test::Random_float(-10.0f, 10.0f);
			const float t = Math_test::Random_float(0.0f, 1.0f);
			CHECK(Math::Equals(Math::Lerp(a, b, t), glm::mix(a, b, t)));
			CHECK(Math::Equals(Math::Smoothstep(-1.0f, 1.0f, a / 10.0f), glm::smoothstep(-1.0f, 1.0f, a / 10.0f)));
		}
	}
}

int main()
{
	Test_constants();
	Test_lerp_and_smoothstep();
	return Math_test::Result("Test_scalar");
}
