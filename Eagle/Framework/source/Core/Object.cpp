#include <Core/Object.hpp>
#include <Core/ObjectPtr.hpp>
#include <Core/ObjectClass.hpp>
#include <Core/Actor.hpp>
#include <Core/Internal/SceneObject.hpp>

namespace EagleEngine
{
	Object::Object()
		: mSceneObject(nullptr)
		, mActorOwner(nullptr)
		, mName()
		, mTags()
		, mTypeID(typeid(void))
		, mActive(true)
		, mPendingKill(false)
		, mUpdateEnabled(UpdateEnable::EnableAll)
	{
		bool result = RegisterObjectHandle(this);
		assert(result, "failed register by object");
	}

	Object::~Object()
	{
		bool result = UnregisterObjectHandle(this);
		assert(result, "failed unregister by object");
	}

	void Object::_internalConstruct()
	{
		awake()
			? start()
			: assert(false, "failed on awake");
	}

	void Object::_internalDestruct()
	{
		if (!dispose())
		{
			assert(false, "failed on dispose");
		}
	}

	void Object::_internalUpdate(double _deltaTime)
	{
		if (mActive && isUpdateEnabled(UpdateEnable::Update))
		{
			update(_deltaTime);
		}
	}

	void Object::_internalUpdateFadeIn(double _deltaTime, double _progress)
	{
		if (mActive && isUpdateEnabled(UpdateEnable::UpdateFadeIn))
		{
			updateFadeIn(_deltaTime, _progress);
		}
	}

	void Object::_internalUpdateFadeOut(double _deltaTime, double _progress)
	{
		if (mActive && isUpdateEnabled(UpdateEnable::UpdateFadeOut))
		{
			updateFadeOut(_deltaTime, _progress);
		}
	}

	void Object::_internalAttachToScene(SceneObject* _scene)
	{
		mSceneObject = _scene;
	}

	void Object::_internalAttachToActor(Actor* _actor)
	{
		mActorOwner = _actor;
	}

	ObjectPtr<SceneObject> Object::getSceneObject() const noexcept
	{
		return mSceneObject;
	}

	ObjectPtr<Actor> Object::getActorOwner() const noexcept
	{
		return mActorOwner;
	}

	void Object::setName(const String& _name)
	{
		mName = Name::Make(_name);
	}

	NameView Object::getName() const noexcept
	{
		return NameView{ mName };
	}

	bool Object::sameName(const String& _name) const noexcept
	{
		return mName == _name;
	}

	void Object::addTag(const String& _tag)
	{
		if (!mTags.includes(_tag))
		{
			mTags.push_back(_tag);
		}
	}

	void Object::addTags(InitializerList<String> _tagList)
	{
		for (const auto& tag : _tagList)
		{
			if (!mTags.includes(tag))
			{
				mTags.push_back(tag);
			}
		}
	}

	void Object::addTags(const Array<String>& _tagList)
	{
		for (const auto& tag : _tagList)
		{
			if (!mTags.includes(tag))
			{
				mTags.push_back(tag);
			}
		}
	}

	HashedString Object::findTag(const String& _tag) const
	{
		if (mTags.includes(_tag))
		{
			return HashedString{ _tag };
		}

		return HashedString{};
	}

	const Array<HashedString>& Object::getTags() const noexcept
	{
		return mTags;
	}

	void Object::removeTag(const String& _tag)
	{
		mTags.remove(_tag);
	}

	bool Object::hasTag(const String& _tag) const
	{
		return mTags.includes(_tag);
	}

	const TypeID& Object::getTypeID() const noexcept
	{
		return mTypeID;
	}

	bool Object::sameTypeID(const TypeID& _typeID) const noexcept
	{
		return mTypeID == _typeID;
	}

	void Object::setUpdateEnable(UpdateEnable _enabled) noexcept
	{
		mUpdateEnabled = _enabled;
	}

	bool Object::isUpdateEnabled(UpdateEnable _enabled) const noexcept
	{
		const uint8 enabled = static_cast<uint8>(_enabled);
		return static_cast<uint8>(mUpdateEnabled) & enabled;
	}

	void Object::setActive(bool _actived) noexcept
	{
		// destroyが呼ばれた後は機能しない
		if (!mPendingKill)
		{
			mActive = _actived;
		}
	}

	bool Object::isActive() const noexcept
	{
		return mActive;
	}

	bool Object::isPendingKill() const noexcept
	{
		return mPendingKill;
	}

	void Object::destroy()
	{
		if (!mPendingKill)
		{
			mPendingKill = true;
			mActive = false;
			_internalDestruct();
		}
	}
}
