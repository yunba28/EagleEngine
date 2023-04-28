#pragma once

#include <String/String.hpp>
#include <Utility/Int.hpp>

namespace EagleEngine
{
	class Name;

	using NameID = uint64;

	class NameView final
	{
	public:

		NameView()noexcept;

		NameView(const NameView& _other)noexcept;

		NameView(const Name& _other)noexcept;

		NameView(NameView&& _other)noexcept;

		NameView& operator=(const NameView& _other)noexcept;

		NameView& operator=(const Name& _other)noexcept;

		NameView& operator=(NameView&& _other)noexcept;

	public:

		NameID id()const noexcept;

		uint64 hash()const noexcept;

		String toString()const;

		String getOriginalName()const;

		String getSequentialName()const;

		bool empty()const noexcept;

		bool operator==(const Name& _other)const noexcept;

		bool operator==(const NameView& _other)const noexcept;

		bool operator==(const String& _other)const noexcept;

		bool operator!=(const Name& _other)const noexcept;

		bool operator!=(const NameView& _other)const noexcept;

		bool operator!=(const String& _other)const noexcept;

		explicit operator bool()const noexcept;

	private:

		NameView(NameID _id)noexcept;

	private:

		NameID mNameID;

		friend class Name;

	};
}

template<>
struct std::hash<EagleEngine::NameView>
{
	[[nodiscard]]
	size_t operator()(const EagleEngine::NameView& _name)const noexcept
	{
		return _name.hash();
	}
};
