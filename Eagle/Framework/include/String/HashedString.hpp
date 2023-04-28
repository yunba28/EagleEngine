#pragma once

#include <String/String.hpp>
#include <Container/HashTable.hpp>
#include <Utility/Int.hpp>

namespace EagleEngine
{
	class HashedString final
	{
	public:

		HashedString();

		HashedString(const char32* _str);

		HashedString(const String& _str);

	public:

		uint64 hash()const noexcept;

		String toString()const;

		bool empty()const noexcept;

		static HashedString Make(const String& tag);

		bool operator==(const HashedString& _other)const noexcept;

		bool operator==(const String& _other)const noexcept;

		bool operator!=(const HashedString& _other)const noexcept;

		bool operator!=(const String& _other)const noexcept;

		explicit operator bool()const noexcept;

	private:

		HashedString(uint64 _hash)noexcept;

		static uint64 MakeHash(const String& tag);

	private:

		uint64 mTagHash;

		static HashTable<uint64, String> sTagTable;

	};
}

template<>
struct std::hash<EagleEngine::HashedString>
{
	[[nodiscard]]
	size_t operator()(const EagleEngine::HashedString& _hashedString)const noexcept
	{
		return _hashedString.hash();
	}
};
