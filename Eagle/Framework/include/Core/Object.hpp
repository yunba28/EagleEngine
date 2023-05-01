#pragma once

#include <CoreMinimal.hpp>

namespace EagleEngine
{

	class Object
	{
	public:

		explicit Object();
		virtual ~Object() = default;

		Object(Object&&) = default;
		Object(const Object&) = delete;
		Object& operator=(Object&&) = default;
		Object& operator=(const Object&) = delete;

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
		virtual void _internalAttachToLevel(LevelBase* _level);
		virtual void _internalAttachToActor(Actor* _actor);

	public:

		/* category 'common' */

		ObjectPtr<LevelBase> getLevel()const noexcept
		{
			return m_level;
		}

		ObjectPtr<Actor> getOwner()const noexcept;

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

		ObjectPtr<LevelBase> m_level;

		ObjectPtr<Actor> m_owner;

		Name m_name;

		Array<HashedString> m_tags;

		TypeID m_typeID;

		bool m_updateEnabled;

		bool m_active;

		bool m_pendingKill;

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
