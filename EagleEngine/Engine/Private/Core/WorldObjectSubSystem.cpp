#include <Core/WorldObjectSubSystem.hpp>

#include <Core/WorldObject.hpp>
#include <Core/ExecutionOrder.hpp>
#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	Timer WorldObjectSubSystem::sHitStopTimer{ 0.s };

	WorldObjectSubSystem::WorldObjectListener::~WorldObjectListener()
	{
		mObjects.clear();
	}

	void WorldObjectSubSystem::WorldObjectListener::update(double inDeltaTime)
	{
		if (mHasPendingKill)
		{
			mObjects.remove_if([](const ObjectPtr<WorldObject>& inTarget)
			{
				return inTarget->pendingKill();
			});
			mHasPendingKill = false;
		}

		if (!mQueue.empty())
		{
			mObjects.reserve(mObjects.size() + mQueue.size());
			for (auto& object : mQueue)
			{
				mObjects.push_back(std::move(object));
			}
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
		mQueue.push_back(ObjectPtr<WorldObject>(inWorldObject));
	}

	ObjectRef<WorldObject> WorldObjectSubSystem::WorldObjectListener::getByName(const String& inName) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [&inName](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->sameName(inName);
		});
		return (found == mObjects.end()) ? nullptr : ObjectRef<WorldObject>(*found);
	}

	ObjectRef<WorldObject> WorldObjectSubSystem::WorldObjectListener::getByTag(const String& inTag) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [&inTag](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->hasTag(inTag);
		});
		return (found == mObjects.end()) ? nullptr : ObjectRef<WorldObject>(*found);
	}

	Array<ObjectRef<WorldObject>> WorldObjectSubSystem::WorldObjectListener::getsByTag(const String& inTag) const
	{
		return mObjects
		.map([](const ObjectPtr<WorldObject>& inObject)
		{
			return ObjectRef<WorldObject>(inObject);
		})
		.filter([&inTag](const ObjectRef<WorldObject>& inObject)
		{
			return inObject->hasTag(inTag);
		});
	}

	WorldObjectSubSystem::~WorldObjectSubSystem()
	{
		WorldObject::SetGlobalTimeDilation(1);
		sHitStopTimer.set(0.s);
	}

	void WorldObjectSubSystem::update(double inDeltaTime)
	{
		if (sHitStopTimer.duration() > 0.s && sHitStopTimer.reachedZero())
		{
			sHitStopTimer.set(0.s);
			WorldObject::SetGlobalTimeDilation(1);
		}

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

	ObjectRef<WorldObject> WorldObjectSubSystem::createObject(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
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

		return ObjectRef<WorldObject>(worldObject);
	}

	void WorldObjectSubSystem::HitStop(double inTimeDilation, double inStopTimeSec)
	{
		sHitStopTimer.restart(Duration{ inStopTimeSec });
		WorldObject::SetGlobalTimeDilation(inTimeDilation);
	}
}
