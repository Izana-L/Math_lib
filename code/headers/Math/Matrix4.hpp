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

		constexpr Matrix4(Vector4 _column0, Vector4 _column1, Vector4 _column2, Vector4 _column3) noexcept : columns{ _column0, _column1, _column2,_column3 } {}

		[[nodiscard]] constexpr Vector4& operator[](std::size_t index) noexcept
		{
			return index == 0 ? columns[0] : index == 1 ? columns[1] : index == 2 ? columns[2] : columns[3];
		}
		[[nodiscard]] constexpr const Vector4& operator[](std::size_t index) const noexcept
		{
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


	};
	static_assert(sizeof(Matrix4) == 4 * sizeof(Vector4), "Matrix4 must be exactly 4 Vector4 in size");
	static_assert(alignof(Matrix4) == alignof(Vector4), "Matrix4 must not be aligned more strictly than a Vector4s");
	static_assert(std::is_standard_layout_v<Matrix4>, "Matrix4 must be standard_layout");
	static_assert(std::is_trivially_copyable_v<Matrix4>, "Matrix4 must be trivially_copyable");
	static_assert(offsetof(Matrix4, columns[0]) == 0, "x must be at offset 0");
	static_assert(offsetof(Matrix4, columns[1]) == sizeof(Vector4), "colum[1] must follow colum[0]");
	static_assert(offsetof(Matrix4, columns[2]) == 2 * sizeof(Vector4), "colum[2] must follow colum[1]");
	static_assert(offsetof(Matrix4, columns[3]) == 3 * sizeof(Vector4), "colum[3] must follow colum[2]");
}
