#include <Core/LevelBase.hpp>
#include <Core/World.hpp>
#include <Core/ExecutionOrder.hpp>

namespace EagleEngine
{
	LevelBase::LevelBase()
	{
	}

	LevelBase::~LevelBase()
	{
	}

	void LevelBase::_internalUpdate(double _deltaTime)
	{
	}

	void LevelBase::_internalAttachToWorld(World* _world)
	{
		mWorld = _world;
	}

	ObjectPtr<Object> LevelBase::createObject(const ObjectClass& _objectClass, Actor* _owner, const String& _name)
	{
		Object* object = _objectClass();
		{
			object->setName(_name);
			object->_internalAttachToLevel(this);
			object->_internalAttachToActor(_owner);
			object->_internalConstruct();
		}

		const TypeID& type = _objectClass.getTypeID();

		if (!mExecutionOrder.contains(type))
		{
			mOrderQueue.push_back(type);
		}

		mObjectTable[type].addObject(object);

		return ObjectPtr<Object>(object);
	}

	void LevelBase::preUpdate()
	{
		if (!mOrderQueue.empty())
		{
			mExecutionOrder.append(mOrderQueue);
			mOrderQueue.clear();
			mExecutionOrder.sort_by([](const TypeID& a, const TypeID& b)
			{
				return ExecutionOrder::GetOrder(a) < ExecutionOrder::GetOrder(b);
			});
		}
	}
}
