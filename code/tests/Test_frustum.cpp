#include "Check.hpp"
#include <algorithm>
#include <cmath>
#include <Math/Math.hpp>

namespace
{
	bool Has_unit_normals(const Math::FrustumPlanes& frustum, const bool with_far_plane)
	{
		return Math::Is_normalized(frustum.left.Xyz()) && Math::Is_normalized(frustum.right.Xyz()) &&
			   Math::Is_normalized(frustum.bottom.Xyz()) && Math::Is_normalized(frustum.top.Xyz()) &&
			   Math::Is_normalized(frustum.z_near.Xyz()) && (!with_far_plane || Math::Is_normalized(frustum.z_far.Xyz()));
	}

	// Cámara en el origen mirando hacia -Z, fov de 90 grados y aspect 1: los planos laterales
	// están a 45 grados, así que a distancia d se ve hasta |x| = d y |y| = d.
	void Test_known_points(const Math::FrustumPlanes& frustum, const bool has_far_plane)
	{
		CHECK(Has_unit_normals(frustum, has_far_plane));

		CHECK(Math::Is_inside(frustum, Math::Vector3(0.0f, 0.0f, -5.0f)));
		CHECK(Math::Is_inside(frustum, Math::Vector3(4.0f, 0.0f, -5.0f)));
		CHECK(Math::Is_inside(frustum, Math::Vector3(-4.0f, 4.0f, -5.0f)));
		// Fuera por cada lado.
		CHECK(!Math::Is_inside(frustum, Math::Vector3(6.0f, 0.0f, -5.0f)));
		CHECK(!Math::Is_inside(frustum, Math::Vector3(-6.0f, 0.0f, -5.0f)));
		CHECK(!Math::Is_inside(frustum, Math::Vector3(0.0f, 6.0f, -5.0f)));
		CHECK(!Math::Is_inside(frustum, Math::Vector3(0.0f, -6.0f, -5.0f)));
		// Antes del plano cercano (z_near = 1) y detrás de la cámara.
		CHECK(!Math::Is_inside(frustum, Math::Vector3(0.0f, 0.0f, -0.5f)));
		CHECK(!Math::Is_inside(frustum, Math::Vector3(0.0f, 0.0f, 5.0f)));
		// Más allá del plano lejano (z_far = 10): fuera, salvo con proyección infinita.
		CHECK(Math::Is_inside(frustum, Math::Vector3(0.0f, 0.0f, -11.0f)) == !has_far_plane);
		CHECK(Math::Is_inside(frustum, Math::Vector3(0.0f, 0.0f, -1e6f)) == !has_far_plane);

		// Con normales unitarias, el valor del plano es la distancia.
		const Math::Vector3 point(0.0f, 0.0f, -3.0f);
		CHECK(Math::Equals(Math::Plane_distance(frustum.z_near, point), 2.0f, 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Plane_distance(frustum.left, point), 3.0f * std::sqrt(0.5f), 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Plane_distance(frustum.right, point), 3.0f * std::sqrt(0.5f), 1e-5f, 1e-5f));
		if (has_far_plane)
		{
			CHECK(Math::Equals(Math::Plane_distance(frustum.z_far, point), 7.0f, 1e-5f, 1e-5f));
		}
	}

	void Test_each_convention()
	{
		const Math::Radians fov = Math::Degrees(90.0f);

		Test_known_points(Math::Extract_frustum_planes_standard(Math::Perspective_standard(fov, 1.0f, 1.0f, 10.0f)), true);
		Test_known_points(Math::Extract_frustum_planes_reverse_z(Math::Perspective_reverse_z(fov, 1.0f, 1.0f, 10.0f)), true);
		Test_known_points(Math::Extract_frustum_planes_reverse_z(Math::Perspective_vulkan(fov, 1.0f, 1.0f, 10.0f)), true);
		Test_known_points(Math::Extract_frustum_planes_reverse_z(Math::Perspective_reverse_z_infinite(fov, 1.0f, 1.0f)), false);
		Test_known_points(Math::Extract_frustum_planes_reverse_z(Math::Perspective_vulkan_infinite(fov, 1.0f, 1.0f)), false);

		// Proyección infinita: el plano lejano sale sin normal y acepta cualquier punto.
		const Math::FrustumPlanes infinite = Math::Extract_frustum_planes_reverse_z(Math::Perspective_vulkan_infinite(fov, 1.0f, 1.0f));
		CHECK(infinite.z_far == Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));

