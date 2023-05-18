#include <Core/WorldObject.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	double WorldObject::sGlobalTimeDilation = 1.0;

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
