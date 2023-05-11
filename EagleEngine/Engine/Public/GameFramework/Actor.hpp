#pragma once

#include <Core/WorldObject.hpp>

namespace eagle
{
	class Actor : public WorldObject
	{
	public:

		Actor() = default;
		~Actor() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual void start()override {}
		virtual void update(double)override {}
		virtual bool dispose()override { return true; }

	private:

	};

	template<Concept::IsActor ActorType>
	ObjectClass CreateActorClass()
	{
		return CreateObjectClass<ActorType>();
	}
}
