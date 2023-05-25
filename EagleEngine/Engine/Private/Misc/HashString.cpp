#include <Misc/HashString.hpp>

#include <Container/HashTable.hpp>

namespace eagle
{

	HashTable<size_t,String> gStringTable
	{
		{0,U""_s}
	};

	size_t RegisterString(const String& inString)
	{
		size_t hash = inString.hash();
		gStringTable.emplace(hash, inString);
		return hash;
	}

	HashString::HashString()
		: mHash(0)
	{
	}

	HashString::~HashString()
	{
	}

	HashString::HashString(const String& inString)
		: mHash(RegisterString(inString))
	{}

	HashString& HashString::operator=(const String& inString)
	{
		mHash = RegisterString(inString);
		return *this;
	}

	String HashString::toString() const
	{
		if (gStringTable.contains(mHash))
		{
			return gStringTable.at(mHash);
		}
		return String();
	}
}
