#pragma once

#include <CoreMinimal.hpp>

namespace EagleEngine
{

	class Object
	{
	public:

		explicit Object();

		virtual ~Object() = 0;

		Object(const Object&) = delete;

		Object& operator=(const Object&) = delete;

		Object(Object&&) = default;

		Object& operator=(Object&&) = default;

	protected:

		/* category 'event' */

		virtual bool awake() = 0;

		virtual void start() = 0;

		virtual void update([[maybe_unused]] double _deltaTime) = 0;

		virtual bool dispose() = 0;

	public:

		/* category 'internal event' */

		virtual void _internalConstruct();

		virtual void _internalDestruct();

		virtual void _internalUpdate(double _deltaTime);

		virtual void _internalAttachToScene(SceneObject* _scene);

		virtual void _internalAttachToScene(Scene* _scene);

		virtual void _internalAttachToActor(Actor* _actor);

	public:

		/* category 'common' */

		ObjectPtr<SceneObject> getSceneObject()const noexcept;

		ObjectPtr<Scene> getScene()const noexcept;

		ObjectPtr<Actor> getActorOwner()const noexcept;

		void setName(const String& _name);

		NameView getName()const noexcept;

		bool sameName(const String& _name)const noexcept;

		void addTag(const String& _tag);

		void addTags(InitializerList<String> _tagList);

		void addTags(const Array<String>& _tagList);

		HashedString findTag(const String& _tag)const;

		const Array<HashedString>& getTags()const noexcept;

		void removeTag(const String& _tag);

		bool hasTag(const String& _tag)const;

		const TypeID& getTypeID()const noexcept;

		bool sameTypeID(const TypeID& _typeID)const noexcept;

		void setUpdateEnable(bool _enabled)noexcept;

		bool isUpdateEnabled()const noexcept;

		void setActive(bool _actived)noexcept;

		bool isActive()const noexcept;

		bool isPendingKill()const noexcept;

		void destroy();

		bool invalid()const;

	public:

		bool operator==(const Object& _other)const noexcept;

		template<class ObjectType>
		bool operator==(const ObjectPtr<ObjectType>& _other)const noexcept;

		bool operator!=(const Object& _other)const noexcept;

		template<class ObjectType>
		bool operator!=(const ObjectPtr<ObjectType>& _other)const noexcept;

		explicit operator bool()const noexcept;

	private:

		ObjectPtr<Scene> mScene;

		ObjectPtr<SceneObject> mSceneObject;

		ObjectPtr<Actor> mActorOwner;

		Name mName;

		Array<HashedString> mTags;

		TypeID mTypeID;

		bool mUpdateEnabled;

		bool mActive;

		bool mPendingKill;

		friend class ObjectClass;

	};

	template<class ObjectType>
	bool Object::operator==(const ObjectPtr<ObjectType>& _other)const noexcept
	{
		return this == _other.get();
	}

	template<class ObjectType>
	bool Object::operator!=(const ObjectPtr<ObjectType>& _other)const noexcept
	{
		return this != _other.get();
	}
}
