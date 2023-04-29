#include <Core/Scene.hpp>
#include <Core/ExecutionOrder.hpp>

namespace EagleEngine
{
	Scene::Scene()
		: mObjectTable()
		, mExecutionOrder()
		, mOrderQueue()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::_internalUpdate(double _deltaTime)
	{
		preUpdate();

		for (const auto& type : mExecutionOrder)
		{
			mObjectTable[type].update(_deltaTime);
		}

		Object::_internalUpdate(_deltaTime);
	}

	ObjectPtr<Object> Scene::createObject(const ObjectClass& _objectClass, Actor* _owner, const String& _name)
	{
		Object* object = _objectClass();
		{
			object->setName(_name);
			object->_internalAttachToScene(this);
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

	void Scene::preUpdate()
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
