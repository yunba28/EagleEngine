#include <Core/Object.hpp>
#include <Core/ObjectPtr.hpp>
#include <Core/ObjectClass.hpp>
#include <Core/Actor.hpp>
#include <Core/LevelBase.hpp>

namespace EagleEngine
{
	Object::Object()
		: mLevel(nullptr)
		, mOwner(nullptr)
		, mName()
		, mTags()
		, mTypeID(typeid(void))
		, mUpdateEnabled(true)
		, mActive(true)
		, mPendingKill(false)
	{
		bool result = RegisterObjectHandle(this);
		assert(("failed register by object", result));
	}

	Object::~Object()
	{
		bool result = UnregisterObjectHandle(this);
		assert(("failed unregister by object", result));
	}

	void Object::_internalConstruct()
	{
		awake()
			? start()
			: assert(("failed on awake", false));
	}

	void Object::_internalDestruct()
	{
		if (!dispose())
		{
			assert(("failed on dispose", false));
		}
	}

	void Object::_internalUpdate(double _deltaTime)
	{
		if (mActive && mUpdateEnabled)
		{
			update(_deltaTime);
		}
	}

	void Object::_internalAttachToLevel(LevelBase* _level)
	{
		mLevel = _level;
	}

	void Object::_internalAttachToActor(Actor* _actor)
	{
		mOwner = _actor;
	}

	ObjectPtr<LevelBase> Object::getLevel() const noexcept
	{
		return mLevel;
	}

	ObjectPtr<Actor> Object::getOwner() const noexcept
	{
		return mOwner;
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

	void Object::setUpdateEnable(bool _enabled) noexcept
	{
		mUpdateEnabled = _enabled;
	}

	bool Object::isUpdateEnabled() const noexcept
	{
		return mUpdateEnabled;
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

	bool Object::invalid() const
	{
		return !IsValidByObject(this);
	}

	bool Object::operator==(const Object& _other) const noexcept
	{
		return this == &_other;
	}

	bool Object::operator!=(const Object& _other) const noexcept
	{
		return this != &_other;
	}

	Object::operator bool() const noexcept
	{
		return IsValidByObject(this);
	}
}
