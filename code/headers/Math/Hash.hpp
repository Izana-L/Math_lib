#pragma once


#include <cstddef>
#include <functional>
#include <Math/Vector2.hpp>
#include <Math/Vector2i.hpp>
#include <Math/Vector2u.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector3i.hpp>
#include <Math/Vector3u.hpp>
#include <Math/Vector4.hpp>
#include <Math/Vector4i.hpp>
#include <Math/Vector4u.hpp>

namespace Math::detail
{
	inline void Hash_combine(std::size_t& seed, std::size_t value_hash) noexcept
	{
		value_hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= value_hash;
	}
}

namespace std
{
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <>
	struct hash<Math::Vector3i>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3i& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<int> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}

	};
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <>
	struct hash<Math::Vector3>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<float> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
}