#include <Core/Actor.hpp>
#include <Core/Component.hpp>
#include <Core/Internal/SceneObject.hpp>

namespace EagleEngine
{
	Actor::Actor()
		: mComponents()
		, mTransform(MakeUnique<Transform>())
	{
	}

	void Actor::_internalDestruct()
	{
		Object::_internalDestruct();
		for (const auto& component : mComponents)
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
		auto found = std::find_if(mComponents.begin(), mComponents.end(), [&_type](const ObjectPtr<Component>& component)
		{
			return component->sameTypeID(_type);
		});

		if (found != mComponents.end())
		{
			return (*found);
		}

		return ObjectPtr<Component>(nullptr);
	}

	ObjectPtr<Component> Actor::getComponentByName(const String& _name) const
	{
		auto found = std::find_if(mComponents.begin(), mComponents.end(), [&_name](const ObjectPtr<Component>& component)
		{
			return component->sameName(_name);
		});

		if (found != mComponents.end())
		{
			return (*found);
		}

		return ObjectPtr<Component>{};
	}

	Array<ObjectPtr<Component>> Actor::getComponentsByTag(const String& _tag) const
	{
		return mComponents.filter([&_tag](const ObjectPtr<Component>& component)
		{
			return component->hasTag(_tag);
		});
	}

	void Actor::detachComponentByTypeID(const TypeID& _type)
	{
		mComponents.remove_if([&_type](const ObjectPtr<Component>& component)
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
		mComponents.remove_if([&_name](const ObjectPtr<Component>& component)
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
		mComponents.remove_if([&_tag](const ObjectPtr<Component>& component)
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
		return mComponents.any([&_type](const ObjectPtr<Component>& component)
		{
			return component->sameTypeID(_type);
		});
	}

	bool Actor::hasComponentByName(const String& _name) const
	{
		return mComponents.any([&_name](const ObjectPtr<Component>& component)
		{
			return component->sameName(_name);
		});
	}

	bool Actor::hasComponentByTag(const String& _tag) const
	{
		return mComponents.any([&_tag](const ObjectPtr<Component>& component)
		{
			return component->hasTag(_tag);
		});
	}

	ObjectPtr<Component> Actor::attachComponentByObjectClass(const ObjectClass& _objectClass, const String& _name)
	{
		String name = (_name == U"") ? U"{}.{}"_fmt(getName().toString(), _objectClass.getClassName()) : _name;
		return Cast<Component>(getSceneObject()->createObject(_objectClass, this, name));
	}
}
