#pragma once

#include <cstddef>
#include <type_traits>
#include <Math/Vector3.hpp>

namespace Math
{
	struct Matrix3
	{
		Vector3 columns[3];


		constexpr Matrix3() noexcept: Matrix3(Vector3(1.0f, 0.0f, 0.0f),Vector3(0.0f, 1.0f, 0.0f),Vector3(0.0f, 0.0f, 1.0f)) {}

		constexpr Matrix3(const Vector3 _column0, const Vector3 _column1, const Vector3 _column2) noexcept : columns{ _column0, _column1, _column2 } {}

		[[nodiscard]] constexpr Vector3& operator[](std::size_t index) noexcept
		{
		    assert(index < 3 && "operator[]: index must be less than 3");
			return index == 0 ? columns[0] : index == 1 ? columns[1] : columns[2];
		}
		[[nodiscard]] constexpr const Vector3& operator[](std::size_t index) const noexcept
		{
		    assert(index < 3 && "operator[]: index must be less than 3");
			return index == 0 ? columns[0] : index == 1 ? columns[1] : columns[2];
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &columns[0].x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &columns[0].x; }

		[[nodiscard]] constexpr static Matrix3 Identity() noexcept { return Matrix3();}

		[[nodiscard]] constexpr static Matrix3 Zero() noexcept { return Matrix3(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)); }

		[[nodiscard]] constexpr static Matrix3 Scale(const Vector3 scale) noexcept
		{
			return Matrix3(Vector3(scale.x, 0.0f, 0.0f), Vector3(0.0f, scale.y, 0.0f), Vector3(0.0f, 0.0f, scale.z));
		}

