#pragma once

#include <Core/Internal/ObjectListener.hpp>
#include <Core/ObjectClass.hpp>
#include <Core/Object.hpp>

#include <Container/HashTable.hpp>

namespace EagleEngine
{
	class Scene : public Object
	{
	public:

		Scene();

		~Scene();

	private:

		virtual bool awake()override { return true; }

		virtual void start()override {}

		virtual void update([[maybe_unused]] double _deltaTime)override {}

		virtual void draw()const {}

		virtual bool dispose()override { return true; }

	public:

		virtual void _internalUpdate(double _deltaTime)override;

	public:

		ObjectPtr<Object> createObject(const ObjectClass& _objectClass, Actor* _owner = nullptr, const String& _name = U"");

		template<Concept::IsActor ActorType = Actor>
		ObjectPtr<ActorType> createActor(const String& _name);

		template<Concept::IsComponent ComponentType>
		ObjectPtr<ComponentType> createComponent(Actor* _owner);

		template<Concept::IsScene SceneType>
		ObjectPtr<SceneType> addSubScene(const String& _name);

	private:

		void preUpdate();

	private:

		HashTable<TypeID, Internal::ObjectListener> mObjectTable;

		Array<TypeID> mExecutionOrder;

		Array<TypeID> mOrderQueue;

	};

	template<Concept::IsActor ActorType>
	inline ObjectPtr<ActorType> Scene::createActor(const String& _name)
	{
		return Cast<ActorType>(createObject(CreateObjectClass<ActorType>(), nullptr, _name));
	}

	template<Concept::IsComponent ComponentType>
	inline ObjectPtr<ComponentType> Scene::createComponent(Actor* _owner)
	{
		return Cast<ComponentType>(createObject(CreateObjectClass<ComponentType>(), _owner));
	}

	template<Concept::IsScene SceneType>
	inline ObjectPtr<SceneType> Scene::addSubScene(const String& _name)
	{
		return Cast<SceneType>(createObject(CreateObjectClass<SceneType>(), nullptr, _name));
	}
}
