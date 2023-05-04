#pragma once

#include <GameFramework/SubSystem.hpp>
#include <Core/ObjectClass.hpp>
#include <Container/Array.hpp>
#include <Container/HashTable.hpp>

namespace eagle
{
	class WorldObjectSubSystem : public SubSystem
	{
	private:

		class WorldObjectListener
		{
		public:

			WorldObjectListener() = default;
			~WorldObjectListener() = default;

			WorldObjectListener(WorldObjectListener&&) = default;
			WorldObjectListener(const WorldObjectListener&) = delete;
			WorldObjectListener& operator=(WorldObjectListener&&) = default;
			WorldObjectListener& operator=(const WorldObjectListener&) = delete;

		public:

			void update(double inDeltaTime);
			void addWorldObject(WorldObject* inWorldObject);

		private:

			Array<ObjectPtr<WorldObject>> mObjects = {};
			Array<ObjectPtr<WorldObject>> mQueue = {};

			bool mHasPendingKill = false;

		};

	public:

		WorldObjectSubSystem() = default;
		~WorldObjectSubSystem() = default;

	public:

		void update(double inDeltaTime)override final;
		ObjectPtr<Object> createObject(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner = nullptr);

		template<Concept::IsActor ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& newName)
		{
			ObjectClass objectClass = CreateObjectClass<ActorType>();
			return Cast<ActorType>(createObject(objectClass, newName));
		}

		template<Concept::IsActor ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& newName, Actor* newOwner)
		{
			ObjectClass objectClass = CreateObjectClass<ActorType>();
			return Cast<ActorType>(createObject(objectClass, newName, newOwner));
		}

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* newOwner)
		{
			ObjectClass objectClass = CreateObjectClass<ComponentType>();
			return Cast<ComponentType>(createObject(objectClass, U"", newOwner));
		}

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* newOwner, const String& newName)
		{
			ObjectClass objectClass = CreateObjectClass<ComponentType>();
			return Cast<ComponentType>(createObject(objectClass, newName, newOwner));
		}

		template<Concept::IsSubLevel SubLevelType>
		ObjectPtr<SubLevelType> createSubLevel(const String& newName = U"")
		{
			ObjectClass objectClass = CreateObjectClass<SubLevelType>();
			return Cast<SubLevelType>(createObject(objectClass, newName, nullptr));
		}

	private:

		HashTable<TypeIndex, WorldObjectListener> mWorldObjectTable = {};

		Array<TypeIndex> mExecutionOrder = {};
		Array<TypeIndex> mOrderQueue = {};

	};
}
