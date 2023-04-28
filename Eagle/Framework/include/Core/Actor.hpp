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

		/* category 'advanced' */

		virtual void updateFadeIn([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) {}

		virtual void updateFadeOut([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) {}

	public:

		virtual void _internalDestruct()override;

	public:

		/* category 'common' */

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

	private:

		ObjectPtr<Component> attachComponentByObjectClass(const ObjectClass& _objectClass, const String& _name);

	private:

		Array<ObjectPtr<Component>> mComponents;

	};

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
}
