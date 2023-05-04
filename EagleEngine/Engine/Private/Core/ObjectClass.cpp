#include <Core/ObjectClass.hpp>

#include <Core/Object.hpp>

namespace eagle
{
	Object* ObjectClass::operator()() const noexcept
	{
		Object* object = mFactory();
		{
			object->mTypeIndex = mTypeIndex;
		}
		return object;
	}

	Object* ObjectClass::operator()(const String& newName) const noexcept
	{
		Object* object = mFactory();
		{
			object->setName(newName);
			object->mTypeIndex = mTypeIndex;
		}
		return object;
	}
}
