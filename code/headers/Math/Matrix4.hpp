#pragma once

#include <cstddef>
#include <type_traits>
#include <Math/Vector4.hpp>
#include <Math/Matrix3.hpp>
#include <Math/Quaternion.hpp>
namespace Math
{
	struct Matrix4
	{
		Vector4 columns[4];


		constexpr Matrix4() noexcept : Matrix4(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
											   Vector4(0.0f, 1.0f, 0.0f, 0.0f),
											   Vector4(0.0f, 0.0f, 1.0f, 0.0f),
											   Vector4(0.0f, 0.0f, 0.0f, 1.0f)) {}

		constexpr Matrix4(const Vector4 _column0, const Vector4 _column1, const Vector4 _column2, const Vector4 _column3) noexcept : columns{ _column0, _column1, _column2,_column3 } {}

		constexpr Matrix4(const Matrix3& linear, const Vector3 translation) noexcept
			: columns{ Vector4(linear[0], 0.0f), Vector4(linear[1], 0.0f), Vector4(linear[2], 0.0f), Vector4(translation, 1.0f) } {}
		[[nodiscard]] constexpr Vector4& operator[](std::size_t index) noexcept
		{
		    assert(index < 4 && "operator[]: index must be less than 4");
			return index == 0 ? columns[0] : index == 1 ? columns[1] : index == 2 ? columns[2] : columns[3];
		}
		[[nodiscard]] constexpr const Vector4& operator[](std::size_t index) const noexcept
		{
		    assert(index < 4 && "operator[]: index must be less than 4");
			return index == 0 ? columns[0] : index == 1 ? columns[1] : index == 2 ? columns[2] : columns[3];
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &columns[0].x; }
		[[nodiscard]] constexpr Matrix3 Upper_left() const noexcept
		{
			return Matrix3(columns[0].Xyz(), columns[1].Xyz(), columns[2].Xyz());
		}
		[[nodiscard]] constexpr const float* Data() const noexcept { return &columns[0].x; }

		[[nodiscard]] constexpr static Matrix4 Identity() noexcept { return Matrix4(); }

		[[nodiscard]] constexpr static Matrix4 Zero() noexcept { return Matrix4(Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																	            Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																				Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																				Vector4(0.0f, 0.0f, 0.0f, 0.0f));}

