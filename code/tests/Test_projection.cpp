#include "Check.hpp"
#include "Glm_convert.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <Math/Math.hpp>

namespace
{
	struct Perspective_parameters
	{
		Math::Radians fov_y;
		float aspect, z_near, z_far;
	};

	Perspective_parameters Random_perspective()
	{
		const float z_near = Math_test::Random_float(0.05f, 1.0f);
		return Perspective_parameters{ Math::Radians(Math_test::Random_float(0.3f, 2.5f)), Math_test::Random_float(0.5f, 2.5f), z_near, z_near + Math_test::Random_float(1.0f, 500.0f) };
	}

	// Niega la fila 1: lo que debe hacer el volteo de Y, escrito aparte para comprobarlo.
	Math::Matrix4 With_row_1_negated(Math::Matrix4 matrix)
	{
		for (std::size_t column = 0; column < 4; ++column)
		{
			matrix[column].y = -matrix[column].y;
		}
		return matrix;
	}

	// glm como oráculo. glm solo tiene la convención estándar (Glm_config.hpp comprueba que
	// es diestra y con profundidad 0..1).
	void Test_against_glm()
	{
		using Math::detail::From_glm;

		for (int i = 0; i < 1000; ++i)
		{
			const Perspective_parameters p = Random_perspective();
			CHECK(Math::Equals(Math::Perspective_standard(p.fov_y, p.aspect, p.z_near, p.z_far),
							   From_glm(glm::perspective(p.fov_y.value, p.aspect, p.z_near, p.z_far)), 1e-5f, 1e-4f));

			const float left = Math_test::Random_float(-10.0f, 10.0f);
			const float right = left + Math_test::Random_float(0.5f, 20.0f);
			const float bottom = Math_test::Random_float(-10.0f, 10.0f);
			const float top = bottom + Math_test::Random_float(0.5f, 20.0f);
			const float z_near = Math_test::Random_float(-10.0f, 10.0f);
			const float z_far = z_near + Math_test::Random_float(0.5f, 100.0f);
			CHECK(Math::Equals(Math::Orthographic_standard(left, right, bottom, top, z_near, z_far),
							   From_glm(glm::ortho(left, right, bottom, top, z_near, z_far)), 1e-5f, 1e-4f));
		}
	}

	// Reverse-Z no tiene oráculo: se comprueba por sus propiedades.
	void Test_perspective_depth()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Perspective_parameters p = Random_perspective();
			const Math::Vector3 at_near(0.0f, 0.0f, -p.z_near);
			const Math::Vector3 at_far(0.0f, 0.0f, -p.z_far);
			const Math::Vector3 between(0.0f, 0.0f, -Math_test::Random_float(p.z_near, p.z_far));

			const Math::Matrix4 standard = Math::Perspective_standard(p.fov_y, p.aspect, p.z_near, p.z_far);
			const Math::Matrix4 reverse = Math::Perspective_reverse_z(p.fov_y, p.aspect, p.z_near, p.z_far);
			const Math::Matrix4 infinite = Math::Perspective_reverse_z_infinite(p.fov_y, p.aspect, p.z_near);

			// Estándar: 0 cerca, 1 lejos.
			CHECK(Math::Equals(Math::Project_to_ndc(at_near, standard).z, 0.0f, 1e-5f, 0.0f));
			CHECK(Math::Equals(Math::Project_to_ndc(at_far, standard).z, 1.0f, 1e-5f, 0.0f));
			// Reverse-Z: 1 cerca, 0 lejos, y en medio es 1 - estándar.
			CHECK(Math::Equals(Math::Project_to_ndc(at_near, reverse).z, 1.0f, 1e-5f, 0.0f));
			CHECK(Math::Equals(Math::Project_to_ndc(at_far, reverse).z, 0.0f, 1e-5f, 0.0f));
			CHECK(Math::Equals(Math::Project_to_ndc(between, reverse).z, 1.0f - Math::Project_to_ndc(between, standard).z, 1e-4f, 0.0f));
			// Infinita: 1 cerca y la profundidad es z_near / distancia.
			CHECK(Math::Equals(Math::Project_to_ndc(at_near, infinite).z, 1.0f, 1e-5f, 0.0f));
			CHECK(Math::Equals(Math::Project_to_ndc(between, infinite).z, p.z_near / -between.z, 1e-5f, 1e-5f));

