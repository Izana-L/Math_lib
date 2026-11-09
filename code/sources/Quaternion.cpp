#include <cassert>
#include <Math/Matrix3.hpp>
#include <Math/Matrix4.hpp>
#include <Math/Quaternion.hpp>
#include "Glm_convert.hpp"

namespace Math
{
	Quaternion Quaternion::From_matrix3(const Matrix3& matrix) noexcept
	{
		assert(Is_orthonormal(matrix) && Determinant(matrix) > 0.0f && "From_matrix3: the matrix must be a rotation (orthonormal, determinant +1)");
		return detail::From_glm(glm::normalize(glm::quat_cast(detail::To_glm(matrix))));
	}

	Quaternion Quaternion::From_matrix4(const Matrix4& matrix) noexcept
	{
		const Matrix3 rotation = matrix.Upper_left();
		assert(Is_orthonormal(rotation) && Determinant(rotation) > 0.0f && "From_matrix4: the 3x3 block must be a rotation (orthonormal, determinant +1)");
		return From_matrix3(rotation);
	}
}
