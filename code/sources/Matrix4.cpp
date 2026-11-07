#include <cassert>
#include <Math/Matrix4.hpp>
#include "Glm_convert.hpp"

namespace Math
{
	Matrix4 Inverse(const Matrix4& matrix) noexcept
	{
		assert(Is_invertible(matrix) && "Inverse: the matrix must have a normal (finite, non-zero) determinant");
		return detail::From_glm(glm::inverse(detail::To_glm(matrix)));
	}
}
