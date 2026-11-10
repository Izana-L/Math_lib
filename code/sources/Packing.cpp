#include <cassert>
#include <cmath>
#include <cstdint>
#include <Math/Packing.hpp>
#include "Glm_convert.hpp"
#include <glm/gtc/packing.hpp>

namespace
{
	Math::Vector2 Fold_octahedral(const Math::Vector2 point) noexcept
	{
		return Math::Vector2((1.0f - std::fabs(point.y)) * (point.x >= 0.0f ? 1.0f : -1.0f),
							 (1.0f - std::fabs(point.x)) * (point.y >= 0.0f ? 1.0f : -1.0f));
	}
}

namespace Math
{
	std::uint16_t Pack_half_1x16(const float value) noexcept
	{
		return glm::packHalf1x16(value);
	}

	std::uint32_t Pack_half_2x16(const Vector2 vector) noexcept
	{
		return glm::packHalf2x16(detail::To_glm(vector));
	}

	std::uint64_t Pack_half_4x16(const Vector4 vector) noexcept
	{
		return glm::packHalf4x16(detail::To_glm(vector));
	}

	float Unpack_half_1x16(const std::uint16_t packed) noexcept
	{
		return glm::unpackHalf1x16(packed);
	}

	Vector2 Unpack_half_2x16(const std::uint32_t packed) noexcept
	{
		return detail::From_glm(glm::unpackHalf2x16(packed));
	}

	Vector4 Unpack_half_4x16(const std::uint64_t packed) noexcept
	{
		return detail::From_glm(glm::unpackHalf4x16(packed));
	}


	std::uint8_t Pack_unorm_1x8(const float value) noexcept
	{
		assert(!std::isnan(value) && "Pack_unorm_1x8: 'value' cannot be NaN");
		return glm::packUnorm1x8(value);
	}

