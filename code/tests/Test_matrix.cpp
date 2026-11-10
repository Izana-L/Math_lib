#include "Check.hpp"
#include "Glm_convert.hpp"
#include <Math/Math.hpp>

namespace
{
	Math::Matrix4 Random_matrix4()
	{
		Math::Matrix4 matrix = Math::Matrix4::Zero();
		for (int i = 0; i < 16; ++i)
		{
			matrix.Data()[i] = Math_test::Random_float(-1.0f, 1.0f);
		}
		return matrix;
	}

	void Test_against_glm()
	{
		using Math::detail::From_glm;
		using Math::detail::To_glm;

		for (int i = 0; i < 1000; ++i)
		{
			const Math::Matrix4 a = Random_matrix4();
			const Math::Matrix4 b = Random_matrix4();

			CHECK(Math::Equals(a * b, From_glm(To_glm(a) * To_glm(b))));
			CHECK(Math::Equals(Math::Transpose(a), From_glm(glm::transpose(To_glm(a)))));
			CHECK(Math::Equals(Math::Determinant(a), glm::determinant(To_glm(a)), 1e-5f, 1e-4f));
		}
	}

	// Invariante: M * M^-1 = I.
	void Test_inverse()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Math::Matrix4 matrix = Math::Matrix4::TRS(
				Math::Vector3(Math_test::Random_float(-10.0f, 10.0f), Math_test::Random_float(-10.0f, 10.0f), Math_test::Random_float(-10.0f, 10.0f)),
				Math::Quaternion::From_axis_angle(Math::Vector3(0.0f, 1.0f, 0.0f), Math::Radians(Math_test::Random_float(-3.0f, 3.0f))),
				Math::Vector3(Math_test::Random_float(0.5f, 2.0f), Math_test::Random_float(0.5f, 2.0f), Math_test::Random_float(0.5f, 2.0f)));

			CHECK(Math::Is_identity(matrix * Math::Inverse(matrix), 1e-5f, 1e-5f));
			CHECK(Math::Equals(Math::Inverse_affine(matrix), Math::Inverse(matrix), 1e-5f, 1e-5f));
		}

		CHECK(!Math::Try_inverse(Math::Matrix4::Zero()).has_value());
	}
}

int main()
{
	Test_against_glm();
	Test_inverse();
	return Math_test::Result("Test_matrix");
}
