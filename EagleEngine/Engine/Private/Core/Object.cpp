#include <Core/Object.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	Object::Object()
		: mName(NoneName)
		, mTypeIndex(typeid(void))
	{
#if _DEBUG
		assert(("Failed register object", RegisterObjectHandle(this)));
#else
		RegisterObjectHandle(this);
#endif
	}

	Object::~Object()
	{
#if _DEBUG
		assert(("Failed unregister object", UnregisterObjectHandle(this)));
#else
		UnregisterObjectHandle(this);
#endif
	}
}
