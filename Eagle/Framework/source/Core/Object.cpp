#include <Core/Object.hpp>
#include <Core/ObjectPtr.hpp>
#include <Core/ObjectClass.hpp>
#include <Core/Actor.hpp>
#include <Core/LevelBase.hpp>

namespace EagleEngine
{
	Object::Object()
		: m_level(nullptr)
		, m_owner(nullptr)
		, m_name()
		, m_tags()
		, m_typeID(typeid(void))
		, m_updateEnabled(true)
		, m_active(true)
		, m_pendingKill(false)
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
		if (m_active && m_updateEnabled)
		{
			update(_deltaTime);
		}
	}

	void Object::_internalAttachToLevel(LevelBase* _level)
	{
		m_level = _level;
	}

	void Object::_internalAttachToActor(Actor* _actor)
	{
		m_owner = _actor;
	}

	ObjectPtr<LevelBase> Object::getLevel() const noexcept
	{
		return m_level;
	}

	ObjectPtr<Actor> Object::getOwner() const noexcept
	{
		return m_owner;
	}

	void Object::setName(const String& _name)
	{
		m_name = Name::Make(_name);
	}

	NameView Object::getName() const noexcept
	{
		return NameView{ m_name };
	}

	bool Object::sameName(const String& _name) const noexcept
	{
		return m_name == _name;
	}

	void Object::addTag(const String& _tag)
	{
		if (!m_tags.includes(_tag))
		{
			m_tags.push_back(_tag);
		}
	}

	void Object::addTags(InitializerList<String> _tagList)
	{
		for (const auto& tag : _tagList)
		{
			if (!m_tags.includes(tag))
			{
				m_tags.push_back(tag);
			}
		}
	}

	void Object::addTags(const Array<String>& _tagList)
	{
		for (const auto& tag : _tagList)
		{
			if (!m_tags.includes(tag))
			{
				m_tags.push_back(tag);
			}
		}
	}

	HashedString Object::findTag(const String& _tag) const
	{
		if (m_tags.includes(_tag))
		{
			return HashedString{ _tag };
		}

		return HashedString{};
	}

	const Array<HashedString>& Object::getTags() const noexcept
	{
		return m_tags;
	}

	void Object::removeTag(const String& _tag)
	{
		m_tags.remove(_tag);
	}

	bool Object::hasTag(const String& _tag) const
	{
		return m_tags.includes(_tag);
	}

	const TypeID& Object::getTypeID() const noexcept
	{
		return m_typeID;
	}

	bool Object::sameTypeID(const TypeID& _typeID) const noexcept
	{
		return m_typeID == _typeID;
	}

	void Object::setUpdateEnable(bool _enabled) noexcept
	{
		m_updateEnabled = _enabled;
	}

	bool Object::isUpdateEnabled() const noexcept
	{
		return m_updateEnabled;
	}

	void Object::setActive(bool _actived) noexcept
	{
		// destroyが呼ばれた後は機能しない
		if (!m_pendingKill)
		{
			m_active = _actived;
		}
	}

	bool Object::isActive() const noexcept
	{
		return m_active;
	}

	bool Object::isPendingKill() const noexcept
	{
		return m_pendingKill;
	}

	void Object::destroy()
	{
		if (!m_pendingKill)
		{
			m_pendingKill = true;
			m_active = false;
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
