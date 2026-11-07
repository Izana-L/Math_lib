#pragma once

#include <cstddef>
#include <type_traits>
#include <Math/Vector4.hpp>

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
		[[nodiscard]] constexpr const float* Data() const noexcept { return &columns[0].x; }

		[[nodiscard]] constexpr static Matrix4 Identity() noexcept { return Matrix4(); }

		[[nodiscard]] constexpr static Matrix4 Zero() noexcept { return Matrix4(Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																	            Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																				Vector4(0.0f, 0.0f, 0.0f, 0.0f),
																				Vector4(0.0f, 0.0f, 0.0f, 0.0f));}
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

		return left_xy * right_zw - left_xz * right_yw + left_xw * right_yz +
			left_yz * right_xw - left_yw * right_xz + left_zw * right_xy;
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


	static_assert(sizeof(Matrix4) == 4 * sizeof(Vector4), "Matrix4 must be exactly 4 Vector4 in size");
	static_assert(alignof(Matrix4) == alignof(Vector4), "Matrix4 must not be aligned more strictly than a Vector4s");
	static_assert(std::is_standard_layout_v<Matrix4>, "Matrix4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix4>, "Matrix4 must be trivially_copyable");
	static_assert(offsetof(Matrix4, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix4, columns[1]) == sizeof(Vector4), "colum[1] must follow colum[0]");
	static_assert(offsetof(Matrix4, columns[2]) == 2 * sizeof(Vector4), "colum[2] must follow colum[1]");
	static_assert(offsetof(Matrix4, columns[3]) == 3 * sizeof(Vector4), "colum[3] must follow colum[2]");
}
