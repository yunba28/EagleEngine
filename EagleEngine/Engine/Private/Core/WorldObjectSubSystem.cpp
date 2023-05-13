#include <Core/WorldObjectSubSystem.hpp>

#include <Core/WorldObject.hpp>
#include <Core/ExecutionOrder.hpp>
#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	Timer WorldObjectSubSystem::sHitStopTimer{ 0.s };

	WorldObjectSubSystem::WorldObjectListener::WorldObjectListener(const ObjectClass& inObjectClass)
		: mObjects()
		, mQueue()
		, mInherited(inObjectClass.inherited())
		, mTypeIndex(inObjectClass.type())
		, mHasPendingKill(false)
	{
	}

	WorldObjectSubSystem::WorldObjectListener::WorldObjectListener()
		: mObjects()
		, mQueue()
		, mInherited(ObjectInherited::None)
		, mTypeIndex(typeid(void))
		, mHasPendingKill(false)
	{
		ensure(false, "not called default constructor");
	}

	WorldObjectSubSystem::WorldObjectListener::~WorldObjectListener()
	{
		mObjects.release();
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
			mObjects.insert(
				mObjects.end(),
				std::move_iterator{ mQueue.begin()},
				std::move_iterator{ mQueue.end() }
			);
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

	ObjectRef<WorldObject> WorldObjectSubSystem::WorldObjectListener::getByOwner(const Actor* const inOwner) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [owner = inOwner](const ObjectPtr<WorldObject>& inObject)
		{
			return inObject->isOwner(owner);
		});
		return (found == mObjects.end()) ? nullptr : ObjectRef<WorldObject>(*found);
	}

	Array<ObjectRef<WorldObject>> WorldObjectSubSystem::WorldObjectListener::getsByOwner(const Actor* const inOwner) const
	{
		return mObjects
			.map([](const ObjectPtr<WorldObject>& inObject)
			{
					return ObjectRef<WorldObject>(inObject);
			})
			.filter([owner = inOwner](const ObjectRef<WorldObject>& inObject)
			{
				return inObject->isOwner(owner);
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
		// Levelの所属とOwnerへのアタッチ
		{
			worldObject->_internalAttachToLevel(getLevel());
			if (newOwner != nullptr)
			{
				worldObject->_internalAttachToOwner(newOwner);
			}
		}

		TypeIndex newTypeIndex = inObjectClass.type();

		// 初めて生成されるObjectならListenerを生成
		if (!mWorldObjectTable.contains(newTypeIndex))
		{
			mWorldObjectTable.emplace(newTypeIndex, WorldObjectListener{ inObjectClass });
			mOrderQueue.push_back(newTypeIndex);
		}

		// ListenerにObjectを登録
		mWorldObjectTable[newTypeIndex].addWorldObject(worldObject);

		worldObject->_internalConstruct();

		return ObjectRef<WorldObject>(worldObject);
	}

	ObjectRef<WorldObject> WorldObjectSubSystem::findByName(const TypeIndex& inFindType, const String& inName)const
	{
		for (const auto& type : mExecutionOrder)
		{
			if (type != inFindType)
				continue;

			if (auto found = mWorldObjectTable.at(type).getByName(inName); found)
			{
				return found;
			}

			break;
		}

		return ObjectRef<WorldObject>{ nullptr };
	}

	ObjectRef<WorldObject> WorldObjectSubSystem::findByTag(const TypeIndex& inFindType, const String& inTag)const
	{
		for (const auto& type : mExecutionOrder)
		{
			if (type != inFindType)
				continue;

			if (auto found = mWorldObjectTable.at(type).getByTag(inTag); found)
			{
				return found;
			}

			break;
		}

		return ObjectRef<WorldObject>{ nullptr };
	}

	Array<ObjectRef<WorldObject>> WorldObjectSubSystem::findsByTag(const String& inTag)const
	{
		Array<ObjectRef<WorldObject>> result;

		for (const auto& type : mExecutionOrder)
		{
			if (auto founds = mWorldObjectTable.at(type).getsByTag(inTag); !founds.isEmpty())
			{
				result.append(founds);
			}
		}

		return result;
	}

	Array<ObjectRef<WorldObject>> WorldObjectSubSystem::findsByTag(const TypeIndex& inFindType, const String& inTag)const
	{
		for (const auto& type : mExecutionOrder)
		{
			if (type != inFindType)
				continue;

			if (auto founds = mWorldObjectTable.at(type).getsByTag(inTag); !founds.isEmpty())
			{
				return founds;
			}

			break;
		}

		return Array<ObjectRef<WorldObject>>{};
	}

	ObjectRef<WorldObject> WorldObjectSubSystem::findByOwner(const TypeIndex& inFindType, const Actor* const inOwner) const
	{
		for (const auto& type : mExecutionOrder)
		{
			if (type != inFindType)
				continue;

			if (auto found = mWorldObjectTable.at(type).getByOwner(inOwner); found)
			{
				return found;
			}

			break;
		}

		return ObjectRef<WorldObject>();
	}

	Array<ObjectRef<WorldObject>> WorldObjectSubSystem::findsByOwner(const Actor* const inOwner) const
	{
		Array<ObjectRef<WorldObject>> result;

		for (const auto& type : mExecutionOrder)
		{
			if (auto founds = mWorldObjectTable.at(type).getsByOwner(inOwner); !founds.isEmpty())
			{
				result.append(founds);
			}
		}

		return result;
	}

	void WorldObjectSubSystem::HitStop(double inTimeDilation, double inStopTimeSec)
	{
		sHitStopTimer.restart(Duration{ inStopTimeSec });
		WorldObject::SetGlobalTimeDilation(inTimeDilation);
	}
}
