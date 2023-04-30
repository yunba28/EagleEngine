#pragma once

#include <Core/LevelBase.hpp>

namespace EagleEngine
{
	class Level : public LevelBase
	{
	public:

		Level();

		~Level();

	public:

		virtual void _internalAttachToWorld(World* _world);

	public:

		ObjectPtr<World> getWorld()const noexcept;

	private:

		ObjectPtr<World> mWorld;

	};
}