		[[nodiscard]] static Matrix3 Rotation_x(const Radians angle) noexcept
		{
			const float cosine = std::cos(angle.value);
			const float sine = std::sin(angle.value);
			return Matrix3(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, cosine, sine), Vector3(0.0f, -sine, cosine));
		}

		[[nodiscard]] static Matrix3 Rotation_y(const Radians angle) noexcept
		{
			const float cosine = std::cos(angle.value);
			const float sine = std::sin(angle.value);
			return Matrix3(Vector3(cosine, 0.0f, -sine), Vector3(0.0f, 1.0f, 0.0f), Vector3(sine, 0.0f, cosine));
		}

		[[nodiscard]] static Matrix3 Rotation_z(const Radians angle) noexcept
		{
			const float cosine = std::cos(angle.value);
			const float sine = std::sin(angle.value);
			return Matrix3(Vector3(cosine, sine, 0.0f), Vector3(-sine, cosine, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
		}

		[[nodiscard]] static Matrix3 Rotation(const Vector3 axis, const Radians angle) noexcept
		{
			assert(Is_normalized(axis) && "Rotation: 'axis' must be a normalized vector");
			const float cosine = std::cos(angle.value);
			const float sine = std::sin(angle.value);
			const float one_minus_cosine = 1.0f - cosine;
			const float xy = axis.x * axis.y * one_minus_cosine;
			const float xz = axis.x * axis.z * one_minus_cosine;
			const float yz = axis.y * axis.z * one_minus_cosine;
			const float x_sine = axis.x * sine;
			const float y_sine = axis.y * sine;
			const float z_sine = axis.z * sine;
			return Matrix3(Vector3(axis.x * axis.x * one_minus_cosine + cosine, xy + z_sine, xz - y_sine),
						   Vector3(xy - z_sine, axis.y * axis.y * one_minus_cosine + cosine, yz + x_sine),
						   Vector3(xz + y_sine, yz - x_sine, axis.z * axis.z * one_minus_cosine + cosine));
		}


		[[nodiscard]] friend constexpr bool operator==(const Matrix3& matrix, const Matrix3& other_matrix)  noexcept = default;

		[[nodiscard]] constexpr Matrix3 operator-() const noexcept
		{return Matrix3(-columns[0], -columns[1],-columns[2]);}
		constexpr Matrix3& operator+=(const Matrix3& other_matrix) noexcept
		{
    		columns[0]+=other_matrix.columns[0];
    		columns[1]+=other_matrix.columns[1];
            columns[2]+=other_matrix.columns[2];
    		return *this;
		}
		constexpr Matrix3& operator-=(const Matrix3& other_matrix) noexcept
		{
		    columns[0]-=other_matrix.columns[0];
    		columns[1]-=other_matrix.columns[1];
            columns[2]-=other_matrix.columns[2];
    		return *this;
		}
		constexpr Matrix3& operator*=(const Matrix3& other_matrix) noexcept
		{
        	const Vector3 column0 = columns[0] * other_matrix.columns[0].x + columns[1] * other_matrix.columns[0].y + columns[2] * other_matrix.columns[0].z;
        	const Vector3 column1 = columns[0] * other_matrix.columns[1].x + columns[1] * other_matrix.columns[1].y + columns[2] * other_matrix.columns[1].z;
        	const Vector3 column2 = columns[0] * other_matrix.columns[2].x + columns[1] * other_matrix.columns[2].y + columns[2] * other_matrix.columns[2].z;
        	columns[0] = column0;
        	columns[1] = column1;
        	columns[2] = column2;
        	return *this;
		}
		Matrix3& operator/=(const Matrix3& other_matrix) noexcept;
		constexpr Matrix3& operator+=(const float scalar) noexcept
		{
		    columns[0]+= scalar;
    		columns[1]+= scalar;
            columns[2]+= scalar;
    		return *this;
		}
		constexpr Matrix3& operator-=(const float scalar) noexcept
		{
		    columns[0]-= scalar;
    		columns[1]-= scalar;
            columns[2]-= scalar;
    		return *this;
	    }
		constexpr Matrix3& operator*=(const float scalar) noexcept
		{
		    columns[0]*= scalar;
    		columns[1]*= scalar;
            columns[2]*= scalar;
    		return *this;
		}
		constexpr Matrix3& operator/=(const float scalar) noexcept
		{
		    columns[0]/= scalar;
    		columns[1]/= scalar;
            columns[2]/= scalar;
        	return *this;
	    }

		[[nodiscard]] friend constexpr Matrix3 operator+( Matrix3 matrix_a, const Matrix3& matrix_b) noexcept
		{ return matrix_a+=matrix_b; }
		[[nodiscard]] friend constexpr Matrix3 operator-(Matrix3 matrix_a, const Matrix3& matrix_b) noexcept
		{ return  matrix_a-=matrix_b; }
		[[nodiscard]] friend constexpr Matrix3 operator*( Matrix3 matrix_a, const Matrix3& matrix_b) noexcept
		{ return matrix_a*=matrix_b; }
		[[nodiscard]] friend Matrix3 operator/(const Matrix3& matrix_a, const Matrix3& matrix_b) noexcept
		{
			Matrix3 result = matrix_a;
			result /= matrix_b;
			return result;
		}
		[[nodiscard]] friend constexpr Matrix3 operator+(Matrix3 matrix,const float scalar) noexcept
		{ return  matrix+=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator+(const float scalar, Matrix3 matrix) noexcept
		{ return  matrix+=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator-(Matrix3 matrix,const float scalar) noexcept
		{ return  matrix-=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator-(const float scalar, Matrix3 matrix) noexcept
		{ return Matrix3(scalar - matrix.columns[0], scalar - matrix.columns[1], scalar - matrix.columns[2]);}
		[[nodiscard]] friend constexpr Matrix3 operator*(Matrix3 matrix, const float scalar) noexcept
		{ return  matrix*=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator*(const float scalar, Matrix3 matrix) noexcept
		{ return  matrix*=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator/(Matrix3 matrix,const float scalar) noexcept
		{ return  matrix/=scalar; }
		[[nodiscard]] friend constexpr Matrix3 operator/(const float scalar, Matrix3 matrix) noexcept
		{ return Matrix3(scalar/matrix.columns[0],scalar/matrix.columns[1],scalar/matrix.columns[2]); }


	};

	[[nodiscard]] constexpr float Determinant(const Matrix3& matrix) noexcept
	{
		return Dot(matrix[0], Cross(matrix[1], matrix[2]));
	}

	[[nodiscard]] inline bool Is_invertible(const Matrix3& matrix) noexcept
	{
	    return std::isnormal(Determinant(matrix));
	}
	[[nodiscard]] inline bool Is_singular(const Matrix3& matrix) noexcept
	{
		return !Is_invertible(matrix);
	}
	[[nodiscard]] Matrix3 Inverse(const Matrix3& matrix) noexcept;

	inline Matrix3& Matrix3::operator/=(const Matrix3& other_matrix) noexcept
	{
		return *this *= Inverse(other_matrix);
	}

	[[nodiscard]]  inline std::optional<Matrix3> Try_inverse(const Matrix3& matrix) noexcept
	{
	    if (!Is_invertible(matrix))
	    {
			return std::nullopt;
		}
		return Inverse(matrix);
	}

	[[nodiscard]] inline Matrix3 Inverse_or(const Matrix3& matrix, const Matrix3& fallback) noexcept
	{
		return Try_inverse(matrix).value_or(fallback);
	}


	[[nodiscard]] constexpr Matrix3 Transpose(const Matrix3& matrix) noexcept
	{
		return Matrix3(Vector3(matrix[0].x, matrix[1].x, matrix[2].x),
					   Vector3(matrix[0].y, matrix[1].y, matrix[2].y),
					   Vector3(matrix[0].z, matrix[1].z, matrix[2].z));
	}


	[[nodiscard]] constexpr bool Equals(const Matrix3& matrix_a, const Matrix3& matrix_b) noexcept
	{
		return Equals(matrix_a[0], matrix_b[0]) && Equals(matrix_a[1], matrix_b[1]) && Equals(matrix_a[2], matrix_b[2]);
	}

	[[nodiscard]] constexpr bool Equals(const Matrix3& matrix_a, const Matrix3& matrix_b, const float absolute_tolerance, const float relative_tolerance) noexcept
	{
		return Equals(matrix_a[0], matrix_b[0], absolute_tolerance, relative_tolerance) &&
			   Equals(matrix_a[1], matrix_b[1], absolute_tolerance, relative_tolerance) &&
			   Equals(matrix_a[2], matrix_b[2], absolute_tolerance, relative_tolerance);
	}
	[[nodiscard]] constexpr bool Is_identity(const Matrix3& matrix) noexcept
	{
		return Equals(matrix, Matrix3::Identity());
	}

	[[nodiscard]] constexpr bool Is_identity(const Matrix3& matrix, const float absolute_tolerance, const float relative_tolerance) noexcept
	{
		return Equals(matrix, Matrix3::Identity(), absolute_tolerance, relative_tolerance);
	}

	[[nodiscard]] constexpr bool Is_orthonormal(const Matrix3& matrix, const float tolerance) noexcept
	{
		return Is_normalized(matrix[0], tolerance) && Is_normalized(matrix[1], tolerance) && Is_normalized(matrix[2], tolerance) &&
			   Equals(Dot(matrix[0], matrix[1]), 0.0f, tolerance, 0.0f) && Equals(Dot(matrix[0], matrix[2]), 0.0f, tolerance, 0.0f) &&
			   Equals(Dot(matrix[1], matrix[2]), 0.0f, tolerance, 0.0f);
	}


	[[nodiscard]] constexpr bool Is_orthonormal(const Matrix3& matrix) noexcept
	{
		return Is_orthonormal(matrix, Epsilon_relative);
	}


	[[nodiscard]] constexpr Matrix3 Normal_matrix(const Matrix3& matrix) noexcept
	{
		const float sign = Determinant(matrix) < 0.0f ? -1.0f : 1.0f;
		return Matrix3(Cross(matrix[1], matrix[2]) * sign,
					   Cross(matrix[2], matrix[0]) * sign,
					   Cross(matrix[0], matrix[1]) * sign);
	}


	[[nodiscard]] inline Vector3 Extract_scale(const Matrix3& matrix) noexcept
	{
		return Vector3(Length(matrix[0]), Length(matrix[1]), Length(matrix[2]));
	}


	[[nodiscard]] inline Matrix3 Extract_rotation(const Matrix3& matrix) noexcept
	{
		assert(Is_normalizable(matrix[0]) && Is_normalizable(matrix[1]) && Is_normalizable(matrix[2]) && "Extract_rotation: no column can be zero or have infinite or NaN values");
		return Matrix3(Normalize(matrix[0]), Normalize(matrix[1]), Normalize(matrix[2]));
	}


	[[nodiscard]] constexpr Vector3 Right(const Matrix3& matrix) noexcept { return matrix[0]; }
	[[nodiscard]] constexpr Vector3 Up(const Matrix3& matrix) noexcept { return matrix[1]; }
	[[nodiscard]] constexpr Vector3 Forward(const Matrix3& matrix) noexcept { return -matrix[2]; }

	static_assert(sizeof(Matrix3) == 3 * sizeof(Vector3), "Matrix3 must be exactly 3 Vector3 in size");
	static_assert(alignof(Matrix3) == alignof(Vector3), "Matrix3 must not be aligned more strictly than a Vector3s");
	static_assert(std::is_standard_layout_v<Matrix3>, "Matrix3 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix3>, "Matrix3 must be trivially_copyable");
	static_assert(offsetof(Matrix3, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix3, columns[1]) == sizeof(Vector3), "colum[1] must follow colum[0]");
	static_assert(offsetof(Matrix3, columns[2]) == 2 * sizeof(Vector3), "colum[2] must follow colum[1]");
}
