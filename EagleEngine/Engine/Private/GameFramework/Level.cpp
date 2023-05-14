#include <GameFramework/Level.hpp>

#include <GameFramework/World.hpp>
#include <Core/RendererSubSystem.hpp>

namespace eagle
{
	Level::Level()
		: mWorld()
	{
		mRendererSubSystem = addSubSystem<RendererSubSystem>();
		ensure(mRendererSubSystem != nullptr, "Failed create RendererSubSystem");
	}

	void Level::_internalAttachToWorld(World* newWorld)
	{
		ensure(mWorld.invalid(), "The level is already attached in World");
		mWorld = newWorld;
	}
}
