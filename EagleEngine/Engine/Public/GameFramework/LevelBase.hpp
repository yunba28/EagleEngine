#pragma once

#include <Core/Object.hpp>
#include <Concept/IsType.hpp>
#include <Container/Array.hpp>

namespace eagle
{
	class WorldObjectSubSystem;
	class RendererSubSystem;

	class LevelBase : public Object
	{
	public:

		LevelBase();
		~LevelBase();

	protected:

		virtual void update([[maybe_unused]] double inDeltaTime) = 0;

	public:

		virtual void _internalUpdate(double inDeltaTime);
		virtual void _internalDraw()const;

	public:

		ObjectRef<SubSystem> addSubSystem(const ObjectClass& inObjectClass);
		ObjectRef<WorldObject> createActor(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner);
		ObjectRef<WorldObject> createComponent(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner);
		ObjectRef<WorldObject> createSubLevel(const ObjectClass& inObjectClass, const String& newName);

		template<Concept::IsSubSystem SubSystemType>
		ObjectRef<SubSystemType> addSubSystem()
		{
			return Cast<SubSystemType>(addSubSystem(CreateObjectClass<SubSystemType>()));
		}

		template<class ActorType>
		ObjectRef<ActorType> createActor(const String& newName)
		{
			return Cast<ActorType>(createActor(CreateObjectClass<ActorType>(), newName, nullptr));
		}

		template<class ActorType>
		ObjectRef<ActorType> createActor(const String& newName, Actor* newOwner)
		{
			return Cast<ActorType>(createActor(CreateObjectClass<ActorType>(), newName, newOwner));
		}

		template<class ComponentType>
		ObjectRef<ComponentType> createComponent(Actor* newOwner)
		{
			return Cast<ComponentType>(createComponent(CreateObjectClass<ComponentType>(), U"", newOwner));
		}

		template<class ComponentType>
		ObjectRef<ComponentType> createComponent(Actor* newOwner, const String& newName)
		{
			return Cast<ComponentType>(createComponent(CreateObjectClass<ComponentType>(), newName, newOwner));
		}

		template<class SubLevelType>
		ObjectRef<SubLevelType> createSubLevel(const String& newName = U"")
		{
			return Cast<SubLevelType>(createSubLevel(CreateObjectClass<SubLevelType>(), newName));
		}

		ObjectRef<WorldObject> findByName(const TypeIndex& inFindType, const String& inName)const;
		ObjectRef<WorldObject> findByTag(const TypeIndex& inFindType, const String& inTag)const;
		Array<ObjectRef<WorldObject>> findsByTag(const String& inTag)const;
		Array<ObjectRef<WorldObject>> findsByTag(const TypeIndex& inFindType, const String& inTag)const;
		ObjectRef<WorldObject> findByOwner(const TypeIndex& inFindType, const Actor* const inOwner)const;
		Array<ObjectRef<WorldObject>> findsByOwner(const Actor* const inOwner)const;

		ObjectRef<SubSystem> findSubSystem(const TypeIndex& inFindType)const;

		template<Concept::IsSubSystem SubSystemType>
		ObjectRef<SubSystemType> findSubSystem()const
		{
			return Cast<SubSystemType>(findSubSystem(typeid(SubSystemType)));
		}

		ObjectRef<WorldObjectSubSystem> getWorldObjectSubSystem()const noexcept
		{
			return mWorldObjectSubSystem;
		}

		ObjectRef<RendererSubSystem> getRendererSubSystem()const noexcept
		{
			return mRendererSubSystem;
		}

	private:

		Array<ObjectPtr<SubSystem>> mSubSystems;

		ObjectRef<WorldObjectSubSystem> mWorldObjectSubSystem;

		ObjectRef<RendererSubSystem> mRendererSubSystem;

		friend class Level;
		friend class SubLevel;

	};
}
