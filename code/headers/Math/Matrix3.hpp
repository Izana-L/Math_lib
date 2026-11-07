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
		[[nodiscard]] friend constexpr bool operator==(const Matrix3& vector, const Matrix3& other_vector)  noexcept = default;

		[[nodiscard]] constexpr Matrix3 operator-() const noexcept
		{return Matrix3(-columns[0], -columns[1],-columns[2]);}
		constexpr Matrix3& operator+=(const Matrix3 other_matrix) noexcept
		{
    		columns[0]+=other_matrix.columns[0];
    		columns[1]+=other_matrix.columns[1];
            columns[2]+=other_matrix.columns[2];
    		return *this;
		}
		constexpr Matrix3& operator-=(const Matrix3 other_matrix) noexcept
		{
		    columns[0]-=other_matrix.columns[0];
    		columns[1]-=other_matrix.columns[1];
            columns[2]-=other_matrix.columns[2];
    		return *this;
		}
		constexpr Matrix3& operator*=(const Matrix3& other_matrix) noexcept
		{
			columns[0] = columns[0] * other_matrix.columns[0].x + columns[1] * other_matrix.columns[0].y + columns[2] * other_matrix.columns[0].z;
			columns[1] = columns[0] * other_matrix.columns[1].x + columns[1] * other_matrix.columns[1].y + columns[2] * other_matrix.columns[1].z;
			columns[2] = columns[0] * other_matrix.columns[2].x + columns[1] * other_matrix.columns[2].y + columns[2] * other_matrix.columns[2].z;
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

		[[nodiscard]] friend constexpr Matrix3 operator+( Matrix3 matrix_a, const Matrix3 matrix_b) noexcept
		{ return matrix_a+=matrix_b; }
		[[nodiscard]] friend constexpr Matrix3 operator-(Matrix3 matrix_a, const Matrix3 matrix_b) noexcept
		{ return  matrix_a-=matrix_b; }
		[[nodiscard]] friend constexpr Matrix3 operator*( Matrix3 matrix_a, const Matrix3 matrix_b) noexcept
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

	[[nodiscard]] inline bool Is_invertible(const Matrix3 matrix) noexcept
	{
	    return std::isnormal(Determinant(matrix));
	}

	[[nodiscard]] Matrix3 Inverse(const Matrix3 matrix) noexcept;

	inline Matrix3& Matrix3::operator/=(const Matrix3& other_matrix) noexcept
	{
		return *this *= Inverse(other_matrix);
	}

	[[nodiscard]]  inline std::optional<Matrix3> Try_inverse(const Matrix3 matrix) noexcept
	{
	    if (!Is_invertible(matrix))
	    {
			return std::nullopt;
		}
		return Inverse(matrix);
	}
	template <std::floating_point Type>
	[[nodiscard]] Matrix3 Inverse_or(const Matrix3 matrix, const Matrix3 fallback) noexcept
	{
		return Try_inverse(matrix).value_or(fallback);
	}


	static_assert(sizeof(Matrix3) == 3 * sizeof(Vector3), "Matrix3 must be exactly 3 Vector3 in size");
	static_assert(alignof(Matrix3) == alignof(Vector3), "Matrix3 must not be aligned more strictly than a Vector3s");
	static_assert(std::is_standard_layout_v<Matrix3>, "Matrix3 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix3>, "Matrix3 must be trivially_copyable");
	static_assert(offsetof(Matrix3, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix3, columns[1]) == sizeof(Vector3), "colum[1] must follow colum[0]");
	static_assert(offsetof(Matrix3, columns[2]) == 2 * sizeof(Vector3), "colum[2] must follow colum[1]");
}
