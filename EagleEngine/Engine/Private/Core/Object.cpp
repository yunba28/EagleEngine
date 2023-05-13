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

	void Object::_internalConstruct()
	{
		ensure(awake(), "Failed to execute the awake function");
	}

	void Object::_internalDestruct()
	{
		ensure(dispose(), "Failed to execute the dispose function");
		Name::Release(mName);
	}
}
