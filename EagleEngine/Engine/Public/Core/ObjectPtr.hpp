#pragma once

#include <Core/TypeIndex.hpp>
#include <Concept/IsType.hpp>
#include <Misc/ensure.hpp>

namespace eagle
{
	namespace Concept
	{
		template<class ObjectA, class ObjectB>
		concept ConvertibleObject =
			(Concept::IsObject<ObjectA> && Concept::IsObject<ObjectB>) &&
			(std::is_base_of_v<ObjectA, ObjectB> || std::is_base_of_v<ObjectB, ObjectA>);
	}

	bool RegisterObjectHandle(Object* inObject);
	bool UnregisterObjectHandle(Object* inObject);
	bool IsValidByObject(const Object* inObject);

	template<class ObjectType>
	class ObjectPtr;

	template<class ObjectType>
	class ObjectRef;

	class ObjectPtrBase
	{
	public:

		ObjectPtrBase() = default;
		~ObjectPtrBase() = default;

		explicit ObjectPtrBase(Object* newObject)
		{
			pointerAssignment(newObject);
		}

		explicit ObjectPtrBase(std::nullptr_t)noexcept
			: mPtr(nullptr)
		{}

	public:

		bool operator==(const Object& inOther)const noexcept
		{
			return mPtr == &inOther;
		}

		bool operator==(const ObjectPtrBase& inOther)const noexcept
		{
			return mPtr == inOther.mPtr;
		}

		bool operator!=(const Object& inOther)const noexcept
		{
			return mPtr != &inOther;
		}

		bool operator!=(const ObjectPtrBase& inOther)const noexcept
		{
			return mPtr != inOther.mPtr;
		}

		explicit operator bool()const noexcept
		{
			return IsValidByObject(mPtr);
		}

	public:

		Object* get()const noexcept
		{
			return mPtr;
		}

		bool isA(const TypeIndex& inTypeIndex)const noexcept;

		bool isA(const ObjectPtrBase& inOther)const noexcept;

		bool isA(const Object* const inObject)const noexcept;

		bool invalid()const noexcept
		{
			return !IsValidByObject(mPtr);
		}

	protected:

		template<class OtherType>
		void pointerAssignment(OtherType* newObject)
		{
			if (newObject != nullptr)
			{
				if (std::is_same_v<Object, OtherType>)
				{
					mPtr = newObject;
				}
				else
				{
					mPtr = dynamic_cast<OtherType*>(newObject);
				}
			}
		}

		template<class ObjectType>
		void copyAssignment(const ObjectPtrBase& inOther)
		{
			if (inOther)
			{
				mPtr = dynamic_cast<ObjectType*>(inOther.mPtr);
			}
		}

		template<class ObjectType>
		void moveAssignment(ObjectPtrBase&& inOther)
		{
			if (inOther)
			{
				mPtr = dynamic_cast<ObjectType*>(inOther.mPtr);
				inOther.mPtr = nullptr;
			}
		}

	protected:

		Object* mPtr = nullptr;

		template<class OtherType>
		friend class ObjectPtr;

		template<class OtherType>
		friend class ObjectRef;

	};

	template<class ObjectType>
	class ObjectPtr : public ObjectPtrBase
	{
	public:

		ObjectPtr() = default;

		~ObjectPtr()
		{
			reset(nullptr);
		}

		ObjectPtr(ObjectPtr<ObjectType>&& inOther)noexcept
		{
			moveAssignment<ObjectType>(std::forward<ObjectPtr<ObjectType>>(inOther));
		}

		template<class OtherType>
		ObjectPtr(ObjectPtr<OtherType>&& inOther)noexcept
		{
			moveAssignment<ObjectType>(std::forward<ObjectPtr<OtherType>>(inOther));
		}

		ObjectPtr(const ObjectPtr<ObjectType>&) = delete;
		template<class OtherType>
		ObjectPtr(const ObjectPtr<OtherType>&) = delete;

		ObjectPtr& operator=(ObjectPtr<ObjectType>&& inOther)
		{
			reset(std::forward<ObjectPtr<ObjectType>>(inOther));
			return *this;
		}

		template<class OtherType>
		ObjectPtr& operator=(ObjectPtr<OtherType>&& inOther)
		{
			reset(std::forward<ObjectPtr<OtherType>>(inOther));
			return *this;
		}

		ObjectPtr& operator=(const ObjectPtr<ObjectType>&) = delete;
		template<class OtherType>
		ObjectPtr& operator=(const ObjectPtr<OtherType>&) = delete;

		template<class OtherType>
		ObjectPtr(OtherType* newObject)
		{
			pointerAssignment(newObject);
			ensure(RegisterObjectHandle(mPtr), "Failed register object");
		}

		ObjectPtr(std::nullptr_t)
			: ObjectPtrBase(nullptr)
		{}

	public:

		ObjectType* operator->()const noexcept
		{
			return static_cast<ObjectType*>(mPtr);
		}

		ObjectType& operator*()noexcept
		{
			return &static_cast<ObjectType*>(mPtr);
		}

