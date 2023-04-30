#include <Core/Level.hpp>
#include <Core/World.hpp>

namespace EagleEngine
{
	Level::Level()
		: mWorld(nullptr)
	{
	}

	Level::~Level()
	{
	}

	void Level::_internalAttachToWorld(World* _world)
	{
		mWorld = _world;
	}

	ObjectPtr<World> Level::getWorld() const noexcept
	{
		return mWorld;
	}
}
