#pragma once

#include <Core/Object.hpp>

namespace EagleEngine
{
	class Actor : public Object
	{
	public:

		Actor();

	protected:

		/* category 'event' */

		virtual bool awake()override { return true; }

		virtual void start()override {}

		virtual void update([[maybe_unused]] double _deltaTime)override {}

		virtual bool dispose()override { return true; }

	public:

		virtual void _internalDestruct()override;

		virtual void _internalAttachToActor(Actor* _actor)override;

	public:

		/* category 'common' */

		void attachToActor(Actor* _owner);

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> attachComponent(const String& _name = U"");

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> getComponent()const;

		ObjectPtr<Component> getComponentByTypeID(const TypeID& _type)const;

		ObjectPtr<Component> getComponentByName(const String& _name)const;

		Array<ObjectPtr<Component>> getComponentsByTag(const String& _tag)const;

		template<Concept::IsComponent ComponentType>
		void detachComponent();

		void detachComponentByTypeID(const TypeID& _type);

		void detachComponentByName(const String& _name);

		void detachComponentsByTag(const String& _tag);

		template<Concept::IsComponent ComponentType>
		bool hasComponent()const;

		bool hasComponentByTypeID(const TypeID& _type)const;

		bool hasComponentByName(const String& _name)const;

		bool hasComponentByTag(const String& _tag)const;

		void setTransform(const Transform& _transform)noexcept;

		const Transform& getTransform()const noexcept;

		void setActorPosition(const Vector3& _position)noexcept;

		Vector3 getActorPosition()const noexcept;

		void setActorRotation(const Quaternion& _rotation)noexcept;

		Quaternion getActorRotation()const noexcept;

		void setActorScale(const Vector3& _scale)noexcept;

		Vector3 getActorScale()const noexcept;

	private:

		ObjectPtr<Component> attachComponentByObjectClass(const ObjectClass& _objectClass, const String& _name);

	private:

		Array<ObjectPtr<Component>> mComponents;

		UniquePtr<Transform> mTransform;

	};

	inline void Actor::attachToActor(Actor* _owner)
	{
		_internalAttachToActor(_owner);
	}

	template<Concept::IsComponent ComponentType>
	inline ObjectPtr<ComponentType> Actor::attachComponent(const String& _name)
	{
		ObjectPtr<Component> component = attachComponentByObjectClass(CreateObjectClass<ComponentType>(), _name);
		{
			mComponents.push_back(component);
		}
		return Cast<ComponentType>(component);
	}

	template<Concept::IsComponent ComponentType>
	inline ObjectPtr<ComponentType> Actor::getComponent() const
	{
		return Cast<ComponentType>(getComponentByTypeID(typeid(ComponentType)));
	}

	template<Concept::IsComponent ComponentType>
	inline void Actor::detachComponent()
	{
		detachComponentByTypeID(typeid(ComponentType));
	}

	template<Concept::IsComponent ComponentType>
	inline bool Actor::hasComponent() const
	{
		return hasComponentByTypeID(typeid(ComponentType));
	}

	inline void Actor::setTransform(const Transform& _transform)noexcept
	{
		(*mTransform) = _transform;
	}

	inline const Transform& Actor::getTransform()const noexcept
	{
		return *mTransform;
	}

	inline void Actor::setActorPosition(const Vector3& _position)noexcept
	{
		mTransform->setPosition(_position);
	}

	inline Vector3 Actor::getActorPosition()const noexcept
	{
		return mTransform->getPosition();
	}

	inline void Actor::setActorRotation(const Quaternion& _rotation)noexcept
	{
		mTransform->setRotation(_rotation);
	}

	inline Quaternion Actor::getActorRotation()const noexcept
	{
		return mTransform->getRotation();
	}

	inline void Actor::setActorScale(const Vector3& _scale)noexcept
	{
		mTransform->setScale(_scale);
	}

	inline Vector3 Actor::getActorScale()const noexcept
	{
		return mTransform->getScale();
	}

}
