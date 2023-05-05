#include <Core/Object.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	Object::Object()
		: mName(NoneName)
		, mTypeIndex(typeid(void))
		, mInherited(ObjectInherited::None)
	{
		ensure(RegisterObjectHandle(this), "Failed register object");
	}

	Object::~Object()
	{
		ensure(UnregisterObjectHandle(this), "Failed unregister object");
	}
}
