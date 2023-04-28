#pragma once

#include <Core/Object.hpp>
#include <Core/Internal/ObjectListener.hpp>
#include <Core/ObjectClass.hpp>

namespace EagleEngine
{
	class SceneObject final : public Object
	{
	public:

		SceneObject();

		~SceneObject();

	private:

		bool awake()override;

		void start()override;

		void update(double _deltaTime)override;

		bool dispose()override;

		void updateFadeIn(double _deltaTime, double _progress)override;

		void updateFadeOut(double _deltaTime, double _progress)override;

	public:

		ObjectPtr<Object> createObject(const ObjectClass& _objectClass, Actor* _owner = nullptr, const String& _name = U"");

		template<Concept::IsActor ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& _name);

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* _owner);

	private:

		void preUpdate();

	private:

		HashTable<TypeID, Internal::ObjectListener> mObjectTable;

		Array<TypeID> mExecutionOrder;

		Array<TypeID> mOrderQueue;

		template<class State, class SharedData>
		friend class SceneBase;

	};

	template<Concept::IsActor ActorType>
	inline ObjectPtr<ActorType> SceneObject::createActor(const String& _name)
	{
		return Cast<ActorType>(createObject(CreateObjectClass<ActorType>(), nullptr, _name));
	}

	template<Concept::IsComponent ComponentType>
	inline ObjectPtr<ComponentType> SceneObject::createComponent(Actor* _owner)
	{
		return Cast<ComponentType>(createObject(CreateObjectClass<ComponentType>(), _owner));
	}
}
