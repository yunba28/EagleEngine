#include <GameFramework/Actor.hpp>

#include <GameFramework/Component.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{

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

	void Actor::LookAtForActor(Actor* inTargetActor)
	{
		auto dir = (inTargetActor->getWorldPosition() - getWorldPosition()).normalized();
		setWorldRotation(Quaternion::FromUnitVectors(Vec3::Forward(), dir));
	}

	void Actor::LookAtForComponent(Component* inTargetComponent)
	{
		auto dir = (inTargetComponent->getWorldPosition() - getWorldPosition()).normalized();
		setWorldRotation(Quaternion::FromUnitVectors(Vec3::Forward(), dir));
	}

	ObjectRef<WorldObject> Actor::_internalCreateComponent(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (auto level = getLevel(); level)
		{
			return level->createComponent(inObjectClass, newName, newOwner);
		}
		return ObjectRef<WorldObject>();
	}
}
