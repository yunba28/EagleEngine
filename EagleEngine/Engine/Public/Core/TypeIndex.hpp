#pragma once

#include <typeinfo>

namespace eagle
{
	class TypeIndex
	{
	public:

		TypeIndex(const std::type_info& inTypeInfo)
			: mTypeInfo(&inTypeInfo)
		{}

		~TypeIndex() = default;

		TypeIndex(TypeIndex&&) = default;
		TypeIndex(const TypeIndex&) = default;
		TypeIndex& operator=(TypeIndex&&) = default;
		TypeIndex& operator=(const TypeIndex&) = default;

	public:

		bool operator==(const TypeIndex& inOther)const noexcept
		{
			return mTypeInfo == inOther.mTypeInfo;
		}

		bool operator!=(const TypeIndex& inOther)const noexcept
		{
			return mTypeInfo != inOther.mTypeInfo;
		}

	public:

		size_t hash()const noexcept
		{
			return mTypeInfo->hash_code();
		}

	private:

		const std::type_info* mTypeInfo;

	};
}

template<>
struct std::hash<eagle::TypeIndex>
{
	size_t operator()(const eagle::TypeIndex& inTypeIndex)const noexcept
	{
		return inTypeIndex.hash();
	}
};
