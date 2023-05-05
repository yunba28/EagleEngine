#include <GameFramework/Level.hpp>

#include <GameFramework/World.hpp>

namespace eagle
{
	void Level::_internalAttachToWorld(World* newWorld)
	{
		ensure(mWorld.invalid(), "The level is already attached in World");
		mWorld = newWorld;
	}
}