		// 'top' es el lado y = +1 de NDC. Sin volteo es arriba en el mundo; con una proyección
		// _vulkan es abajo (documentado en FrustumPlanes).
		const Math::Vector3 high(0.0f, 6.0f, -5.0f);
		const Math::FrustumPlanes y_up = Math::Extract_frustum_planes_reverse_z(Math::Perspective_reverse_z(fov, 1.0f, 1.0f, 10.0f));
		const Math::FrustumPlanes y_down = Math::Extract_frustum_planes_reverse_z(Math::Perspective_vulkan(fov, 1.0f, 1.0f, 10.0f));
		CHECK(Math::Plane_distance(y_up.top, high) < 0.0f && Math::Plane_distance(y_up.bottom, high) > 0.0f);
		CHECK(Math::Plane_distance(y_down.bottom, high) < 0.0f && Math::Plane_distance(y_down.top, high) > 0.0f);
		CHECK(y_up.top == y_down.bottom && y_up.bottom == y_down.top);

		// Ortográfica: el frustum es una caja.
		const Math::FrustumPlanes box = Math::Extract_frustum_planes_reverse_z(Math::Orthographic_vulkan(-2.0f, 2.0f, -1.0f, 1.0f, 0.0f, 10.0f));
		CHECK(Has_unit_normals(box, true));
		CHECK(Math::Is_inside(box, Math::Vector3(1.9f, 0.9f, -5.0f)));
		CHECK(!Math::Is_inside(box, Math::Vector3(2.1f, 0.0f, -5.0f)));
		CHECK(!Math::Is_inside(box, Math::Vector3(0.0f, 1.1f, -5.0f)));
		CHECK(!Math::Is_inside(box, Math::Vector3(0.0f, 0.0f, -10.5f)));
		CHECK(!Math::Is_inside(box, Math::Vector3(0.0f, 0.0f, 0.5f)));
	}

	// Con una cámara cualquiera: estar dentro de los planos (en mundo) tiene que coincidir con
	// estar dentro del volumen de clip tras multiplicar por la matriz.
	void Test_against_clip_space()
	{
		int inside_count = 0;
		for (int i = 0; i < 5000; ++i)
		{
			const Math::Vector3 eye(Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f));
			const Math::Vector3 target(Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f));
			const Math::Vector3 up(0.0f, 1.0f, 0.0f);
			if (!Math::Can_make_view_basis(target - eye, up) || Math::Length(Math::Cross(Math::Normalize(target - eye), up)) < 0.1f)
			{
				continue;
			}

			const Math::Matrix4 view_projection = Math::Perspective_vulkan(Math::Degrees(70.0f), 1.6f, 0.5f, 30.0f) * Math::Look_at(eye, target, up);
			const Math::FrustumPlanes frustum = Math::Extract_frustum_planes_reverse_z(view_projection);
			CHECK(Has_unit_normals(frustum, true));

			const Math::Vector3 point(Math_test::Random_float(-15.0f, 15.0f), Math_test::Random_float(-15.0f, 15.0f), Math_test::Random_float(-15.0f, 15.0f));
			const Math::Vector4 clip = view_projection * Math::Vector4(point, 1.0f);
			// Margen hasta la cara más cercana del volumen de clip: positivo dentro.
			const float margin = std::min({ clip.w - std::fabs(clip.x), clip.w - std::fabs(clip.y), clip.z, clip.w - clip.z });
			if (std::fabs(margin) < 1e-3f)
			{
				continue; // Justo en el borde: el redondeo puede decidir cualquier cosa.
			}

			CHECK(Math::Is_inside(frustum, point) == (margin > 0.0f));
			inside_count += margin > 0.0f ? 1 : 0;
		}
		// El test no vale de nada si todos los puntos caen fuera.
		CHECK(inside_count > 100);
	}
}

int main()
{
	Test_each_convention();
	Test_against_clip_space();
	return Math_test::Result("Test_frustum");
}
