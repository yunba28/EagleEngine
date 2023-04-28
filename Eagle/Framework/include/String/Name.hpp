#pragma once

#include <Container/HashTable.hpp>
#include <String/NameView.hpp>

namespace EagleEngine
{

	class Name final
	{
	public:

		Name();

		Name(const char32* _name);

		Name(const String& _name);

		~Name();

		Name(const Name&) = delete;

		Name& operator=(const Name&) = delete;

		Name(Name&& _other) noexcept;

		Name& operator=(Name&& _other)noexcept;

	private:

		struct Param final
		{
			String name = U""_s;
			NameID base = 0;
			int32 count = 0;
			int32 max = 0;
			bool used = false;
		};

	public:

		NameID id()const noexcept;

		uint64 hash()const noexcept;

		String toString()const;

		String getOriginalName()const;

		String getSequentialName()const;

		bool empty()const noexcept;

		static Name Make(const String& name);

		static NameView Find(const String& name);

		static void Release(Name& name);

		static void ReleaseAll();

		static bool Used(const String& name);

		static bool Used(NameID id);

		bool operator==(const Name& _other)const noexcept;

		bool operator==(const NameView& _other)const noexcept;

		bool operator==(const String& _other)const noexcept;

		bool operator!=(const Name& _other)const noexcept;

		bool operator!=(const NameView& _other)const noexcept;

		bool operator!=(const String& _other)const noexcept;

		explicit operator bool()const noexcept;

	private:

		Name(NameID _id)noexcept;

		static NameID MakeID(const String& name);

		static NameID ExchangeID(Name&& name);

	private:

		NameID mNameID;

		static HashTable<NameID, Param> sNameTable;

		friend class NameView;
	};
}

template<>
struct std::hash<EagleEngine::Name>
{
	[[nodiscard]]
	size_t operator()(const EagleEngine::Name& _name)const noexcept
	{
		return _name.hash();
	}
};