		const ObjectType& operator*()const noexcept
		{
			return &static_cast<ObjectType*>(mPtr);
		}

		operator ObjectType* ()const noexcept
		{
			return static_cast<ObjectType*>(mPtr);
		}

	public:

		void reset()
		{
			reset(nullptr);
		}

		void reset(std::nullptr_t)
		{
			if (UnregisterObjectHandle(mPtr))
			{
				delete mPtr;
			}
			mPtr = nullptr;
		}

		void reset(ObjectType* newObject)
		{
			reset(nullptr);
			pointerAssignment(newObject);
			ensure(RegisterObjectHandle(mPtr), "Failed reset register object");
		}

		template<class OtherType>
		void reset(OtherType* newObject)
		{
			reset(nullptr);
			pointerAssignment(newObject);
			ensure(RegisterObjectHandle(mPtr), "Failed reset register object");
		}

		void reset(ObjectPtr<ObjectType>&& inOther)noexcept
		{
			moveAssignment<ObjectType>(std::forward<ObjectPtr<ObjectType>>(inOther));
		}

		template<class OtherType>
		void reset(ObjectPtr<OtherType>&& inOther)noexcept
		{
			moveAssignment<ObjectType>(std::forward<ObjectPtr<OtherType>>(inOther));
		}

	};

	template<class ObjectType>
	class ObjectRef : public ObjectPtrBase
	{
	public:

		ObjectRef() = default;
		~ObjectRef() = default;

		ObjectRef(ObjectRef<ObjectType>&&) = default;
		ObjectRef(const ObjectRef<ObjectType>&) = default;
		ObjectRef& operator=(ObjectRef<ObjectType>&&) = default;
		ObjectRef& operator=(const ObjectRef<ObjectType>&) = default;

		template<class OtherType>
		ObjectRef(ObjectRef<OtherType>&& inOther)
		{
			moveAssignment<ObjectType>(std::forward<ObjectRef<OtherType>>(inOther));
		}

		template<class OtherType>
		ObjectRef(const ObjectRef<OtherType>& inOther)
		{
			copyAssignment<ObjectType>(inOther);
		}

		template<class OtherType>
		ObjectRef& operator=(ObjectRef<OtherType>&& inOther)
		{
			moveAssignment<ObjectType>(std::forward<ObjectRef<OtherType>>(inOther));
			return *this;
		}

		template<class OtherType>
		ObjectRef& operator=(const ObjectRef<OtherType>& inOther)
		{
			copyAssignment<ObjectType>(inOther);
			return *this;
		}

		ObjectRef(ObjectPtr<ObjectType>&&) = delete;
		template<class OtherType>
		ObjectRef(ObjectPtr<OtherType>&&) = delete;
		ObjectRef& operator=(ObjectPtr<ObjectType>&&) = delete;
		template<class OtherType>
		ObjectRef& operator=(ObjectPtr<OtherType>&&) = delete;

		template<class OtherType>
		ObjectRef(const ObjectPtr<OtherType>& inOther)
		{
			copyAssignment<ObjectType>(inOther);
		}

		template<class OtherType>
		ObjectRef& operator=(const ObjectPtr<OtherType>& inOther)
		{
			copyAssignment<ObjectType>(inOther);
			return *this;
		}

		explicit ObjectRef(ObjectType* newObject)
		{
			pointerReference(newObject);
		}

		template<class OtherType>
		ObjectRef(OtherType* newObject)
		{
			pointerReference(newObject);
		}

		ObjectRef& operator=(ObjectType* newObject)
		{
			pointerReference(newObject);
			return *this;
		}

		template<class OtherType>
		ObjectRef& operator=(OtherType* newObject)
		{
			pointerReference(newObject);
			return *this;
		}

		ObjectRef(std::nullptr_t)
			: ObjectPtrBase(std::nullptr_t{})
		{}

		ObjectRef& operator=(std::nullptr_t)
		{
			mPtr = nullptr;
			return *this;
		}

	public:

		ObjectType* operator->()const noexcept
		{
			return static_cast<ObjectType*>(mPtr);
		}

		ObjectType& operator*()noexcept
		{
			return &static_cast<ObjectType*>(mPtr);
		}

		const ObjectType& operator*()const noexcept
		{
			return &static_cast<ObjectType*>(mPtr);
		}

		operator ObjectType* ()const noexcept
		{
			return static_cast<ObjectType*>(mPtr);
		}

	private:

		void pointerReference(ObjectType* newObject)
		{
			if (newObject != nullptr)
			{
				mPtr = newObject;
			}
		}

		template<class OtherType>
		void pointerReference(OtherType* newObject)
		{
			pointerAssignment(newObject);
		}

	};

	template<class To, class From>
	ObjectRef<To> Cast(const ObjectPtr<From>& inObjectPtr)
	{
		return ObjectRef<To>(inObjectPtr);
	}

	template<class To, class From>
	ObjectRef<To> Cast(const ObjectRef<From>& inObjectPtr)
	{
		return ObjectRef<To>(inObjectPtr);
	}
}
