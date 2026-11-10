#include "Check.hpp"
#include "Glm_convert.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <Math/Math.hpp>

namespace
{
	Math::Vector3 Random_vector3(const float min, const float max)
	{
		return Math::Vector3(Math_test::Random_float(min, max), Math_test::Random_float(min, max), Math_test::Random_float(min, max));
	}

	// glm como oráculo, con entradas aleatorias.
	void Test_against_glm()
	{
		using Math::detail::From_glm;
		using Math::detail::To_glm;

		for (int i = 0; i < 1000; ++i)
		{
			const Math::Vector3 eye = Random_vector3(-10.0f, 10.0f);
			const Math::Vector3 target = Random_vector3(-10.0f, 10.0f);
			const Math::Vector3 up = Random_vector3(-1.0f, 1.0f);
			// Se descartan los casos casi paralelos: ahí el resultado es poco preciso en los dos.
			if (!Math::Can_make_view_basis(target - eye, up) ||
				Math::Length(Math::Cross(Math::Normalize(target - eye), Math::Normalize(up))) < 0.05f)
			{
				continue;
			}

			CHECK(Math::Equals(Math::Look_at(eye, target, up), From_glm(glm::lookAt(To_glm(eye), To_glm(target), To_glm(up))), 1e-4f, 1e-4f));
		}
	}

	// Invariantes de la base y de la vista.
	void Test_invariants()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Math::Vector3 eye = Random_vector3(-10.0f, 10.0f);
			const Math::Vector3 target = Random_vector3(-10.0f, 10.0f);
			const Math::Vector3 up(0.0f, 1.0f, 0.0f);
			const Math::Vector3 to_target = target - eye;
			if (!Math::Can_make_view_basis(to_target, up) ||
				Math::Length(Math::Cross(Math::Normalize(to_target), up)) < 0.05f)
			{
				continue;
			}

			// La base es ortonormal y diestra: [right | up | -forward] es una rotación.
			const Math::ViewBasis basis = Math::Make_view_basis(to_target, up);
			const Math::Matrix3 rotation(basis.right, basis.up, -basis.forward);
			CHECK(Math::Is_orthonormal(rotation));
			CHECK(Math::Equals(Math::Determinant(rotation), 1.0f, 1e-5f, 1e-5f));
			CHECK(Math::Equals(basis.forward, Math::Normalize(to_target)));
			// El 'up' de la base queda del mismo lado que la pista.
			CHECK(Math::Dot(basis.up, up) > 0.0f);

			// La vista es la inversa de la matriz de la cámara en el mundo.
			const Math::Matrix4 view = Math::View_from_basis(eye, basis);
			const Math::Matrix4 camera(rotation, eye);
			CHECK(Math::Equals(view, Math::Inverse_rigid(camera), 1e-4f, 1e-4f));
			CHECK(Math::Is_identity(view * camera, 1e-4f, 1e-4f));

			// La cámara queda en el origen y el objetivo delante, sobre el eje -Z.
			CHECK(Math::Equals(Math::Transform_point(view, eye), Math::Vector3(), 1e-4f, 1e-4f));
			CHECK(Math::Equals(Math::Transform_point(view, target), Math::Vector3(0.0f, 0.0f, -Math::Length(to_target)), 1e-4f, 1e-4f));
		}

		// Una cámara sin rotación en el origen mira hacia -Z: su vista es la identidad.
		CHECK(Math::Is_identity(Math::Look_at(Math::Vector3(), Math::Vector3(0.0f, 0.0f, -1.0f), Math::Vector3(0.0f, 1.0f, 0.0f))));
	}

	// Casos límite (ROADMAP, Tests): las variantes seguras no devuelven NaN.
	void Test_edge_cases()
	{
		const Math::Vector3 eye(1.0f, 2.0f, 3.0f);
		const Math::Vector3 up(0.0f, 1.0f, 0.0f);
		const float nan = std::numeric_limits<float>::quiet_NaN();
		const float infinity = std::numeric_limits<float>::infinity();

		// El objetivo coincide con la cámara.
		CHECK(!Math::Try_look_at(eye, eye, up).has_value());
		// Se mira justo hacia arriba o hacia abajo.
		CHECK(!Math::Try_look_at(eye, eye + Math::Vector3(0.0f, 5.0f, 0.0f), up).has_value());
		CHECK(!Math::Try_look_at(eye, eye - Math::Vector3(0.0f, 5.0f, 0.0f), up).has_value());
		// 'up' nulo, NaN e infinito.
		CHECK(!Math::Try_look_at(eye, Math::Vector3(), Math::Vector3()).has_value());
		CHECK(!Math::Try_look_at(eye, Math::Vector3(nan, 0.0f, 0.0f), up).has_value());
		CHECK(!Math::Try_look_at(Math::Vector3(infinity, 0.0f, 0.0f), Math::Vector3(), up).has_value());
		CHECK(!Math::Try_make_view_basis(Math::Vector3(), up).has_value());

		// Caso válido: hay resultado y es finito.
		const std::optional<Math::Matrix4> view = Math::Try_look_at(eye, Math::Vector3(), up);
		CHECK(view.has_value());
		CHECK(view.has_value() && Math::Is_finite((*view)[0]) && Math::Is_finite((*view)[1]) && Math::Is_finite((*view)[2]) && Math::Is_finite((*view)[3]));

		// 'up' no tiene que estar normalizado ni ser perpendicular a 'forward'.
		const Math::ViewBasis basis = Math::Make_view_basis(Math::Vector3(0.0f, 0.0f, -4.0f), Math::Vector3(0.0f, 3.0f, -3.0f));
		CHECK(Math::Equals(basis.right, Math::Vector3(1.0f, 0.0f, 0.0f)));
		CHECK(Math::Equals(basis.up, Math::Vector3(0.0f, 1.0f, 0.0f)));
		CHECK(Math::Equals(basis.forward, Math::Vector3(0.0f, 0.0f, -1.0f)));
	}
}

int main()
{
	Test_against_glm();
	Test_invariants();
	Test_edge_cases();
	return Math_test::Result("Test_view");
}
