#pragma once

#include <Concept/IsType.hpp>

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
	class ObjectPtr final
	{
	public:

		ObjectPtr() = default;
		~ObjectPtr() = default;

		ObjectPtr(ObjectPtr&&)noexcept = default;
		ObjectPtr(const ObjectPtr&)noexcept = default;
		ObjectPtr& operator=(ObjectPtr&&)noexcept = default;
		ObjectPtr& operator=(const ObjectPtr&)noexcept = default;

		template<class OtherType>
		ObjectPtr(ObjectPtr<OtherType>&& inOtherPtr)requires(Concept::ConvertibleObject<ObjectType,OtherType>)
		{
			MoveAssignment(std::forward<ObjectPtr<OtherType>>(inOtherPtr));
		}

		template<class OtherType>
		ObjectPtr(const ObjectPtr<OtherType>& inOtherPtr)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			CopyAssignment(inOtherPtr);
		}

		template<class OtherType>
		ObjectPtr& operator=(ObjectPtr<OtherType>&& inOtherPtr)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			MoveAssignment(std::forward<ObjectPtr<OtherType>>(inOtherPtr));
			return *this;
		}

		template<class OtherType>
		ObjectPtr& operator=(const ObjectPtr<OtherType>& inOtherPtr)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			CopyAssignment(inOtherPtr);
			return *this;
		}

		explicit ObjectPtr(Object* inObject)noexcept
			: mObjectPtr(inObject)
		{}

		ObjectPtr& operator=(Object* inObject)noexcept
		{
			mObjectPtr = inObject;
			return *this;
		}

		template<class OtherType>
		ObjectPtr(OtherType* inOtherPtr)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			PointerAssignment(inOtherPtr);
		}

		template<class OtherType>
		ObjectPtr& operator=(OtherType* inOtherPtr)requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			PointerAssignment(inOtherPtr);
			return *this;
		}

		ObjectPtr(std::nullptr_t)noexcept
			: mObjectPtr(nullptr)
		{}

		ObjectPtr& operator=(std::nullptr_t)noexcept
		{
			mObjectPtr = nullptr;
			return *this;
		}

	public:

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

		template<class OtherType>
		bool operator==(OtherType* inOtherPtr)const noexcept requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return mObjectPtr == inOtherPtr;
		}

		template<class OtherType>
		bool operator==(const ObjectPtr<OtherType>& inOtherPtr)const noexcept requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return mObjectPtr == inOtherPtr.mObjectPtr;
		}

		template<class OtherType>
		bool operator!=(OtherType* inOtherPtr)const noexcept requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return mObjectPtr != inOtherPtr;
		}

		template<class OtherType>
		bool operator!=(const ObjectPtr<OtherType>& inOtherPtr)const noexcept requires(Concept::ConvertibleObject<ObjectType, OtherType>)
		{
			return mObjectPtr != inOtherPtr.mObjectPtr;
		}

		explicit operator bool()const
		{
			return IsValidByObject(mObjectPtr);
		}

		operator ObjectType* ()const noexcept
		{
			return static_cast<ObjectType*>(mObjectPtr);
		}

	public:

		Object* get()const noexcept
		{
			return mObjectPtr;
		}

		bool invalid()const noexcept
		{
			return !IsValidByObject(mObjectPtr);
		}

	private:

		template<class OtherType>
		void PointerAssignment(OtherType* inOtherPtr)
		{
			if (inOtherPtr->invalid())
			{
				mObjectPtr = nullptr;
				return;
			}

			Object* ptr = dynamic_cast<ObjectType*>(inOtherPtr);
			assert(("Failed to cast ObjectPtr", ptr != nullptr));

			mObjectPtr = ptr;
		}

		template<class OtherType>
		void MoveAssignment(ObjectPtr<OtherType>&& inOtherPtr)
		{
			if (inOtherPtr.invalid())
			{
				mObjectPtr = nullptr;
				return;
			}

			Object* ptr = dynamic_cast<ObjectType*>(inOtherPtr.mObjectPtr);
			assert(("Failed to move cast ObjectPtr", ptr != nullptr));

			mObjectPtr = ptr;
			inOtherPtr.mObjectPtr = nullptr;
		}

		template<class OtherType>
		void CopyAssignment(const ObjectPtr<OtherType>& inOtherPtr)
		{
			if (inOtherPtr.invalid())
			{
				mObjectPtr = nullptr;
				return;
			}

			Object* ptr = dynamic_cast<ObjectType*>(inOtherPtr.mObjectPtr);
			assert(("Failed to copy cast ObjectPtr", ptr != nullptr));

			mObjectPtr = ptr;
		}

	private:

		Object* mObjectPtr = nullptr;

	};
}
