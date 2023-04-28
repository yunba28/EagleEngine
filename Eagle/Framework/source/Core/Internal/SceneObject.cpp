#include <Core/Internal/SceneObject.hpp>
#include <Core/Object.hpp>
#include <Core/ExecutionOrder.hpp>

namespace EagleEngine
{
	SceneObject::SceneObject()
	{
	}

	SceneObject::~SceneObject()
	{
	}

	bool SceneObject::awake()
	{
		return true;
	}

	void SceneObject::start()
	{
	}

	void SceneObject::update(double _deltaTime)
	{
		preUpdate();

		for (const auto& type : mExecutionOrder)
		{
			mObjectTable[type].update(_deltaTime);
		}
	}

	bool SceneObject::dispose()
	{
		return true;
	}

	void SceneObject::updateFadeIn(double _deltaTime, double _progress)
	{
		preUpdate();

		for (const auto& type : mExecutionOrder)
		{
			mObjectTable[type].updateFadeIn(_deltaTime, _progress);
		}
	}

	void SceneObject::updateFadeOut(double _deltaTime, double _progress)
	{
		preUpdate();

		for (const auto& type : mExecutionOrder)
		{
			mObjectTable[type].updateFadeOut(_deltaTime, _progress);
		}
	}

	ObjectPtr<Object> SceneObject::createObject(const ObjectClass& _objectClass, Actor* _owner, const String& _name)
	{
		Object* object = _objectClass();
		{
			object->_internalAttachToScene(this);
			object->_internalAttachToActor(_owner);
			if (_name != U"")
			{
				object->setName(_name);
			}
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

	void SceneObject::preUpdate()
	{
		if (!mOrderQueue.empty())
		{
			mExecutionOrder.append(mOrderQueue);
			mOrderQueue.clear();
			mExecutionOrder.stable_sort_by([](const TypeID& a, const TypeID& b)
			{
				return ExecutionOrder::GetOrder(a) < ExecutionOrder::GetOrder(b);
			});
		}
	}
}
