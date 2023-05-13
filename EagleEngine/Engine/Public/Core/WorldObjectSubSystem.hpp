#pragma once

#include <GameFramework/SubSystem.hpp>
#include <Core/ObjectClass.hpp>
#include <Container/Array.hpp>
#include <Container/HashTable.hpp>
#include <Misc/Timer.hpp>

namespace eagle
{
	class WorldObjectSubSystem : public SubSystem
	{
	private:

		class WorldObjectListener
		{
		public:

			WorldObjectListener() = default;
			~WorldObjectListener();

			WorldObjectListener(WorldObjectListener&&) = default;
			WorldObjectListener(const WorldObjectListener&) = delete;
			WorldObjectListener& operator=(WorldObjectListener&&) = default;
			WorldObjectListener& operator=(const WorldObjectListener&) = delete;

		public:

			void update(double inDeltaTime);
			void addWorldObject(WorldObject* inWorldObject);
			ObjectRef<WorldObject> getByName(const String& inName)const;
			ObjectRef<WorldObject> getByTag(const String& inTag)const;
			Array<ObjectRef<WorldObject>> getsByTag(const String& inTag)const;

		private:

			Array<ObjectPtr<WorldObject>> mObjects = {};
			Array<ObjectPtr<WorldObject>> mQueue = {};

			bool mHasPendingKill = false;

		};

	public:

		WorldObjectSubSystem() = default;
		~WorldObjectSubSystem();

	public:

		void update(double inDeltaTime)override final;
		ObjectRef<WorldObject> createObject(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner = nullptr);

		template<Concept::IsActor ActorType = Actor>
		ObjectRef<ActorType> createActor(const String& newName)
		{
			ObjectClass objectClass = CreateObjectClass<ActorType>();
			return Cast<ActorType>(createObject(objectClass, newName));
		}

		template<Concept::IsActor ActorType = Actor>
		ObjectRef<ActorType> createActor(const String& newName, Actor* newOwner)
		{
			ObjectClass objectClass = CreateObjectClass<ActorType>();
			return Cast<ActorType>(createObject(objectClass, newName, newOwner));
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> createComponent(Actor* newOwner)
		{
			ObjectClass objectClass = CreateObjectClass<ComponentType>();
			return Cast<ComponentType>(createObject(objectClass, U"", newOwner));
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> createComponent(Actor* newOwner, const String& newName)
		{
			ObjectClass objectClass = CreateObjectClass<ComponentType>();
			return Cast<ComponentType>(createObject(objectClass, newName, newOwner));
		}

		template<Concept::IsSubLevel SubLevelType>
		ObjectRef<SubLevelType> createSubLevel(const String& newName = U"")
		{
			ObjectClass objectClass = CreateObjectClass<SubLevelType>();
			return Cast<SubLevelType>(createObject(objectClass, newName, nullptr));
		}

	public:

		static void HitStop(double inTimeDilation, double inStopTimeSec);

	private:

		HashTable<TypeIndex, WorldObjectListener> mWorldObjectTable = {};

		Array<TypeIndex> mExecutionOrder = {};
		Array<TypeIndex> mOrderQueue = {};

		static Timer sHitStopTimer;

	};
}
