#pragma once

#include <cassert>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <Math/Matrix3.hpp>
#include <Math/Matrix4.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

namespace Math
{

	struct ViewBasis
	{
		Vector3 right, up, forward;
	};

	static_assert(sizeof(ViewBasis) == 3 * sizeof(Vector3), "ViewBasis must be exactly 3 Vector3 in size");
	static_assert(alignof(ViewBasis) == alignof(Vector3), "ViewBasis must not be aligned more strictly than a Vector3");
	static_assert(std::is_standard_layout_v<ViewBasis>, "ViewBasis must be standard_layout");
	static_assert(std::is_trivially_copyable_v<ViewBasis>, "ViewBasis must be trivially_copyable");
	static_assert(offsetof(ViewBasis, right) == 0, "right must be at offset 0");
	static_assert(offsetof(ViewBasis, up) == sizeof(Vector3), "up must follow right");
	static_assert(offsetof(ViewBasis, forward) == 2 * sizeof(Vector3), "forward must follow up");


	[[nodiscard]] inline bool Can_make_view_basis(const Vector3 forward, const Vector3 up) noexcept
	{
		if (!Is_normalizable(forward) || !Is_normalizable(up))
		{
			return false;
		}
		return Is_normalizable(Cross(Normalize(forward), Normalize(up)));
	}


	[[nodiscard]] inline ViewBasis Make_view_basis(const Vector3 forward, const Vector3 up) noexcept
	{
		assert(Can_make_view_basis(forward, up) && "Make_view_basis: 'forward' and 'up' must be normalizable and not parallel");
		const Vector3 direction = Normalize(forward);
		const Vector3 right = Normalize(Cross(direction, Normalize(up)));
		return ViewBasis{ right, Cross(right, direction), direction };
	}

	[[nodiscard]] inline std::optional<ViewBasis> Try_make_view_basis(const Vector3 forward, const Vector3 up) noexcept
	{
		if (!Can_make_view_basis(forward, up))
		{
			return std::nullopt;
		}
		return Make_view_basis(forward, up);
	}

	[[nodiscard]] constexpr Matrix4 View_from_basis(const Vector3 eye, const ViewBasis& basis) noexcept
	{
		assert(Is_orthonormal(Matrix3(basis.right, basis.up, -basis.forward)) && "View_from_basis: the basis must be orthonormal");
		return Matrix4(Vector4(basis.right.x, basis.up.x, -basis.forward.x, 0.0f),
					   Vector4(basis.right.y, basis.up.y, -basis.forward.y, 0.0f),
					   Vector4(basis.right.z, basis.up.z, -basis.forward.z, 0.0f),
					   Vector4(-Dot(basis.right, eye), -Dot(basis.up, eye), Dot(basis.forward, eye), 1.0f));
	}


	[[nodiscard]] inline Matrix4 Look_at(const Vector3 eye, const Vector3 target, const Vector3 up) noexcept
	{
		assert(Can_make_view_basis(target - eye, up) && "Look_at: 'target' must differ from 'eye' and the view direction cannot be parallel to 'up'");
		return View_from_basis(eye, Make_view_basis(target - eye, up));
	}


	[[nodiscard]] inline std::optional<Matrix4> Try_look_at(const Vector3 eye, const Vector3 target, const Vector3 up) noexcept
	{
		if (!Can_make_view_basis(target - eye, up))
		{
			return std::nullopt;
		}
		return Look_at(eye, target, up);
	}
}
