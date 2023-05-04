#pragma once

#include <CoreUtility.hpp>
#include <Core/Name.hpp>
#include <Core/ObjectPtr.hpp>
#include <Core/TypeIndex.hpp>

namespace eagle
{
	class Object
	{
	public:

		explicit Object();
		virtual ~Object() = 0;

		Object(Object&&) = default;
		Object(const Object&) = delete;
		Object& operator=(Object&&) = default;
		Object& operator=(const Object&) = delete;

	public:

		bool operator==(const Object& inOther)const noexcept
		{
			return this == &inOther;
		}

		bool operator!=(const Object& inOther)const noexcept
		{
			return this != &inOther;
		}

		explicit operator bool()const
		{
			return IsValidByObject(this);
		}

	public:

		void setName(const String& inName)
		{
			mName = Name(inName);
		}

		NameView getName()const noexcept
		{
			return mName;
		}

		const TypeIndex& type()const noexcept
		{
			return mTypeIndex;
		}

		bool isA(const Object* const inObject)const noexcept
		{
			return mTypeIndex == inObject->mTypeIndex;
		}

		bool isA(const TypeIndex& inTypeIndex)const noexcept
		{
			return mTypeIndex == inTypeIndex;
		}

		bool invalid()const noexcept
		{
			return !IsValidByObject(this);
		}

	private:

		Name mName;

		TypeIndex mTypeIndex;

		friend class ObjectClass;

	};
}
