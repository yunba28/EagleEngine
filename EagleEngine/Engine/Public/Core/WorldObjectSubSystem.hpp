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

			WorldObjectListener();
			~WorldObjectListener();

			WorldObjectListener(const ObjectClass& inObjectClass);

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
			ObjectRef<WorldObject> getByOwner(const Actor* const inOwner)const;
			Array<ObjectRef<WorldObject>> getsByOwner(const Actor* const inOwner)const;

			bool hasInherited(ObjectInherited inInherited)const noexcept
			{
				return static_cast<uint8>(mInherited) & static_cast<uint8>(inInherited);
			}

		private:

			Array<ObjectPtr<WorldObject>> mObjects;
			Array<ObjectPtr<WorldObject>> mQueue;

			const ObjectInherited mInherited;

			const TypeIndex mTypeIndex;

			bool mHasPendingKill;

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

		ObjectRef<WorldObject> findByName(const TypeIndex& inFindType, const String& inName)const;

		template<Concept::IsWorldObject WorldObjectType>
		ObjectRef<WorldObjectType> findByName(const String& inName)const
		{
			return Cast<WorldObjectType>(findByName(typeid(WorldObjectType), inName));
		}

		ObjectRef<WorldObject> findByTag(const TypeIndex& inFindType, const String& inTag)const;

		template<Concept::IsWorldObject WorldObjectType>
		ObjectRef<WorldObjectType> findByTag(const String& inTag)const
		{
			return Cast<WorldObjectType>(findByTag(typeid(WorldObjectType), inTag));
		}

		Array<ObjectRef<WorldObject>> findsByTag(const String& inTag)const;
		Array<ObjectRef<WorldObject>> findsByTag(const TypeIndex& inFindType, const String& inTag)const;

		template<Concept::IsWorldObject WorldObjectType>
		Array<ObjectRef<WorldObjectType>> findsByTag(const String& inTag)const
		{
			if (auto founds = findsByTag(typeid(WorldObjectType), inTag); !founds.isEmpty())
			{
				return founds.map([](const ObjectRef<WorldObject>& inObject)
				{
					return Cast<WorldObjectType>(inObject);
				});
			}

			return Array<ObjectRef<WorldObjectType>>{};
		}

		ObjectRef<WorldObject> findByOwner(const TypeIndex& inFindType, const Actor* const inOwner)const;

		template<Concept::IsWorldObject WorldObjectType>
		ObjectRef<WorldObjectType> findByOwer(const Actor* const inOwner)const
		{
			return Cast<WorldObjectType>(findByOwer(typeid(WorldObjectType), inOwner));
		}

		Array<ObjectRef<WorldObject>> findsByOwner(const Actor* const inOwner)const;

	public:

		static void HitStop(double inTimeDilation, double inStopTimeSec);

	private:

		HashTable<TypeIndex, WorldObjectListener> mWorldObjectTable = {};

		Array<TypeIndex> mExecutionOrder = {};
		Array<TypeIndex> mOrderQueue = {};

		static Timer sHitStopTimer;

	};
}
