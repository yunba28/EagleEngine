#include <Core/WorldObjectSubSystem.hpp>

#include <Core/WorldObject.hpp>

namespace eagle
{
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

	void WorldObjectSubSystem::update(double inDeltaTime)
	{
		if (!mOrderQueue.empty())
		{
			mExecutionOrder.append(mOrderQueue);
			mOrderQueue.clear();
			// sorting

		}

		for (const auto& type : mExecutionOrder)
		{
			mWorldObjectTable[type].update(inDeltaTime);
		}
	}

	ObjectPtr<Object> WorldObjectSubSystem::createObject(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (inObjectClass.hasInherited(ObjectInherited::None))
			return nullptr;

		Object* object = inObjectClass(newName);

		if (inObjectClass.hasInherited(ObjectInherited::WorldObject) && newOwner != nullptr)
		{
			static_cast<WorldObject*>(object)->_internalAttachToOwner(newOwner);
		}

		return object;
	}
}
