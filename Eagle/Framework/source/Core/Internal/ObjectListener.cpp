#include <Core/Internal/ObjectListener.hpp>
#include <Core/Object.hpp>

namespace EagleEngine::Internal
{
	ObjectListener::ObjectListener()
		: mObjects()
		, mQueue()
		, mPendingRemove(false)
	{
	}

	ObjectListener::~ObjectListener()
	{
	}

	void ObjectListener::update(double _deltaTime)
	{
		preUpdate();

		for (const auto& pObj : mObjects)
		{
			pObj->_internalUpdate(_deltaTime);
			mPendingRemove |= pObj->isPendingKill();
		}
	}

	void ObjectListener::addObject(Object* _object)
	{
		mQueue.push_back(_object);
	}

	ObjectPtr<Object> ObjectListener::findObjectByName(const String& _name) const
	{
		auto found = std::find_if(mObjects.begin(), mObjects.end(), [&_name](const ObjectPtr<Object>& _object)
		{
			return _object->sameName(_name);
		});

		if (found == mObjects.end())
		{
			return ObjectPtr<Object>{};
		}

		return (*found);
	}

	Array<ObjectPtr<Object>> ObjectListener::findObjectsByTag(const String& _tag) const
	{
		return filterObjectsByTag(_tag);
	}

	void ObjectListener::clearObjects()
	{
		mObjects.each([](ObjectPtr<Object>& obj)
		{
			obj->destroy();
		});
		mObjects.clear();
	}

	void ObjectListener::preUpdate()
	{
		if (mPendingRemove)
		{
			mObjects.remove_if([](const ObjectPtr<Object>& object)
			{
				if (object->isPendingKill())
				{
					if (object)
					{
						delete object.get();
					}
					return true;
				}
				return false;
			});
			mPendingRemove = false;
		}

		if (!mQueue.empty())
		{
			mObjects.append(mQueue);
			mQueue.clear();
		}
	}

	Array<ObjectPtr<Object>> ObjectListener::filterObjectsByTag(const String& _tag) const
	{
		return mObjects.filter([&_tag](const ObjectPtr<Object>& _object)
		{
			return _object->hasTag(_tag);
		});
	}
}
