#include <GameFramework/Actor.hpp>

#include <GameFramework/Component.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	void Actor::attachToActor(Actor* newOwner)
	{
		detachToOwner();
		_internalAttachToOwner(newOwner);
	}

	void Actor::attachToComponent(Component* newOwner)
	{
		detachToOwner();
		_internalAttachToOwner(newOwner);
	}

	void Actor::detachToOwner()
	{
		_internalDetachToOwner();
	}

	ObjectRef<WorldObject> Actor::findComponentByOwner(const TypeIndex& inFindType) const
	{
		if (auto level = getLevel(); level)
		{
			return level->findByOwner(inFindType, this);
		}
		return ObjectRef<WorldObject>();
	}

	ObjectRef<WorldObject> Actor::findComponentByName(const TypeIndex& inFindType, const String& inName) const
	{
		if (auto level = getLevel(); level)
		{
			return level->findByName(inFindType, inName);
		}
		return ObjectRef<WorldObject>();
	}

	ObjectRef<WorldObject> Actor::findComponentByTag(const TypeIndex& inFindType, const String& inTag) const
	{
		if (auto level = getLevel(); level)
		{
			return level->findByTag(inFindType, inTag);
		}
		return ObjectRef<WorldObject>();
	}

	ObjectRef<WorldObject> Actor::createComponent(const ObjectClass& inObjectClass, const String& newName)
	{
		if (auto level = getLevel(); level)
		{
			return level->createComponent(inObjectClass, newName, this);
		}
		return ObjectRef<WorldObject>();
	}
}
