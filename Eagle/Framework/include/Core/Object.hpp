#pragma once

#include <CoreMinimal.hpp>

namespace EagleEngine
{
	enum class UpdateEnable : uint8
	{
		Update  = 0b001,
		UpdateFadeIn = 0b010,
		UpdateFadeOut = 0b100,
		EnableAll = 0b111,
		DisableAll = 0b000,
		UpdateOnly = Update,
		UpdateFadeOnly = 0b110
	};

	class Object
	{
	public:

		explicit Object();

		virtual ~Object() = 0;

	protected:

		/* category 'event' */

		virtual bool awake() = 0;

		virtual void start() = 0;

		virtual void update([[maybe_unused]] double _deltaTime) = 0;

		virtual bool dispose() = 0;

		/* category 'advanced' */

		virtual void updateFadeIn([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) = 0;

		virtual void updateFadeOut([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) = 0;

	public:

		/* category 'internal event' */

		virtual void _internalConstruct();

		virtual void _internalDestruct();

		virtual void _internalUpdate(double _deltaTime);

		virtual void _internalUpdateFadeIn(double _deltaTime, double _progress);

		virtual void _internalUpdateFadeOut(double _deltaTime, double _progress);

		virtual void _internalAttachToScene(SceneObject* _scene);

		virtual void _internalAttachToActor(Actor* _actor);

	public:

		/* category 'common' */

		ObjectPtr<SceneObject> getSceneObject()const noexcept;

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

		void setUpdateEnable(UpdateEnable _enabled)noexcept;

		bool isUpdateEnabled(UpdateEnable _enabled)const noexcept;

		void setActive(bool _actived)noexcept;

		bool isActive()const noexcept;

		bool isPendingKill()const noexcept;

		void destroy();

	private:

		ObjectPtr<SceneObject> mSceneObject;

		ObjectPtr<Actor> mActorOwner;

		Name mName;

		Array<HashedString> mTags;

		TypeID mTypeID;

		UpdateEnable mUpdateEnabled;

		bool mActive;

		bool mPendingKill;

		friend class ObjectClass;

	};

}