	std::uint16_t Pack_unorm_2x8(const Vector2 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && "Pack_unorm_2x8: 'vector' cannot have NaN values");
		return glm::packUnorm2x8(detail::To_glm(vector));
	}

	std::uint32_t Pack_unorm_4x8(const Vector4 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && !std::isnan(vector.z) && !std::isnan(vector.w) && "Pack_unorm_4x8: 'vector' cannot have NaN values");
		return glm::packUnorm4x8(detail::To_glm(vector));
	}

	float Unpack_unorm_1x8(const std::uint8_t packed) noexcept
	{
		return glm::unpackUnorm1x8(packed);
	}

	Vector2 Unpack_unorm_2x8(const std::uint16_t packed) noexcept
	{
		return detail::From_glm(glm::unpackUnorm2x8(packed));
	}

	Vector4 Unpack_unorm_4x8(const std::uint32_t packed) noexcept
	{
		return detail::From_glm(glm::unpackUnorm4x8(packed));
	}


	std::uint8_t Pack_snorm_1x8(const float value) noexcept
	{
		assert(!std::isnan(value) && "Pack_snorm_1x8: 'value' cannot be NaN");
		return glm::packSnorm1x8(value);
	}

	std::uint16_t Pack_snorm_2x8(const Vector2 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && "Pack_snorm_2x8: 'vector' cannot have NaN values");
		return glm::packSnorm2x8(detail::To_glm(vector));
	}

	std::uint32_t Pack_snorm_4x8(const Vector4 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && !std::isnan(vector.z) && !std::isnan(vector.w) && "Pack_snorm_4x8: 'vector' cannot have NaN values");
		return glm::packSnorm4x8(detail::To_glm(vector));
	}

	float Unpack_snorm_1x8(const std::uint8_t packed) noexcept
	{
		return glm::unpackSnorm1x8(packed);
	}

	Vector2 Unpack_snorm_2x8(const std::uint16_t packed) noexcept
	{
		return detail::From_glm(glm::unpackSnorm2x8(packed));
	}

	Vector4 Unpack_snorm_4x8(const std::uint32_t packed) noexcept
	{
		return detail::From_glm(glm::unpackSnorm4x8(packed));
	}


	std::uint16_t Pack_unorm_1x16(const float value) noexcept
	{
		assert(!std::isnan(value) && "Pack_unorm_1x16: 'value' cannot be NaN");
		return glm::packUnorm1x16(value);
	}

	std::uint32_t Pack_unorm_2x16(const Vector2 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && "Pack_unorm_2x16: 'vector' cannot have NaN values");
		return glm::packUnorm2x16(detail::To_glm(vector));
	}

	std::uint64_t Pack_unorm_4x16(const Vector4 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && !std::isnan(vector.z) && !std::isnan(vector.w) && "Pack_unorm_4x16: 'vector' cannot have NaN values");
		return glm::packUnorm4x16(detail::To_glm(vector));
	}

	float Unpack_unorm_1x16(const std::uint16_t packed) noexcept
	{
		return glm::unpackUnorm1x16(packed);
	}

	Vector2 Unpack_unorm_2x16(const std::uint32_t packed) noexcept
	{
		return detail::From_glm(glm::unpackUnorm2x16(packed));
	}

	Vector4 Unpack_unorm_4x16(const std::uint64_t packed) noexcept
	{
		return detail::From_glm(glm::unpackUnorm4x16(packed));
	}


	std::uint16_t Pack_snorm_1x16(const float value) noexcept
	{
		assert(!std::isnan(value) && "Pack_snorm_1x16: 'value' cannot be NaN");
		return glm::packSnorm1x16(value);
	}

	std::uint32_t Pack_snorm_2x16(const Vector2 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && "Pack_snorm_2x16: 'vector' cannot have NaN values");
		return glm::packSnorm2x16(detail::To_glm(vector));
	}

	std::uint64_t Pack_snorm_4x16(const Vector4 vector) noexcept
	{
		assert(!std::isnan(vector.x) && !std::isnan(vector.y) && !std::isnan(vector.z) && !std::isnan(vector.w) && "Pack_snorm_4x16: 'vector' cannot have NaN values");
		return glm::packSnorm4x16(detail::To_glm(vector));
	}

	float Unpack_snorm_1x16(const std::uint16_t packed) noexcept
	{
		return glm::unpackSnorm1x16(packed);
	}

	Vector2 Unpack_snorm_2x16(const std::uint32_t packed) noexcept
	{
		return detail::From_glm(glm::unpackSnorm2x16(packed));
	}

	Vector4 Unpack_snorm_4x16(const std::uint64_t packed) noexcept
	{
		return detail::From_glm(glm::unpackSnorm4x16(packed));
	}


	Vector2 Encode_octahedral(const Vector3 normal) noexcept
	{
		assert(Is_normalized(normal) && "Encode_octahedral: 'normal' must be normalized");
		const float scale = 1.0f / (std::fabs(normal.x) + std::fabs(normal.y) + std::fabs(normal.z));
		const Vector2 projected(normal.x * scale, normal.y * scale);
		return normal.z >= 0.0f ? projected : Fold_octahedral(projected);
	}

	Vector3 Decode_octahedral(const Vector2 encoded) noexcept
	{
		assert(std::fabs(encoded.x) <= 1.0f && std::fabs(encoded.y) <= 1.0f && "Decode_octahedral: 'encoded' must be in the range [-1, 1]");
		const float z = 1.0f - std::fabs(encoded.x) - std::fabs(encoded.y);
		const Vector2 xy = z >= 0.0f ? encoded : Fold_octahedral(encoded);
		return Normalize(Vector3(xy, z));
	}

	std::uint16_t Pack_octahedral_2x8(const Vector3 normal) noexcept
	{
		return Pack_snorm_2x8(Encode_octahedral(normal));
	}

	std::uint32_t Pack_octahedral_2x16(const Vector3 normal) noexcept
	{
		return Pack_snorm_2x16(Encode_octahedral(normal));
	}

	Vector3 Unpack_octahedral_2x8(const std::uint16_t packed) noexcept
	{
		return Decode_octahedral(Unpack_snorm_2x8(packed));
	}

	Vector3 Unpack_octahedral_2x16(const std::uint32_t packed) noexcept
	{
		return Decode_octahedral(Unpack_snorm_2x16(packed));
	}
}