			// x e y no dependen de la convención de profundidad.
			const Math::Vector3 point(Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f), between.z);
			CHECK(Math::Equals(Math::Project_to_ndc(point, reverse).Xy(), Math::Project_to_ndc(point, standard).Xy()));
			CHECK(Math::Equals(Math::Project_to_ndc(point, infinite).Xy(), Math::Project_to_ndc(point, standard).Xy()));
		}

		// La infinita es el límite de la finita con un plano lejano enorme.
		CHECK(Math::Equals(Math::Perspective_reverse_z(Math::Radians(1.0f), 1.5f, 0.1f, 1e30f), Math::Perspective_reverse_z_infinite(Math::Radians(1.0f), 1.5f, 0.1f)));
		// Muy lejos, la profundidad de la infinita tiende a 0 sin llegar a ser negativa.
		const float far_depth = Math::Project_to_ndc(Math::Vector3(0.0f, 0.0f, -1e9f), Math::Perspective_reverse_z_infinite(Math::Radians(1.0f), 1.5f, 0.1f)).z;
		CHECK(far_depth > 0.0f && far_depth < 1e-9f);

		// El borde del campo de visión cae en el borde de NDC: con fov de 90 grados, un punto a
		// 45 grados hacia arriba da y = 1, y con aspect 2 hace falta el doble en x para x = 1.
		const Math::Matrix4 projection = Math::Perspective_reverse_z(Math::Degrees(90.0f), 2.0f, 0.1f, 100.0f);
		CHECK(Math::Equals(Math::Project_to_ndc(Math::Vector3(0.0f, 5.0f, -5.0f), projection).y, 1.0f, 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Project_to_ndc(Math::Vector3(10.0f, 0.0f, -5.0f), projection).x, 1.0f, 1e-5f, 1e-5f));
	}

	void Test_orthographic_depth()
	{
		const float left = -2.0f, right = 6.0f, bottom = -1.0f, top = 3.0f, z_near = 0.5f, z_far = 20.0f;
		const Math::Vector3 near_corner(left, bottom, -z_near);
		const Math::Vector3 far_corner(right, top, -z_far);

		const Math::Matrix4 standard = Math::Orthographic_standard(left, right, bottom, top, z_near, z_far);
		CHECK(Math::Equals(Math::Project_to_ndc(near_corner, standard), Math::Vector3(-1.0f, -1.0f, 0.0f), 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Project_to_ndc(far_corner, standard), Math::Vector3(1.0f, 1.0f, 1.0f), 1e-5f, 1e-5f));

		const Math::Matrix4 reverse = Math::Orthographic_reverse_z(left, right, bottom, top, z_near, z_far);
		CHECK(Math::Equals(Math::Project_to_ndc(near_corner, reverse), Math::Vector3(-1.0f, -1.0f, 1.0f), 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Project_to_ndc(far_corner, reverse), Math::Vector3(1.0f, 1.0f, 0.0f), 1e-5f, 1e-5f));

		// Vulkan: la esquina de abajo (bottom) cae en y = +1, que en Vulkan es abajo en pantalla.
		const Math::Matrix4 vulkan = Math::Orthographic_vulkan(left, right, bottom, top, z_near, z_far);
		CHECK(Math::Equals(Math::Project_to_ndc(near_corner, vulkan), Math::Vector3(-1.0f, 1.0f, 1.0f), 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Project_to_ndc(far_corner, vulkan), Math::Vector3(1.0f, -1.0f, 0.0f), 1e-5f, 1e-5f));
		CHECK(vulkan == With_row_1_negated(reverse));

		// Coordenadas de píxel con Y hacia abajo (interfaz): top = 0 y bottom = alto. El origen
		// queda arriba a la izquierda en Vulkan (x = -1, y = -1).
		const Math::Matrix4 pixels = Math::Orthographic_vulkan(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 1.0f);
		CHECK(Math::Equals(Math::Project_to_ndc(Math::Vector3(0.0f, 0.0f, 0.0f), pixels).Xy(), Math::Vector2(-1.0f, -1.0f), 1e-5f, 1e-5f));
		CHECK(Math::Equals(Math::Project_to_ndc(Math::Vector3(800.0f, 600.0f, 0.0f), pixels).Xy(), Math::Vector2(1.0f, 1.0f), 1e-5f, 1e-5f));
	}

	// El volteo de Y: solo cambia la fila 1, y un punto por encima de la cámara baja en NDC.
	void Test_vulkan_flip()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Perspective_parameters p = Random_perspective();
			CHECK(Math::Perspective_vulkan(p.fov_y, p.aspect, p.z_near, p.z_far) == With_row_1_negated(Math::Perspective_reverse_z(p.fov_y, p.aspect, p.z_near, p.z_far)));
			CHECK(Math::Perspective_vulkan_infinite(p.fov_y, p.aspect, p.z_near) == With_row_1_negated(Math::Perspective_reverse_z_infinite(p.fov_y, p.aspect, p.z_near)));
		}

		const Math::Matrix4 vulkan = Math::Perspective_vulkan(Math::Degrees(60.0f), 16.0f / 9.0f, 0.1f, 100.0f);
		const Math::Vector3 above = Math::Project_to_ndc(Math::Vector3(0.0f, 1.0f, -5.0f), vulkan);
		const Math::Vector3 to_the_right = Math::Project_to_ndc(Math::Vector3(1.0f, 0.0f, -5.0f), vulkan);
		CHECK(above.y < 0.0f);
		CHECK(to_the_right.x > 0.0f);
		// Reverse-Z: lo más cercano tiene más profundidad.
		CHECK(Math::Project_to_ndc(Math::Vector3(0.0f, 0.0f, -1.0f), vulkan).z > Math::Project_to_ndc(Math::Vector3(0.0f, 0.0f, -50.0f), vulkan).z);

		// Degrees se convierte solo a Radians (CONVENTIONS.md §5).
		CHECK(Math::Perspective_vulkan(Math::Degrees(60.0f), 1.5f, 0.1f, 100.0f) == Math::Perspective_vulkan(Math::Radians(Math::Degrees(60.0f)), 1.5f, 0.1f, 100.0f));
	}

	// Ida y vuelta: Unproject deshace Project_to_ndc.
	void Test_unproject()
	{
		for (int i = 0; i < 1000; ++i)
		{
			const Math::Vector3 eye(Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f), Math_test::Random_float(-5.0f, 5.0f));
			const Math::Vector3 target = eye + Math::Vector3(Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-0.5f, 0.5f), -1.0f);
			const Math::Matrix4 view = Math::Look_at(eye, target, Math::Vector3(0.0f, 1.0f, 0.0f));
			const Math::Matrix4 view_projection = Math::Perspective_vulkan(Math::Degrees(70.0f), 1.6f, 0.5f, 100.0f) * view;
			const Math::Matrix4 inverse = Math::Inverse(view_projection);

			// Un punto delante de la cámara, entre 1 y 20 unidades.
			const Math::Vector3 in_view(Math_test::Random_float(-0.5f, 0.5f), Math_test::Random_float(-0.5f, 0.5f), -1.0f);
			const Math::Vector3 point = Math::Transform_point(Math::Inverse_rigid(view), in_view * Math_test::Random_float(1.0f, 20.0f));

			const Math::Vector3 ndc = Math::Project_to_ndc(point, view_projection);
			CHECK(ndc.x >= -1.0f && ndc.x <= 1.0f && ndc.y >= -1.0f && ndc.y <= 1.0f && ndc.z >= 0.0f && ndc.z <= 1.0f);
			CHECK(Math::Equals(Math::Unproject(ndc, inverse), point, 1e-2f, 1e-3f));
		}

		// El centro de la pantalla con profundidad 1 es el punto del plano cercano delante de la cámara.
		const Math::Matrix4 projection = Math::Perspective_vulkan_infinite(Math::Degrees(60.0f), 1.0f, 0.25f);
		CHECK(Math::Equals(Math::Unproject(Math::Vector3(0.0f, 0.0f, 1.0f), Math::Inverse(projection)), Math::Vector3(0.0f, 0.0f, -0.25f), 1e-5f, 1e-5f));
	}

	void Test_validation()
	{
		const Math::Radians fov(1.0f);
		const float nan = std::numeric_limits<float>::quiet_NaN();
		const float infinity = std::numeric_limits<float>::infinity();

		CHECK(Math::Is_valid_perspective(fov, 1.5f, 0.1f, 100.0f));
		CHECK(Math::Is_valid_perspective(Math::Degrees(179.0f), 1.5f, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(Math::Radians(0.0f), 1.5f, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(Math::Degrees(180.0f), 1.5f, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(Math::Radians(nan), 1.5f, 0.1f, 100.0f));
		// Ventana minimizada: alto 0 da aspect infinito o NaN; ancho 0 da aspect 0.
		CHECK(!Math::Is_valid_perspective(fov, 0.0f, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(fov, infinity, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(fov, nan, 0.1f, 100.0f));
		CHECK(!Math::Is_valid_perspective(fov, 1.5f, 0.0f, 100.0f));
		CHECK(!Math::Is_valid_perspective(fov, 1.5f, 0.1f, 0.1f));
		CHECK(!Math::Is_valid_perspective(fov, 1.5f, 100.0f, 0.1f));
		CHECK(!Math::Is_valid_perspective(fov, 1.5f, 0.1f, infinity));
		CHECK(Math::Is_valid_perspective_infinite(fov, 1.5f, 0.1f));
		CHECK(!Math::Is_valid_perspective_infinite(fov, 1.5f, 0.0f));

		CHECK(Math::Is_valid_orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f));
		CHECK(Math::Is_valid_orthographic(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 1.0f));
		CHECK(!Math::Is_valid_orthographic(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f));
		CHECK(!Math::Is_valid_orthographic(-1.0f, 1.0f, 2.0f, 2.0f, 0.0f, 10.0f));
		CHECK(!Math::Is_valid_orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 5.0f, 5.0f));
		CHECK(!Math::Is_valid_orthographic(-1.0f, nan, -1.0f, 1.0f, 0.0f, 10.0f));
	}
}

int main()
{
	Test_against_glm();
	Test_perspective_depth();
	Test_orthographic_depth();
	Test_vulkan_flip();
	Test_unproject();
	Test_validation();
	return Math_test::Result("Test_projection");
}
