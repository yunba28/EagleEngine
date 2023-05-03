#pragma once

#include <CoreUtility.hpp>
#include <Core/Name.hpp>
#include <Core/ObjectPtr.hpp>

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

		bool invalid()const noexcept
		{
			return !IsValidByObject(this);
		}

	private:

		Name mName;

	};
}