		[[nodiscard]] constexpr static Matrix4 Translation(const Vector3 translation) noexcept
		{
			return Matrix4(Matrix3(), translation);
		}
		[[nodiscard]] constexpr static Matrix4 TRS(const Vector3 translation, const Quaternion rotation, const Vector3 scale) noexcept
		{
			assert(Equals(rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w, 1.0f) && "TRS: 'rotation' must be a normalized quaternion");
			const float xx = rotation.x * rotation.x;
			const float yy = rotation.y * rotation.y;
			const float zz = rotation.z * rotation.z;
			const float xy = rotation.x * rotation.y;
			const float xz = rotation.x * rotation.z;
			const float yz = rotation.y * rotation.z;
			const float wx = rotation.w * rotation.x;
			const float wy = rotation.w * rotation.y;
			const float wz = rotation.w * rotation.z;
			return Matrix4(Matrix3(Vector3(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy)) * scale.x,
								   Vector3(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx)) * scale.y,
								   Vector3(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy)) * scale.z),
						   translation);
		}

		[[nodiscard]] friend constexpr bool operator==(const Matrix4& vector, const Matrix4& other_vector)  noexcept = default;

		[[nodiscard]] constexpr Matrix4 operator-() const noexcept
		{return Matrix4(-columns[0], -columns[1],-columns[2],-columns[3]);}
		constexpr Matrix4& operator+=(const Matrix4 other_matrix) noexcept
		{
    		columns[0]+=other_matrix.columns[0];
    		columns[1]+=other_matrix.columns[1];
            columns[2]+=other_matrix.columns[2];
            columns[3]+=other_matrix.columns[3];
    		return *this;
		}
		constexpr Matrix4& operator-=(const Matrix4 other_matrix) noexcept
		{
		    columns[0]-=other_matrix.columns[0];
    		columns[1]-=other_matrix.columns[1];
            columns[2]-=other_matrix.columns[2];
            columns[3]-=other_matrix.columns[3];
    		return *this;
		}
		constexpr Matrix4& operator*=(const Matrix4& other_matrix) noexcept
		{
        	const Vector4 column0 = columns[0] * other_matrix.columns[0].x + columns[1] * other_matrix.columns[0].y + columns[2] * other_matrix.columns[0].z + columns[3] * other_matrix.columns[0].w;
        	const Vector4 column1 = columns[0] * other_matrix.columns[1].x + columns[1] * other_matrix.columns[1].y + columns[2] * other_matrix.columns[1].z + columns[3] * other_matrix.columns[1].w;
        	const Vector4 column2 = columns[0] * other_matrix.columns[2].x + columns[1] * other_matrix.columns[2].y + columns[2] * other_matrix.columns[2].z + columns[3] * other_matrix.columns[2].w;
        	const Vector4 column3 = columns[0] * other_matrix.columns[3].x + columns[1] * other_matrix.columns[3].y + columns[2] * other_matrix.columns[3].z + columns[3] * other_matrix.columns[3].w;
        	columns[0] = column0;
        	columns[1] = column1;
        	columns[2] = column2;
        	columns[3] = column3;
        	return *this;
		}
		Matrix4& operator/=(const Matrix4& other_matrix) noexcept;
		constexpr Matrix4& operator+=(const float scalar) noexcept
		{
		    columns[0]+= scalar;
    		columns[1]+= scalar;
            columns[2]+= scalar;
            columns[3]+= scalar;
    		return *this;
		}
		constexpr Matrix4& operator-=(const float scalar) noexcept
		{
		    columns[0]-= scalar;
    		columns[1]-= scalar;
            columns[2]-= scalar;
            columns[3]-= scalar;
    		return *this;
	    }
		constexpr Matrix4& operator*=(const float scalar) noexcept
		{
		    columns[0]*= scalar;
    		columns[1]*= scalar;
            columns[2]*= scalar;
            columns[3]*= scalar;
    		return *this;
		}
		constexpr Matrix4& operator/=(const float scalar) noexcept
		{
		    columns[0]/= scalar;
    		columns[1]/= scalar;
            columns[2]/= scalar;
            columns[3]/= scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Matrix4 operator+( Matrix4 matrix_a, const Matrix4 matrix_b) noexcept
		{ return matrix_a+=matrix_b; }
		[[nodiscard]] friend constexpr Matrix4 operator-(Matrix4 matrix_a, const Matrix4 matrix_b) noexcept
		{ return  matrix_a-=matrix_b; }
		[[nodiscard]] friend constexpr Matrix4 operator*( Matrix4 matrix_a, const Matrix4 matrix_b) noexcept
		{ return matrix_a*=matrix_b; }
		[[nodiscard]] friend Matrix4 operator/(const Matrix4& matrix_a, const Matrix4& matrix_b) noexcept
		{
			Matrix4 result = matrix_a;
			result /= matrix_b;
			return result;
		}
		[[nodiscard]] friend constexpr Matrix4 operator+(Matrix4 matrix,const float scalar) noexcept
		{ return  matrix+=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator+(const float scalar, Matrix4 matrix) noexcept
		{ return  matrix+=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator-(Matrix4 matrix,const float scalar) noexcept
		{ return  matrix-=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator-(const float scalar, Matrix4 matrix) noexcept
		{ return Matrix4(scalar - matrix.columns[0], scalar - matrix.columns[1], scalar - matrix.columns[2], scalar - matrix.columns[3]);}
		[[nodiscard]] friend constexpr Matrix4 operator*(Matrix4 matrix, const float scalar) noexcept
		{ return  matrix*=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator*(const float scalar, Matrix4 matrix) noexcept
		{ return  matrix*=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator/(Matrix4 matrix,const float scalar) noexcept
		{ return  matrix/=scalar; }
		[[nodiscard]] friend constexpr Matrix4 operator/(const float scalar, Matrix4 matrix) noexcept
		{ return Matrix4(scalar/matrix.columns[0],scalar/matrix.columns[1],scalar/matrix.columns[2],scalar/matrix.columns[3]); }

	};

	[[nodiscard]] constexpr Vector3 Right(const Matrix4& matrix) noexcept { return matrix[0].Xyz(); }
	[[nodiscard]] constexpr Vector3 Up(const Matrix4& matrix) noexcept { return matrix[1].Xyz(); }
	[[nodiscard]] constexpr Vector3 Forward(const Matrix4& matrix) noexcept { return -matrix[2].Xyz(); }

	[[nodiscard]] constexpr float Determinant(const Matrix4& matrix) noexcept
	{
		const Vector4 column0 = matrix[0];
		const Vector4 column1 = matrix[1];
		const Vector4 column2 = matrix[2];
		const Vector4 column3 = matrix[3];

		const float left_xy = column0.x * column1.y - column0.y * column1.x;
		const float left_xz = column0.x * column1.z - column0.z * column1.x;
		const float left_xw = column0.x * column1.w - column0.w * column1.x;
		const float left_yz = column0.y * column1.z - column0.z * column1.y;
		const float left_yw = column0.y * column1.w - column0.w * column1.y;
		const float left_zw = column0.z * column1.w - column0.w * column1.z;

		const float right_xy = column2.x * column3.y - column2.y * column3.x;
		const float right_xz = column2.x * column3.z - column2.z * column3.x;
		const float right_xw = column2.x * column3.w - column2.w * column3.x;
		const float right_yz = column2.y * column3.z - column2.z * column3.y;
		const float right_yw = column2.y * column3.w - column2.w * column3.y;
		const float right_zw = column2.z * column3.w - column2.w * column3.z;

		return left_xy * right_zw - left_xz * right_yw + left_xw * right_yz + left_yz * right_xw - left_yw * right_xz + left_zw * right_xy;
	}

	[[nodiscard]] inline bool Is_invertible(const Matrix4& matrix) noexcept
	{
	    return std::isnormal(Determinant(matrix));
	}

	[[nodiscard]] Matrix4 Inverse(const Matrix4& matrix) noexcept;
	inline Matrix4& Matrix4::operator/=(const Matrix4& other_matrix) noexcept
	{
		return *this *= Inverse(other_matrix);
	}
	[[nodiscard]] inline std::optional<Matrix4> Try_inverse(const Matrix4& matrix) noexcept
	{
	    if (!Is_invertible(matrix))
	    {
			return std::nullopt;
		}
		return Inverse(matrix);
	}

	[[nodiscard]] inline Matrix4 Inverse_or(const Matrix4& matrix, const Matrix4 fallback) noexcept
	{
		return Try_inverse(matrix).value_or(fallback);
	}

	[[nodiscard]] constexpr Matrix4 Transpose(const Matrix4& matrix) noexcept
	{
		return Matrix4(Vector4(matrix[0].x, matrix[1].x, matrix[2].x, matrix[3].x),
					   Vector4(matrix[0].y, matrix[1].y, matrix[2].y, matrix[3].y),
					   Vector4(matrix[0].z, matrix[1].z, matrix[2].z, matrix[3].z),
					   Vector4(matrix[0].w, matrix[1].w, matrix[2].w, matrix[3].w));
	}

	[[nodiscard]] constexpr bool Equals(const Matrix4& matrix_a, const Matrix4& matrix_b) noexcept
	{
		return Equals(matrix_a[0], matrix_b[0]) && Equals(matrix_a[1], matrix_b[1]) &&
			   Equals(matrix_a[2], matrix_b[2]) && Equals(matrix_a[3], matrix_b[3]);
	}

	[[nodiscard]] constexpr bool Equals(const Matrix4& matrix_a, const Matrix4& matrix_b, const float absolute_tolerance, const float relative_tolerance) noexcept
	{
		return Equals(matrix_a[0], matrix_b[0], absolute_tolerance, relative_tolerance) &&
			   Equals(matrix_a[1], matrix_b[1], absolute_tolerance, relative_tolerance) &&
			   Equals(matrix_a[2], matrix_b[2], absolute_tolerance, relative_tolerance) &&
			   Equals(matrix_a[3], matrix_b[3], absolute_tolerance, relative_tolerance);
	}

	[[nodiscard]] constexpr bool Is_orthonormal(const Matrix4& matrix) noexcept
	{
		return Is_orthonormal(matrix.Upper_left());
	}

	[[nodiscard]] constexpr bool Is_orthonormal(const Matrix4& matrix, const float tolerance) noexcept
	{
		return Is_orthonormal(matrix.Upper_left(), tolerance);
	}

	[[nodiscard]] constexpr Matrix3 Normal_matrix(const Matrix4& matrix) noexcept
	{
		return Normal_matrix(matrix.Upper_left());
	}

	[[nodiscard]] inline Vector3 Extract_scale(const Matrix4& matrix) noexcept
	{
		return Extract_scale(matrix.Upper_left());
	}

	[[nodiscard]] inline Matrix3 Extract_rotation(const Matrix4& matrix) noexcept
	{
		return Extract_rotation(matrix.Upper_left());
	}

	[[nodiscard]] constexpr Vector3 Extract_translation(const Matrix4& matrix) noexcept
	{
		return matrix[3].Xyz();
	}

	[[nodiscard]] constexpr Vector3 Transform_point(const Matrix4& matrix, const Vector3 point) noexcept
	{
		return matrix[0].Xyz() * point.x + matrix[1].Xyz() * point.y + matrix[2].Xyz() * point.z + matrix[3].Xyz();
	}

	[[nodiscard]] constexpr Vector3 Transform_direction(const Matrix4& matrix, const Vector3 direction) noexcept
	{
		return matrix[0].Xyz() * direction.x + matrix[1].Xyz() * direction.y + matrix[2].Xyz() * direction.z;
	}

	[[nodiscard]] constexpr bool Is_affine(const Matrix4& matrix, const float tolerance) noexcept
	{
		return Equals(matrix[0].w, 0.0f, tolerance, 0.0f) && Equals(matrix[1].w, 0.0f, tolerance, 0.0f) &&
			   Equals(matrix[2].w, 0.0f, tolerance, 0.0f) && Equals(matrix[3].w, 1.0f, tolerance, 0.0f);
	}

	[[nodiscard]] constexpr bool Is_affine(const Matrix4& matrix) noexcept
	{
		return Is_affine(matrix, Epsilon_relative);
	}

	[[nodiscard]] inline Matrix4 Inverse_affine(const Matrix4& matrix) noexcept
	{
		assert(Is_affine(matrix) && "Inverse_affine: the matrix must be affine (last row 0, 0, 0, 1)");
		const Matrix3 linear = matrix.Upper_left();
		assert(Is_invertible(linear) && "Inverse_affine: the 3x3 block must have a normal (finite, non-zero) determinant");
		const Matrix3 linear_inverse = Inverse(linear);
		const Vector3 translation = Extract_translation(matrix);
		return Matrix4(linear_inverse, -(linear_inverse[0] * translation.x + linear_inverse[1] * translation.y + linear_inverse[2] * translation.z));
	}


	[[nodiscard]] constexpr Matrix4 Inverse_rigid(const Matrix4& matrix) noexcept
	{
		assert(Is_affine(matrix) && "Inverse_rigid: the matrix must be affine (last row 0, 0, 0, 1)");
		assert(Is_orthonormal(matrix) && "Inverse_rigid: the 3x3 block must be orthonormal (no scale or shear)");
		const Matrix3 rotation = matrix.Upper_left();
		const Vector3 translation = Extract_translation(matrix);
		return Matrix4(Transpose(rotation), Vector3(-Dot(rotation[0], translation), -Dot(rotation[1], translation), -Dot(rotation[2], translation)));
	}

	struct Decomposition
	{
		Vector3 translation;
		Quaternion rotation;
		Vector3 scale;
	};


	[[nodiscard]] inline Decomposition Decompose(const Matrix4& matrix) noexcept
	{
		assert(Is_affine(matrix) && "Decompose: the matrix must be affine (last row 0, 0, 0, 1)");
		const Matrix3 linear = matrix.Upper_left();
		assert(Is_normalizable(linear[0]) && Is_normalizable(linear[1]) && Is_normalizable(linear[2]) && "Decompose: no column of the 3x3 block can be zero or have infinite or NaN values");
		Vector3 scale = Extract_scale(linear);
		Matrix3 rotation = Extract_rotation(linear);
		if (Determinant(linear) < 0.0f)
		{
			scale = -scale;
			rotation = -rotation;
		}
		assert(Is_orthonormal(rotation) && "Decompose: the matrix must not have shear (the columns of the 3x3 block must be perpendicular)");
		return Decomposition{ Extract_translation(matrix), Quaternion::From_matrix3(rotation), scale };
	}




	static_assert(sizeof(Matrix4) == 4 * sizeof(Vector4), "Matrix4 must be exactly 4 Vector4 in size");
	static_assert(alignof(Matrix4) == alignof(Vector4), "Matrix4 must not be aligned more strictly than a Vector4s");
	static_assert(std::is_standard_layout_v<Matrix4>, "Matrix4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix4>, "Matrix4 must be trivially_copyable");
	static_assert(offsetof(Matrix4, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix4, columns[1]) == sizeof(Vector4), "colum[1] must follow colum[0]");
	static_assert(offsetof(Matrix4, columns[2]) == 2 * sizeof(Vector4), "colum[2] must follow colum[1]");
	static_assert(offsetof(Matrix4, columns[3]) == 3 * sizeof(Vector4), "colum[3] must follow colum[2]");

	static_assert(sizeof(Decomposition) == 10 * sizeof(float), "Decomposition must be exactly 10 float in size");
	static_assert(alignof(Decomposition) == alignof(float), "Decomposition must not be aligned more strictly than a float");
	static_assert(std::is_standard_layout_v<Decomposition>, "Decomposition must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Decomposition>, "Decomposition must be trivially_copyable");
	static_assert(offsetof(Decomposition, translation) == 0, "translation must be at offset 0");
	static_assert(offsetof(Decomposition, rotation) == 3 * sizeof(float), "rotation must follow translation");
	static_assert(offsetof(Decomposition, scale) == 7 * sizeof(float), "scale must follow rotation");
}
