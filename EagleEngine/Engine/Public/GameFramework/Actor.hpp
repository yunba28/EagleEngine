#pragma once

#include <Core/WorldObject.hpp>
#include <Core/ObjectClass.hpp>

namespace eagle
{
	class Actor : public WorldObject
	{
	public:

		Actor() = default;
		~Actor() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual bool dispose()override { return true; }
		virtual void start()override {}
		virtual void update(double)override {}

	public:

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> createComponent()
		{
			return Cast<ComponentType>(_internalCreateComponent(CreateObjectClass<ComponentType>(),U""), nullptr);
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> createComponent(const String& newName)
		{
			return Cast<ComponentType>(_internalCreateComponent(CreateObjectClass<ComponentType>(), newName), nullptr);
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> attachComponent()
		{
			auto component = _internalCreateComponent(
				CreateObjectClass<ComponentType>(),
				U"{}.Component"_fmt(getName().getSequentialName()),
				this
			);
			return Cast<ComponentType>(component);
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> attachComponent(const String& newName)
		{
			auto component = _internalCreateComponent(
				CreateObjectClass<ComponentType>(),
				newName,
				this
			);
			return Cast<ComponentType>(component);
		}

		ObjectRef<WorldObject> findComponentByOwner(const TypeIndex& inFindType)const;
		ObjectRef<WorldObject> findComponentByName(const TypeIndex& inFindType, const String& inName)const;
		ObjectRef<WorldObject> findComponentByTag(const TypeIndex& inFindType, const String& inTag)const;

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> findComponent()const
		{
			return Cast<ComponentType>(findComponentByOwner(typeid(ComponentType)));
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> findComponentByName(const String& inName)const
		{
			return Cast<ComponentType>(findComponentByName(typeid(ComponentType), inName));
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> findComponentByTag(const String& inTag)const
		{
			return Cast<ComponentType>(findComponentByTag(typeid(ComponentType), inTag));
		}

		template<Concept::IsComponent ComponentType>
		bool detachComponent()
		{
			if (auto found = findComponentByOwner(typeid(ComponentType)); found)
			{
				found->destroy();
				return true;
			}
			return false;
		}

		void LookAtForActor(Actor* inTargetActor);
		void LookAtForComponent(Component* inTargetComponent);

	private:

		ObjectRef<WorldObject> _internalCreateComponent(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner);

	};

	template<Concept::IsActor ActorType>
	ObjectClass CreateActorClass()
	{
		return CreateObjectClass<ActorType>();
	}
}
