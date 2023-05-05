#include <Core/WorldObjectSubSystem.hpp>

#include <Core/WorldObject.hpp>
#include <Core/ExecutionOrder.hpp>
#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	WorldObjectSubSystem::WorldObjectListener::~WorldObjectListener()
	{
		for (const auto& object : mObjects)
		{
			if (IsValidByObject(object.get()))
			{
				delete object.get();
			}
		}
		mObjects.clear();
	}

	void WorldObjectSubSystem::WorldObjectListener::update(double inDeltaTime)
	{
		if (mHasPendingKill)
		{
			mObjects.remove_if([](const ObjectPtr<WorldObject>& inTarget)
			{
				if (inTarget->pendingKill())
				{
					delete inTarget.get();
					return true;
				}
				return false;
			});
			mHasPendingKill = false;
		}

		if (!mQueue.empty())
		{
			mObjects.append(mQueue);
			mQueue.clear();
		}

		for (const auto& object : mObjects)
		{
			object->_internalUpdate(inDeltaTime);
			mHasPendingKill |= object->pendingKill();
		}
	}

	void WorldObjectSubSystem::WorldObjectListener::addWorldObject(WorldObject* inWorldObject)
	{
		mQueue.push_back(ObjectPtr<WorldObject>{ inWorldObject });
	}

	ObjectPtr<WorldObject> WorldObjectSubSystem::WorldObjectListener::getByName(const String& inName) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [&inName](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->sameName(inName);
		});
		return (found == mObjects.end()) ? nullptr : *found;
	}

	ObjectPtr<WorldObject> WorldObjectSubSystem::WorldObjectListener::getByTag(const String& inTag) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [&inTag](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->hasTag(inTag);
		});
		return (found == mObjects.end()) ? nullptr : *found;
	}

	Array<ObjectPtr<WorldObject>> WorldObjectSubSystem::WorldObjectListener::getsByTag(const String& inTag) const
	{
		return mObjects.filter([&inTag](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->hasTag(inTag);
		});
	}

	void WorldObjectSubSystem::update(double inDeltaTime)
	{
		if (!mOrderQueue.empty())
		{
			mExecutionOrder.append(mOrderQueue);
			mOrderQueue.clear();
			mExecutionOrder.sort_by([](const TypeIndex& inLeft, const TypeIndex& inRight)
			{
				return ExecutionOrder::Get(inLeft) < ExecutionOrder::Get(inRight);
			});
		}

		for (const auto& type : mExecutionOrder)
		{
			mWorldObjectTable[type].update(inDeltaTime);
		}
	}

	ObjectPtr<WorldObject> WorldObjectSubSystem::createObject(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (!inObjectClass.hasInherited(ObjectInherited::WorldObject))
			return nullptr;

		WorldObject* worldObject = static_cast<WorldObject*>(inObjectClass(newName));
		worldObject->_internalAttachToLevel(getLevel());
		if (newOwner != nullptr)
		{
			worldObject->_internalAttachToOwner(newOwner);
		}

		TypeIndex newTypeIndex = inObjectClass.type();

		if (!mWorldObjectTable.contains(newTypeIndex))
		{
			mWorldObjectTable.emplace(newTypeIndex, WorldObjectListener{});
			mOrderQueue.push_back(newTypeIndex);
		}

		mWorldObjectTable[newTypeIndex].addWorldObject(worldObject);

		return worldObject;
	}
}
