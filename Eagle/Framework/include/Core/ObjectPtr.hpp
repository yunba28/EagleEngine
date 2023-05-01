#pragma once

#include <CoreFwd.hpp>
#include <Utility/Concepts.hpp>

namespace EagleEngine
{
	namespace Concept
	{
		template<class ObjectA, class ObjectB>
		concept ConvertibleObject =
			(Concept::IsObject<ObjectA> && Concept::IsObject<ObjectB>) &&
			(std::is_base_of_v<ObjectA, ObjectB> || std::is_base_of_v<ObjectB, ObjectA>);
	}

	bool RegisterObjectHandle(Object* object);

	bool UnregisterObjectHandle(Object* object);

	bool IsValidByObject(const Object* object);

	template<class ObjectType>
	class ObjectPtr final
	{
	public:

		explicit ObjectPtr()noexcept
			: m_ptr(nullptr)
		{}

		explicit ObjectPtr(Object* _object)noexcept
			: m_ptr(_object)
		{}

		template<class OtherType>
		ObjectPtr(OtherType* _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
			: m_ptr(static_cast<Object*>(_other))
		{}

		template<class OtherType>
		ObjectPtr& operator=(OtherType* _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			m_ptr = static_cast<Object*>(_other);
			return *this;
		}

		explicit ObjectPtr(std::nullptr_t)noexcept
			: m_ptr(nullptr)
		{}

		ObjectPtr& operator=(std::nullptr_t)noexcept
		{
			m_ptr = nullptr;
			return *this;
		}

		ObjectPtr(ObjectPtr&&)noexcept = default;
		ObjectPtr(const ObjectPtr&)noexcept = default;
		ObjectPtr& operator=(ObjectPtr&&)noexcept = default;
		ObjectPtr& operator=(const ObjectPtr&)noexcept = default;

		template<class OtherType>
		ObjectPtr(ObjectPtr<OtherType>&& _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
			: m_ptr(std::forward<OtherType>(_other.m_ptr))
		{}

		template<class OtherType>
		ObjectPtr(const ObjectPtr<OtherType>& _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
			: m_ptr(_other.m_ptr)
		{}

		template<class OtherType>
		ObjectPtr& operator=(ObjectPtr<OtherType>&& _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			m_ptr = std::forward<OtherType>(_other.m_ptr);
			return *this;
		}

		template<class OtherType>
		ObjectPtr& operator=(const ObjectPtr<OtherType>& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			m_ptr = _other.m_ptr;
			return *this;
		}

		~ObjectPtr() = default;

		ObjectType* operator->()const noexcept
		{
			return static_cast<ObjectType*>(get());
		}

		ObjectType& operator*()noexcept
		{
			return *static_cast<ObjectType*>(get());
		}

		const ObjectType& operator*()const noexcept
		{
			return *static_cast<ObjectType*>(get());
		}

		operator ObjectType* ()const noexcept
		{
			return static_cast<ObjectType*>(get());
		}

		explicit operator bool()const noexcept
		{
			return IsValidByObject(m_ptr);
		}

		bool operator==(Object* _object)const noexcept
		{
			return m_ptr == _object;
		}

		template<class OtherType>
		bool operator==(const ObjectPtr<OtherType>& _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return m_ptr == _other.get();
		}

		bool operator!=(Object* _object)const noexcept
		{
			return m_ptr != _object;
		}

		template<class OtherType>
		bool operator!=(const ObjectPtr<OtherType>& _other)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return m_ptr != _other.get();
		}

	public:

		Object* get()const noexcept
		{
			return m_ptr;
		}

		bool invalid()const noexcept
		{
			return !IsValidByObject(m_ptr);
		}

	private:

		Object* m_ptr;

		template<class OtherType>
		friend class ObjectPtr;

	};

	template<class ObjectType>
	ObjectPtr(std::nullptr_t) -> ObjectPtr<ObjectType>;

	template<class To, class From>
	inline ObjectPtr<To> Cast(const ObjectPtr<From>& object)requires(Concept::ConvertibleObject<To, From>)
	{
		return ObjectPtr<To>(object);
	}
}
