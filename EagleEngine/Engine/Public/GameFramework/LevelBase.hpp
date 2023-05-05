#pragma once

#include <Core/WorldObjectSubSystem.hpp>

namespace eagle
{
	class LevelBase : public Object
	{
	public:

		LevelBase();
		~LevelBase();

	protected:

		virtual void update([[maybe_unused]] double inDeltaTime) {};
		virtual void draw()const {};

	public:

		virtual void _internalUpdate(double inDeltaTime);
		virtual void _internalDraw()const;

	public:

		ObjectPtr<SubSystem> addSubSystem(const ObjectClass& inObjectClass);

		template<Concept::IsSubSystem SubSystemType>
		ObjectPtr<SubSystemType> addSubSystem()
		{
			return Cast<SubSystemType>(addSubSystem(CreateObjectClass<SubSystemType>()));
		}

		template<class ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& newName)
		{
			return mWorldObjectSubSystem->createActor<ActorType>(newName);
		}

		template<class ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& newName, Actor* newOwner)
		{
			return mWorldObjectSubSystem->createActor<ActorType>(newName, newOwner);
		}

		template<class ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* newOwner)
		{
			return mWorldObjectSubSystem->createComponent<ComponentType>(newOwner);
		}

		template<class ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* newOwner, const String& newName)
		{
			return mWorldObjectSubSystem->createComponent<ComponentType>(newOwner, newName);
		}

		template<class SubLevelType>
		ObjectPtr<SubLevelType> createSubLevel(const String& newName = U"")
		{
			return mWorldObjectSubSystem->createSubLevel<SubLevelType>(newName);
		}

	private:

		Array<ObjectPtr<SubSystem>> mSubSystems;

		ObjectPtr<WorldObjectSubSystem> mWorldObjectSubSystem;

	};
}
