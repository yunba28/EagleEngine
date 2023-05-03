#include <Core/WorldObject.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	void WorldObject::_internalAttachToLevel(Level* inLevel)
	{
		assert(("The object is already registered in Level", mLevel.invalid()));
		mLevel = inLevel;
	}

	void WorldObject::_internalAttachToOwner(Actor* inOwner)
	{
		mOwner = inOwner;
	}
}
