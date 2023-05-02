#pragma once

#include "CoreTypes.hpp"
#include "Container/String.hpp"

namespace eagle
{
	class Name;
	class NameView;

	using NameID = size_t;

	struct NameRow
	{
		String name = U"";
		NameID base = 0ULL;
		int32  count = 0;
		int32  max = 0;
	};

	struct NoneName_t {}constexpr inline NoneName;

	class NameBase
	{
	public:

		NameBase() = default;
		~NameBase() = default;

		NameBase(NoneName_t)noexcept
			: mNameID(0)
		{}

	public:

		bool operator==(const NameBase& inOther)const noexcept
		{
			return mNameID == inOther.mNameID;
		}

		bool operator==(const String& inOther)const noexcept
		{
			return mNameID == inOther.hash();
		}

		friend bool operator==(const String& inLeft, const NameBase& inRight)noexcept
		{
			return inLeft.hash() == inRight.mNameID;
		}

		bool operator==(NoneName_t)const noexcept
		{
			return mNameID == 0;
		}

		bool operator!=(const NameBase& inOther)const noexcept
		{
			return mNameID != inOther.mNameID;
		}

		bool operator!=(const String& inOther)const noexcept
		{
			return mNameID != inOther.hash();
		}

		friend bool operator!=(const String& inLeft, const NameBase& inRight)noexcept
		{
			return inLeft.hash() != inRight.mNameID;
		}

		bool operator!=(NoneName_t)const noexcept
		{
			return mNameID != 0;
		}

		explicit operator bool()const noexcept
		{
			return mNameID != 0;
		}

	public:

		NameID id()const noexcept
		{
			return mNameID;
		}

		size_t hash()const noexcept
		{
			return mNameID;
		}

	public:

		static int32 Make(const String& inName, Name& outName);

		static void Release(Name& inName);

		static bool Exist(NameID inNameID);

		static bool Exist(const String& inName);

		static bool Find(NameID inNameID, String& outName);

		static bool Find(const String& inName, String& outName);

	protected:

		NameID mNameID = 0;

	};

	class NameView final : public NameBase
	{
	public:

		NameView() = default;
		~NameView() = default;

		NameView(NameView&&)noexcept = default;
		NameView(const NameView&)noexcept = default;
		NameView& operator=(NameView&&)noexcept = default;
		NameView& operator=(const NameView&)noexcept = default;

		NameView& operator=(NoneName_t)noexcept
		{
			mNameID = 0;
			return *this;
		}

	public:



	};

	class Name final : public NameBase
	{
	public:

	private:

	};
}
