#include <String/HashedString.hpp>

namespace EagleEngine
{
	HashTable<uint64, String> HashedString::sTagTable
	{
		{0,U""_s}
	};

	HashedString::HashedString()
		: mTagHash(0)
	{
	}

	HashedString::HashedString(const char32* _str)
		: mTagHash(HashedString::MakeHash(_str))
	{
	}

	HashedString::HashedString(const String& _str)
		: mTagHash(HashedString::MakeHash(_str))
	{
	}

	uint64 HashedString::hash() const noexcept
	{
		return mTagHash;
	}

	String HashedString::toString() const
	{
		if (HashedString::sTagTable.contains(mTagHash))
		{
			return HashedString::sTagTable.at(mTagHash);
		}

		return String{};
	}

	bool HashedString::empty() const noexcept
	{
		return mTagHash == 0;
	}

	HashedString HashedString::Make(const String& tag)
	{
		return HashedString{ HashedString::MakeHash(tag) };
	}

	bool HashedString::operator==(const HashedString& _other) const noexcept
	{
		return mTagHash == _other.mTagHash;
	}

	bool HashedString::operator==(const String& _other) const noexcept
	{
		return mTagHash == _other.hash();
	}

	bool HashedString::operator!=(const HashedString& _other) const noexcept
	{
		return mTagHash != _other.mTagHash;
	}

	bool HashedString::operator!=(const String& _other) const noexcept
	{
		return mTagHash != _other.hash();
	}

	HashedString::operator bool() const noexcept
	{
		return mTagHash != 0;
	}

	HashedString::HashedString(uint64 _hash)noexcept
		: mTagHash(_hash)
	{

	}

	uint64 HashedString::MakeHash(const String& tag)
	{
		if (tag == U"")
			return 0ULL;

		uint64 hash = tag.hash();

		if (!HashedString::sTagTable.contains(hash))
		{
			HashedString::sTagTable.emplace(hash, tag);
		}

		return hash;
	}
}
