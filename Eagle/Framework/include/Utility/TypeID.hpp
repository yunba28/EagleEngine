#pragma once

#include <typeinfo>
#include <String/String.hpp>

namespace EagleEngine
{
	class TypeID final
	{
	public:

		TypeID(const std::type_info& _type) noexcept : mTypeInfo(&_type) {}

	public:

		[[nodiscard]]
		size_t hash()const noexcept { return mTypeInfo->hash_code(); }

		[[nodiscard]]
		String name()const;

		bool operator==(const TypeID& _other)const noexcept { return (*mTypeInfo) == (*_other.mTypeInfo); }

		bool operator!=(const TypeID& _other)const noexcept { return (*mTypeInfo) != (*_other.mTypeInfo); }

		bool operator<(const TypeID& _other)const noexcept { return mTypeInfo->before(*_other.mTypeInfo); }

		bool operator>=(const TypeID& _other)const noexcept { return !mTypeInfo->before(*_other.mTypeInfo); }

		bool operator>(const TypeID& _other)const noexcept { return !((*this) < _other); }

		bool operator<=(const TypeID& _other)const noexcept { return !((*this) >= _other); }

	private:

		const std::type_info* mTypeInfo;

	};
}

template<>
struct std::hash<EagleEngine::TypeID>
{
	[[nodiscard]]
	size_t operator()(const EagleEngine::TypeID& _type)const noexcept
	{
		return _type.hash();
	}
};
