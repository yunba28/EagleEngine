#include <Core/ObjectClass.hpp>
#include <Core/Object.hpp>

namespace EagleEngine
{

	Object* ObjectClass::operator()() const
	{
		Object* object = mFactory();
		{
			object->mTypeID = mTypeID;
		}
		return object;
	}

	bool ObjectClass::operator==(const ObjectClass& _other) const
	{
		return mTypeID == _other.mTypeID;
	}

	bool ObjectClass::operator!=(const ObjectClass& _other) const
	{
		return mTypeID != _other.mTypeID;
	}

	String ObjectClass::getClassName() const
	{
		return mTypeID.name().removed(U"class ");
	}
}
