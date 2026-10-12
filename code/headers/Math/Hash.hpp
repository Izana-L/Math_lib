#pragma once


#include <cstddef>
#include <functional>
#include <Math/Vector2T.hpp>
#include <Math/Vector3T.hpp>
#include <Math/Vector4T.hpp>


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
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct hash<Math::Vector2T<T>>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<T> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct hash<Math::Vector3T<T>>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<T> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};
	template <typename T>
	requires std::is_arithmetic_v<T>
	struct hash<Math::Vector4T<T>>
	{
		[[nodiscard]] std::size_t operator()(const Math::Vector3& vector) const noexcept
		{
			std::size_t seed = 0;
			std::hash<T> hasher;

			Math::detail::Hash_combine(seed, hasher(vector.x));
			Math::detail::Hash_combine(seed, hasher(vector.y));
			Math::detail::Hash_combine(seed, hasher(vector.z));

			return seed;
		}
	};

}