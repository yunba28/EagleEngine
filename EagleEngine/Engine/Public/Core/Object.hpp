#pragma once

#include <CoreUtility.hpp>
#include <Core/Name.hpp>
#include <Core/ObjectPtr.hpp>
#include <Core/ObjectInherited.hpp>
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

	protected:

		virtual bool awake() = 0;
		virtual bool dispose() = 0;

	public:

		virtual void _internalConstruct();
		virtual void _internalDestruct();


	public:

		void setName(const String& inName)
		{
			mName = Name(inName);
		}

		NameView getName()const noexcept
		{
			return mName;
		}

		bool sameName(const String& inName)const noexcept
		{
			return mName == inName;
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

		ObjectInherited inherited()const noexcept
		{
			return mInherited;
		}

		bool hasInherited(ObjectInherited inInherited)const noexcept
		{
			const uint8 bits = static_cast<uint8>(inInherited);
			return static_cast<uint8>(inInherited) & bits;
		}

		bool invalid()const noexcept
		{
			return !IsValidByObject(this);
		}

	private:

		Name mName;

		TypeIndex mTypeIndex;

		ObjectInherited mInherited;

		friend class ObjectClass;

	};
}

