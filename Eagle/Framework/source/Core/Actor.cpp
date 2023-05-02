#include <Core/Actor.hpp>
#include <Core/Component.hpp>
#include <Core/LevelBase.hpp>

namespace EagleEngine
{
	Actor::Actor()
		: m_components()
		, m_transform(MakeUnique<Transform>())
	{
	}

	void Actor::_internalDestruct()
	{
		Object::_internalDestruct();
		for (const auto& component : m_components)
		{
			component->destroy();
		}
	}

	void Actor::_internalAttachToActor(Actor* _actor)
	{
		Object::_internalAttachToActor(_actor);
	}

	ObjectPtr<Component> Actor::getComponentByTypeID(const TypeID& _type) const
	{
		auto found = std::find_if(m_components.begin(), m_components.end(), [&_type](const ObjectPtr<Component>& component)
		{
			return component->sameTypeID(_type);
		});

		if (found != m_components.end())
		{
			return (*found);
		}

		return ObjectPtr<Component>(nullptr);
	}

	ObjectPtr<Component> Actor::getComponentByName(const String& _name) const
	{
		auto found = std::find_if(m_components.begin(), m_components.end(), [&_name](const ObjectPtr<Component>& component)
		{
			return component->sameName(_name);
		});

		if (found != m_components.end())
		{
			return (*found);
		}

		return ObjectPtr<Component>{};
	}

	Array<ObjectPtr<Component>> Actor::getComponentsByTag(const String& _tag) const
	{
		return m_components.filter([&_tag](const ObjectPtr<Component>& component)
		{
			return component->hasTag(_tag);
		});
	}

	void Actor::detachComponentByTypeID(const TypeID& _type)
	{
		m_components.remove_if([&_type](const ObjectPtr<Component>& component)
		{
			if (component->sameTypeID(_type))
			{
				component->destroy();
				return true;
			}
			return false;
		});
	}

	void Actor::detachComponentByName(const String& _name)
	{
		m_components.remove_if([&_name](const ObjectPtr<Component>& component)
		{
			if (component->sameName(_name))
			{
				component->destroy();
				return true;
			}
			return false;
		});
	}

	void Actor::detachComponentsByTag(const String& _tag)
	{
		m_components.remove_if([&_tag](const ObjectPtr<Component>& component)
		{
			if (component->hasTag(_tag))
			{
				component->destroy();
				return true;
			}
			return false;
		});
	}

	bool Actor::hasComponentByTypeID(const TypeID& _type) const
	{
		return m_components.any([&_type](const ObjectPtr<Component>& component)
		{
			return component->sameTypeID(_type);
		});
	}

	bool Actor::hasComponentByName(const String& _name) const
	{
		return m_components.any([&_name](const ObjectPtr<Component>& component)
		{
			return component->sameName(_name);
		});
	}

	bool Actor::hasComponentByTag(const String& _tag) const
	{
		return m_components.any([&_tag](const ObjectPtr<Component>& component)
		{
			return component->hasTag(_tag);
		});
	}

	ObjectPtr<Component> Actor::attachComponentByObjectClass(const ObjectClass& _objectClass, const String& _name)
	{
		String name = (_name == U"") ? U"{}.{}"_fmt(getName().toString(), _objectClass.getClassName()) : _name;
		return Cast<Component>(getLevel()->createObject(_objectClass, this, name));
	}
}
