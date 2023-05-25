#include <Core/WorldObject.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	double WorldObject::sGlobalTimeDilation = 1.0;

	WorldObject::WorldObject()
		: mTransform(nullptr)
		, mLevel(nullptr)
		, mSelf(this)
		, mOwner(nullptr)
		, mTags()
		, mCustomTimeDilation(1.0)
		, mActive(true)
		, mStarted(false)
		, mPendingKill(false)
		, mUpdateEnabled(true)
		, bCanCreateTransform(true)
	{
	}

	void WorldObject::_internalAttachToLevel(Level* newLevel)
	{
		ensure(mLevel.invalid(), "The object is already registered in Level");
		mLevel = newLevel;
	}

	void WorldObject::_internalAttachToOwner(WorldObject* newOwner)
	{
		mOwner = newOwner;
	}

	void WorldObject::_internalDetachToOwner()
	{
		mOwner = nullptr;
	}

	void WorldObject::attachToActor(Actor* newOwner)
	{
		detachToOwner();
		_internalAttachToOwner(newOwner);
	}

	void WorldObject::attachToComponent(Component* newOwner)
	{
		detachToOwner();
		_internalAttachToOwner(newOwner);
	}

	ObjectRef<WorldObject> WorldObject::getRoot() const
	{
		auto owner = getOwner();
		if (!owner)
		{
			return mSelf;
		}
		auto tmp = owner->getOwner();
		while (tmp)
		{
			owner = tmp;
			tmp = owner->getOwner();
		}
		return owner;
	}

	void WorldObject::detachToOwner()
	{
		_internalDetachToOwner();
	}

	void WorldObject::addTag(const String& newTag)
	{
		if (!hasTag(newTag))
		{
			mTags.push_back(HashString(newTag));
		}
	}

	void WorldObject::addTags(const Array<String>& newTags)
	{
		for (const auto& tag : newTags)
		{
			addTag(tag);
		}
	}

	void WorldObject::removeTag(const String& inTag)
	{
		mTags.remove(inTag);
	}

	Array<String> WorldObject::getTags() const noexcept
	{
		return mTags.map([](const HashString& str) {return str.toString(); });
	}

	bool WorldObject::hasTag(const String& inTag) const
	{
		return mTags.contains(inTag);
	}

	void WorldObject::destroy()
	{
		if (!mPendingKill)
		{
			mActive = false;
			mPendingKill = true;
			_internalDestruct();
		}
	}
}
