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
		
	}

	Object::~Object()
	{
	
	}
}
