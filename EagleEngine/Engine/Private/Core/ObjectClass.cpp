#include <Core/ObjectClass.hpp>

#include <Core/Object.hpp>

namespace eagle
{
	Object* ObjectClass::operator()() const noexcept
	{
		Object* object = mFactory();
		{
			object->mTypeIndex = mTypeIndex;
			object->mInherited = mInherited;
		}
		return object;
	}

	Object* ObjectClass::operator()(const String& newName) const noexcept
	{
		Object* object = mFactory();
		{
			object->setName(newName);
			object->mTypeIndex = mTypeIndex;
			object->mInherited = mInherited;
		}
		return object;
	}
}
