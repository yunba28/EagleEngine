#pragma once

#include <CoreTypes.hpp>
#include <Container/String.hpp>

namespace eagle
{
	class HashString
	{
	public:

		HashString();
		~HashString();

		HashString(HashString&&) = default;
		HashString(const HashString&) = default;
		HashString& operator=(HashString&&) = default;
		HashString& operator=(const HashString&) = default;

		HashString(const String& inString);
		HashString& operator=(const String& inString);

	public:

		bool operator==(const HashString& inOther)const noexcept
		{
			return mHash == inOther.mHash;
		}

		bool operator==(const String& inOther)const noexcept
		{
			return mHash == inOther.hash();
		}

		friend bool operator==(const String& inLeft, const HashString& inRight)noexcept
		{
			return inLeft.hash() == inRight.mHash;
		}

		bool operator!=(const HashString& inOther)const noexcept
		{
			return mHash != inOther.mHash;
		}

		bool operator!=(const String& inOther)const noexcept
		{
			return mHash != inOther.hash();
		}

		friend bool operator!=(const String& inLeft, const HashString& inRight)noexcept
		{
			return inLeft.hash() != inRight.mHash;
		}

		explicit operator bool()const noexcept
		{
			return mHash != 0;
		}

	public:

		size_t hash()const noexcept
		{
			return mHash;
		}

		String toString()const;

	private:

		size_t mHash = 0;

	};
}

template<>
struct std::hash<eagle::HashString>
{
	size_t operator()(const eagle::HashString& inString)const noexcept
	{
		return inString.hash();
	}
};
