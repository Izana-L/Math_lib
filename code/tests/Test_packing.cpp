#include "Check.hpp"
#include "Glm_convert.hpp"
#include <glm/gtc/packing.hpp>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <Math/Math.hpp>
#include <Math/Packing.hpp>

namespace
{
	Math::Vector4 Random_vector4()
	{
		return Math::Vector4(Math_test::Random_float(-1.5f, 1.5f), Math_test::Random_float(-1.5f, 1.5f), Math_test::Random_float(-1.5f, 1.5f), Math_test::Random_float(-1.5f, 1.5f));
	}

	Math::Vector3 Random_normal()
	{
		Math::Vector3 vector;
		do
		{
			vector = Math::Vector3(Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f), Math_test::Random_float(-1.0f, 1.0f));
		} while (Math::Length_squared(vector) < 0.01f);
		return Math::Normalize(vector);
	}

	// Valores conocidos y orden de los bytes en memoria (el que esperan los formatos de Vulkan).
	void Test_known_values()
	{
		CHECK(Math::Pack_half_1x16(1.0f) == 0x3C00);
		CHECK(Math::Pack_half_1x16(-2.0f) == 0xC000);
		CHECK(Math::Unpack_half_1x16(0x3C00) == 1.0f);

		CHECK(Math::Pack_unorm_1x8(0.0f) == 0);
		CHECK(Math::Pack_unorm_1x8(1.0f) == 255);
		CHECK(Math::Pack_unorm_1x16(1.0f) == 0xFFFF);
		CHECK(Math::Pack_snorm_1x8(1.0f) == 0x7F);
		CHECK(Math::Pack_snorm_1x8(-1.0f) == 0x81);
		CHECK(Math::Pack_snorm_1x16(-1.0f) == 0x8001);

		// Fuera de rango se satura.
		CHECK(Math::Pack_unorm_1x8(2.0f) == 255);
		CHECK(Math::Pack_unorm_1x8(-1.0f) == 0);
		CHECK(Math::Pack_snorm_1x16(-5.0f) == 0x8001);

		// El entero más negativo también se lee como -1.
		CHECK(Math::Unpack_snorm_1x8(0x80) == -1.0f);
		CHECK(Math::Unpack_snorm_1x16(0x8000) == -1.0f);

		// La componente x va en los bits bajos: en memoria queda x, y, z, w.
		const std::uint32_t color = Math::Pack_unorm_4x8(Math::Vector4(1.0f, 0.5f, 0.0f, 1.0f));
		std::uint8_t bytes[4];
		std::memcpy(bytes, &color, sizeof(bytes));
		CHECK(color == 0xFF0080FFu);
		CHECK(bytes[0] == 255 && bytes[1] == 128 && bytes[2] == 0 && bytes[3] == 255);

		const std::uint64_t halves = Math::Pack_half_4x16(Math::Vector4(1.0f, 2.0f, -1.0f, 0.5f));
		std::uint16_t words[4];
		std::memcpy(words, &halves, sizeof(words));
		CHECK(words[0] == 0x3C00 && words[1] == 0x4000 && words[2] == 0xBC00 && words[3] == 0x3800);

		CHECK(Math::Pack_unorm_2x16(Math::Vector2(1.0f, 0.0f)) == 0x0000FFFFu);
	}

	// glm como oráculo, con entradas aleatorias dentro y fuera de rango.
	void Test_against_glm()
	{
		using Math::detail::To_glm;

		for (int i = 0; i < 2000; ++i)
		{
			const Math::Vector4 vector4 = Random_vector4();
			const Math::Vector2 vector2 = vector4.Xyz().Xy();
			const float value = vector4.x;

			CHECK(Math::Pack_half_1x16(value) == glm::packHalf1x16(value));
			CHECK(Math::Pack_half_2x16(vector2) == glm::packHalf2x16(To_glm(vector2)));
			CHECK(Math::Pack_half_4x16(vector4) == glm::packHalf4x16(To_glm(vector4)));

			CHECK(Math::Pack_unorm_1x8(value) == glm::packUnorm1x8(value));
			CHECK(Math::Pack_unorm_2x8(vector2) == glm::packUnorm2x8(To_glm(vector2)));
			CHECK(Math::Pack_unorm_4x8(vector4) == glm::packUnorm4x8(To_glm(vector4)));

			CHECK(Math::Pack_snorm_1x8(value) == glm::packSnorm1x8(value));
			CHECK(Math::Pack_snorm_2x8(vector2) == glm::packSnorm2x8(To_glm(vector2)));
			CHECK(Math::Pack_snorm_4x8(vector4) == glm::packSnorm4x8(To_glm(vector4)));

			CHECK(Math::Pack_unorm_1x16(value) == glm::packUnorm1x16(value));
			CHECK(Math::Pack_unorm_2x16(vector2) == glm::packUnorm2x16(To_glm(vector2)));
			CHECK(Math::Pack_unorm_4x16(vector4) == glm::packUnorm4x16(To_glm(vector4)));

			CHECK(Math::Pack_snorm_1x16(value) == glm::packSnorm1x16(value));
			CHECK(Math::Pack_snorm_2x16(vector2) == glm::packSnorm2x16(To_glm(vector2)));
			CHECK(Math::Pack_snorm_4x16(vector4) == glm::packSnorm4x16(To_glm(vector4)));
		}
	}

	// Idas y vueltas: tras empaquetar y desempaquetar, el error es como mucho medio paso.
	void Test_round_trips()
	{
		const float step_unorm_8 = 0.5f / 255.0f + 1e-6f;
		const float step_snorm_8 = 0.5f / 127.0f + 1e-6f;
		const float step_unorm_16 = 0.5f / 65535.0f + 1e-6f;
		const float step_snorm_16 = 0.5f / 32767.0f + 1e-6f;

		for (int i = 0; i < 2000; ++i)
		{
			const Math::Vector4 vector4 = Random_vector4();
			const Math::Vector2 vector2 = vector4.Xyz().Xy();
			const float value = vector4.x;

			// Lo que debe salir: el valor original recortado al rango del formato.
			const Math::Vector4 unorm4 = Math::Saturate(vector4);
			const Math::Vector4 snorm4 = Math::Clamp(vector4, -1.0f, 1.0f);
			const Math::Vector2 unorm2 = unorm4.Xyz().Xy();
			const Math::Vector2 snorm2 = snorm4.Xyz().Xy();

			CHECK(Math::Equals(Math::Unpack_unorm_1x8(Math::Pack_unorm_1x8(value)), unorm4.x, step_unorm_8, 0.0f));
			CHECK(Math::Equals(Math::Unpack_unorm_2x8(Math::Pack_unorm_2x8(vector2)), unorm2, step_unorm_8, 0.0f));
			CHECK(Math::Equals(Math::Unpack_unorm_4x8(Math::Pack_unorm_4x8(vector4)), unorm4, step_unorm_8, 0.0f));

			CHECK(Math::Equals(Math::Unpack_snorm_1x8(Math::Pack_snorm_1x8(value)), snorm4.x, step_snorm_8, 0.0f));
			CHECK(Math::Equals(Math::Unpack_snorm_2x8(Math::Pack_snorm_2x8(vector2)), snorm2, step_snorm_8, 0.0f));
			CHECK(Math::Equals(Math::Unpack_snorm_4x8(Math::Pack_snorm_4x8(vector4)), snorm4, step_snorm_8, 0.0f));

			CHECK(Math::Equals(Math::Unpack_unorm_1x16(Math::Pack_unorm_1x16(value)), unorm4.x, step_unorm_16, 0.0f));
			CHECK(Math::Equals(Math::Unpack_unorm_2x16(Math::Pack_unorm_2x16(vector2)), unorm2, step_unorm_16, 0.0f));
			CHECK(Math::Equals(Math::Unpack_unorm_4x16(Math::Pack_unorm_4x16(vector4)), unorm4, step_unorm_16, 0.0f));

			CHECK(Math::Equals(Math::Unpack_snorm_1x16(Math::Pack_snorm_1x16(value)), snorm4.x, step_snorm_16, 0.0f));
			CHECK(Math::Equals(Math::Unpack_snorm_2x16(Math::Pack_snorm_2x16(vector2)), snorm2, step_snorm_16, 0.0f));
			CHECK(Math::Equals(Math::Unpack_snorm_4x16(Math::Pack_snorm_4x16(vector4)), snorm4, step_snorm_16, 0.0f));

			// Half no recorta: guarda unas 3 cifras significativas.
			CHECK(Math::Equals(Math::Unpack_half_1x16(Math::Pack_half_1x16(value)), value, 0.0f, 1e-3f));
			CHECK(Math::Equals(Math::Unpack_half_2x16(Math::Pack_half_2x16(vector2)), vector2, 0.0f, 1e-3f));
			CHECK(Math::Equals(Math::Unpack_half_4x16(Math::Pack_half_4x16(vector4)), vector4, 0.0f, 1e-3f));
		}
	}

	// La normal que sale es unitaria y se desvía de la original menos de lo que permite cada formato.
	void Check_octahedral(const Math::Vector3 normal)
	{
		const Math::Vector2 encoded = Math::Encode_octahedral(normal);
		CHECK(std::fabs(encoded.x) <= 1.0f && std::fabs(encoded.y) <= 1.0f);

		const Math::Vector3 from_float = Math::Decode_octahedral(encoded);
		const Math::Vector3 from_16 = Math::Unpack_octahedral_2x16(Math::Pack_octahedral_2x16(normal));
		const Math::Vector3 from_8 = Math::Unpack_octahedral_2x8(Math::Pack_octahedral_2x8(normal));

		CHECK(Math::Is_normalized(from_float) && Math::Is_normalized(from_16) && Math::Is_normalized(from_8));
		CHECK(Math::Angle_between(normal, from_float).value < 1e-5f);
		CHECK(Math::Angle_between(normal, from_16).value < 1e-4f);
		CHECK(Math::Angle_between(normal, from_8).value < 0.03f);
	}

	void Test_octahedral()
	{
		for (int i = 0; i < 20000; ++i)
		{
			Check_octahedral(Random_normal());
		}

		// Casos límite: los ejes, las aristas y las esquinas del octaedro.
		const float edge = std::sqrt(0.5f);
		const float corner = std::sqrt(1.0f / 3.0f);
		const Math::Vector3 special_normals[] = {
			Math::Vector3(1.0f, 0.0f, 0.0f), Math::Vector3(-1.0f, 0.0f, 0.0f),
			Math::Vector3(0.0f, 1.0f, 0.0f), Math::Vector3(0.0f, -1.0f, 0.0f),
			Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector3(0.0f, 0.0f, -1.0f),
			Math::Vector3(edge, edge, 0.0f), Math::Vector3(-edge, -edge, 0.0f),
			Math::Vector3(-edge, 0.0f, edge), Math::Vector3(0.0f, edge, -edge),
			Math::Vector3(corner, corner, corner), Math::Vector3(-corner, -corner, -corner), Math::Vector3(corner, -corner, -corner) };
		for (const Math::Vector3 normal : special_normals)
		{
			Check_octahedral(normal);
		}

		// +Z es el centro del cuadrado, -Z sus cuatro esquinas y +X el centro de un lado.
		CHECK(Math::Encode_octahedral(Math::Vector3(0.0f, 0.0f, 1.0f)) == Math::Vector2(0.0f, 0.0f));
		CHECK(Math::Equals(Math::Decode_octahedral(Math::Vector2(0.0f, 0.0f)), Math::Vector3(0.0f, 0.0f, 1.0f)));
		CHECK(Math::Equals(Math::Decode_octahedral(Math::Vector2(1.0f, 1.0f)), Math::Vector3(0.0f, 0.0f, -1.0f)));
		CHECK(Math::Equals(Math::Decode_octahedral(Math::Vector2(-1.0f, 1.0f)), Math::Vector3(0.0f, 0.0f, -1.0f)));
		CHECK(Math::Equals(Math::Decode_octahedral(Math::Vector2(1.0f, 0.0f)), Math::Vector3(1.0f, 0.0f, 0.0f)));
	}
}

int main()
{
	Test_known_values();
	Test_against_glm();
	Test_round_trips();
	Test_octahedral();
	return Math_test::Result("Test_packing");
}
