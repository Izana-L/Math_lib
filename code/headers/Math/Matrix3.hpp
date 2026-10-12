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

		constexpr Matrix3(Vector3 _column0, Vector3 _column1, Vector3 _column2) noexcept : columns{ _column0, _column1, _column2 } {}

		[[nodiscard]] constexpr Vector3& operator[](std::size_t index) noexcept
		{
			return index == 0 ? columns[0] : index == 1 ? columns[1] : columns[2];
		}
		[[nodiscard]] constexpr const Vector3& operator[](std::size_t index) const noexcept
		{
			return index == 0 ? columns[0] : index == 1 ? columns[1] : columns[2];
		}
		[[nodiscard]] constexpr float* Data() noexcept { return &columns[0].x; }
		[[nodiscard]] constexpr const float* Data() const noexcept { return &columns[0].x; }

		[[nodiscard]] constexpr static Matrix3 Identity() noexcept { return Matrix3();}

		[[nodiscard]] constexpr static Matrix3 Zero() noexcept { return Matrix3(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)); }

		[[nodiscard]] constexpr bool operator==(const Matrix3& other_matrix) const noexcept
		{
			return(this->columns[0] == other_matrix.columns[0] && this->columns[1] == other_matrix.columns[1] && this->columns[2] == other_matrix.columns[2]);
		}
		[[nodiscard]] constexpr bool operator!=(const Matrix3& other_matrix) const noexcept
		{
			return(this->columns[0] != other_matrix.columns[0] || this->columns[1] != other_matrix.columns[1] || this->columns[2] != other_matrix.columns[2]);
		}

	};
	static_assert(sizeof(Matrix3) == 3 * sizeof(Vector3), "Matrix3 must be exactly 3 Vector3 in size");
	static_assert(alignof(Matrix3) == alignof(Vector3), "Matrix3 must not be aligned more strictly than a Vector3s");
	static_assert(std::is_standard_layout_v<Matrix3>, "Matrix3 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix3>, "Matrix3 must be trivially_copyable");
	static_assert(offsetof(Matrix3, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix3, columns[1]) == sizeof(Vector3), "y must follow x");
	static_assert(offsetof(Matrix3, columns[2]) == 2 * sizeof(Vector3), "z must follow y");
}
