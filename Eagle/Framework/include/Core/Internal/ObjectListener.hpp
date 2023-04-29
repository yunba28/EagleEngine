#pragma once

#include <Core/ObjectPtr.hpp>
#include <Container/Array.hpp>

namespace EagleEngine::Internal
{
	class ObjectListener final
	{
	public:

		ObjectListener();

		~ObjectListener();

	public:

		void update(double _deltaTime);

		void addObject(Object* _object);

		ObjectPtr<Object> findObjectByName(const String& _name)const;

		Array<ObjectPtr<Object>> findObjectsByTag(const String& _tag)const;

		template<class ObjectType>
		Array<ObjectPtr<ObjectType>> findObjectsByTag(const String& _tag)const;

		void clearObjects();

	private:

		void preUpdate();

		Array<ObjectPtr<Object>> filterObjectsByTag(const String& _tag)const;

	private:

		Array<ObjectPtr<Object>> mObjects;

		Array<ObjectPtr<Object>> mQueue;

		bool mPendingRemove;
	};

	template<class ObjectType>
	Array<ObjectPtr<ObjectType>> ObjectListener::findObjectsByTag(const String& _tag)const
	{
		if (auto foundAry = filterObjectsByTag(_tag); !foundAry.empty())
		{
			return foundAry.map([](const ObjectPtr<Object>& _object) {return Cast<ObjectType>(_object); });
		}

		return Array<ObjectPtr<ObjectType>>{};
	}
}
