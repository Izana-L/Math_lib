
#include <cstdint>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

namespace Math
{

	[[nodiscard]] std::uint16_t Pack_half_1x16(const float value) noexcept;
	//VK_FORMAT_R16G16_SFLOAT
	[[nodiscard]] std::uint32_t Pack_half_2x16(const Vector2 vector) noexcept;
	//VK_FORMAT_R16G16B16A16_SFLOAT
	[[nodiscard]] std::uint64_t Pack_half_4x16(const Vector4 vector) noexcept;
	[[nodiscard]] float Unpack_half_1x16(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector2 Unpack_half_2x16(const std::uint32_t packed) noexcept;
	[[nodiscard]] Vector4 Unpack_half_4x16(const std::uint64_t packed) noexcept;


	[[nodiscard]] std::uint8_t Pack_unorm_1x8(const float value) noexcept;
	//VK_FORMAT_R16G16_UNORM
	[[nodiscard]] std::uint16_t Pack_unorm_2x8(const Vector2 vector) noexcept;
	//VK_FORMAT_R8G8B8A8_UNORM
	[[nodiscard]] std::uint32_t Pack_unorm_4x8(const Vector4 vector) noexcept;
	[[nodiscard]] float Unpack_unorm_1x8(const std::uint8_t packed) noexcept;
	[[nodiscard]] Vector2 Unpack_unorm_2x8(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector4 Unpack_unorm_4x8(const std::uint32_t packed) noexcept;

	[[nodiscard]] std::uint8_t Pack_snorm_1x8(const float value) noexcept;
	//VK_FORMAT_R16G16_SNORM
	[[nodiscard]] std::uint16_t Pack_snorm_2x8(const Vector2 vector) noexcept;
	//VK_FORMAT_R8G8B8A8_SNORM
	[[nodiscard]] std::uint32_t Pack_snorm_4x8(const Vector4 vector) noexcept;
	[[nodiscard]] float Unpack_snorm_1x8(const std::uint8_t packed) noexcept;
	[[nodiscard]] Vector2 Unpack_snorm_2x8(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector4 Unpack_snorm_4x8(const std::uint32_t packed) noexcept;

	[[nodiscard]] std::uint16_t Pack_unorm_1x16(const float value) noexcept;
	[[nodiscard]] std::uint32_t Pack_unorm_2x16(const Vector2 vector) noexcept;
	[[nodiscard]] std::uint64_t Pack_unorm_4x16(const Vector4 vector) noexcept;
	[[nodiscard]] float Unpack_unorm_1x16(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector2 Unpack_unorm_2x16(const std::uint32_t packed) noexcept;
	[[nodiscard]] Vector4 Unpack_unorm_4x16(const std::uint64_t packed) noexcept;

	[[nodiscard]] std::uint16_t Pack_snorm_1x16(const float value) noexcept;
	[[nodiscard]] std::uint32_t Pack_snorm_2x16(const Vector2 vector) noexcept;
	[[nodiscard]] std::uint64_t Pack_snorm_4x16(const Vector4 vector) noexcept;
	[[nodiscard]] float Unpack_snorm_1x16(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector2 Unpack_snorm_2x16(const std::uint32_t packed) noexcept;
	[[nodiscard]] Vector4 Unpack_snorm_4x16(const std::uint64_t packed) noexcept;

	[[nodiscard]] Vector2 Encode_octahedral(const Vector3 normal) noexcept;
	[[nodiscard]] Vector3 Decode_octahedral(const Vector2 encoded) noexcept;
	//VK_FORMAT_R8G8_SNORM
	[[nodiscard]] std::uint16_t Pack_octahedral_2x8(const Vector3 normal) noexcept;
	//VK_FORMAT_R16G16_SNORM
	[[nodiscard]] std::uint32_t Pack_octahedral_2x16(const Vector3 normal) noexcept;
	[[nodiscard]] Vector3 Unpack_octahedral_2x8(const std::uint16_t packed) noexcept;
	[[nodiscard]] Vector3 Unpack_octahedral_2x16(const std::uint32_t packed) noexcept;
}
