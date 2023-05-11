#pragma once

#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	class Level : public LevelBase
	{
	public:

		Level() = default;
		~Level() = default;

	protected:

		virtual void awake()override {}
		virtual void dispose()override {}
		virtual void update([[maybe_unused]] double inDeltaTime)override {}
		virtual void draw()const override{}

	public:

		virtual void _internalAttachToWorld(World* newWorld);

	public:

		ObjectRef<World> getWorld()const noexcept
		{
			return mWorld;
		}

	private:

		ObjectRef<World> mWorld;

	};

	template<Concept::IsLevel LevelType>
	ObjectClass CreateLevelClass()
	{
		return CreateObjectClass<LevelType>();
	}
}
