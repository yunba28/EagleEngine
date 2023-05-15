#pragma once

#include <Core/WorldObject.hpp>
#include <Core/ObjectClass.hpp>
#include <Math/Transform.hpp>

namespace eagle
{
	class Actor : public WorldObject, public ITransformable
	{
	public:

		Actor() = default;
		~Actor() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual void start()override {}
		virtual void update(double)override {}
		virtual bool dispose()override { return true; }

	public:

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> attachComponent()
		{
			return Cast<ComponentType>(createComponent(
				CreateObjectClass<ComponentType>(),
				U"{}.Component"_fmt(getName().getSequentialName())
			));
		}

		template<Concept::IsComponent ComponentType>
		ObjectRef<ComponentType> attachComponent(const String& newName)
		{
			return Cast<ComponentType>(createComponent(
				CreateObjectClass<ComponentType>(),
				newName
			));
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

		/*--- ITransformable --------------------------------------------------*/

		const Transform& getTransform()const override
		{
			return mTransform;
		}

		ObjectRef<Actor> getParent()const override
		{
			return getOwner();
		}

		/*--- Transform -------------------------------------------------------*/

		void setActorTransform(const Transform& newTransform)noexcept
		{
			mTransform = newTransform;
		}

		const Transform& getActorTransform()const noexcept
		{
			return mTransform;
		}

		/*---------------------------------------------------------------------*/

	private:

		ObjectRef<WorldObject> createComponent(const ObjectClass& inObjectClass, const String& newName);

	private:

		Transform mTransform = Transform::Identity();

	};

	template<Concept::IsActor ActorType>
	ObjectClass CreateActorClass()
	{
		return CreateObjectClass<ActorType>();
	}
}
