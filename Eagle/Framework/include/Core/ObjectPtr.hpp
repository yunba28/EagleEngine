#pragma once

#include <Core/CoreFwd.hpp>
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

		explicit ObjectPtr()noexcept;

		explicit ObjectPtr(Object* _object)noexcept;

		template<class OtherType>
		ObjectPtr(OtherType* _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		template<class OtherType>
		ObjectPtr& operator=(OtherType* _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		ObjectPtr(const ObjectPtr&)noexcept = default;

		template<class OtherType>
		ObjectPtr(const ObjectPtr<OtherType>& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		ObjectPtr& operator=(const ObjectPtr&)noexcept = default;

		template<class OtherType>
		ObjectPtr& operator=(const ObjectPtr<OtherType>& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		ObjectPtr(ObjectPtr&&)noexcept = default;

		template<class OtherType>
		ObjectPtr(ObjectPtr<OtherType>&& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		ObjectPtr& operator=(ObjectPtr&&)noexcept = default;

		template<class OtherType>
		ObjectPtr& operator=(ObjectPtr<OtherType>&& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		explicit ObjectPtr(std::nullptr_t)noexcept;

		ObjectPtr& operator=(std::nullptr_t)noexcept;

		~ObjectPtr() = default;

		ObjectType* operator->()const noexcept;

		ObjectType& operator*()noexcept;

		const ObjectType& operator*()const noexcept;

		operator ObjectType* ()const noexcept;

		explicit operator bool()const noexcept;

		bool operator==(Object* _object)const noexcept;

		template<class OtherType>
		bool operator==(const ObjectPtr<OtherType>& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

		bool operator!=(Object* _object)const noexcept;

		template<class OtherType>
		bool operator!=(const ObjectPtr<OtherType>& _otherObject)requires(Concept::ConvertibleObject<ObjectType, OtherType>);

	public:

		Object* get()const noexcept;

		bool invalid()const noexcept;

	private:

		Object* mPtr;

		template<class OtherType>
		friend class ObjectPtr;

	};

	template<class ObjectType>
	ObjectPtr(std::nullptr_t) -> ObjectPtr<ObjectType>;

	template<class ObjectType>
	inline ObjectPtr<ObjectType>::ObjectPtr() noexcept
		: mPtr(nullptr)
	{
	}

	template<class ObjectType>
	inline ObjectPtr<ObjectType>::ObjectPtr(Object* _object) noexcept
		: mPtr(_object)
	{
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>::ObjectPtr(OtherType* _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		: mPtr(static_cast<Object*>(_otherObject))
	{
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>& ObjectPtr<ObjectType>::operator=(OtherType* _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
	{
		mPtr = static_cast<Object*>(_otherObject);
		return *this;
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>::ObjectPtr(const ObjectPtr<OtherType>& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		: mPtr(_otherObject.mPtr)
	{
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>& ObjectPtr<ObjectType>::operator=(const ObjectPtr<OtherType>& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
	{
		mPtr = _otherObject.mPtr;
		return *this;
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>::ObjectPtr(ObjectPtr<OtherType>&& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		: mPtr(std::forward<Object*>(_otherObject.mPtr))
	{
		_otherObject.mPtr = nullptr;
	}

	template<class ObjectType>
	template<class OtherType>
	ObjectPtr<ObjectType>& ObjectPtr<ObjectType>::operator=(ObjectPtr<OtherType>&& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
	{
		mPtr = std::forward<Object*>(_otherObject.mPtr);
		_otherObject.mPtr = nullptr;
		return *this;
	}

	template<class ObjectType>
	ObjectPtr<ObjectType>::ObjectPtr(std::nullptr_t)noexcept
		: mPtr(nullptr)
	{}

	template<class ObjectType>
	ObjectPtr<ObjectType>& ObjectPtr<ObjectType>::operator=(std::nullptr_t)noexcept
	{
		mPtr = nullptr;
		return *this;
	}

	template<class ObjectType>
	inline ObjectType* ObjectPtr<ObjectType>::operator->() const noexcept
	{
		return static_cast<ObjectType*>(get());
	}

	template<class ObjectType>
	inline ObjectType& ObjectPtr<ObjectType>::operator*() noexcept
	{
		return *static_cast<ObjectType*>(get());
	}

	template<class ObjectType>
	inline const ObjectType& ObjectPtr<ObjectType>::operator*() const noexcept
	{
		return *static_cast<ObjectType*>(get());
	}

	template<class ObjectType>
	inline ObjectPtr<ObjectType>::operator ObjectType*() const noexcept
	{
		return static_cast<ObjectType*>(get());
	}

	template<class ObjectType>
	inline ObjectPtr<ObjectType>::operator bool() const noexcept
	{
		return IsValidByObject(mPtr);
	}

	template<class ObjectType>
	inline bool ObjectPtr<ObjectType>::operator==(Object* _object) const noexcept
	{
		return mPtr == _object;
	}

	template<class ObjectType>
	template<class OtherType>
	inline bool ObjectPtr<ObjectType>::operator==(const ObjectPtr<OtherType>& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
	{
		return mPtr == _otherObject.mPtr;
	}

	template<class ObjectType>
	inline bool ObjectPtr<ObjectType>::operator!=(Object* _object) const noexcept
	{
		return mPtr != _object;
	}

	template<class ObjectType>
	template<class OtherType>
	inline bool ObjectPtr<ObjectType>::operator!=(const ObjectPtr<OtherType>& _otherObject)
		requires(Concept::ConvertibleObject<ObjectType, OtherType>)
	{
		return mPtr != _otherObject.mPtr;
	}

	template<class ObjectType>
	inline Object* ObjectPtr<ObjectType>::get() const noexcept
	{
		return mPtr;
	}

	template<class ObjectType>
	inline bool ObjectPtr<ObjectType>::invalid() const noexcept
	{
		return !IsValidByObject(mPtr);
	}

	template<class To, class From>
	inline ObjectPtr<To> Cast(const ObjectPtr<From>& object)requires(Concept::ConvertibleObject<To, From>)
	{
		return ObjectPtr<To>(object);
	}
}
