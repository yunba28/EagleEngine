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

		}

	private:

		Object* mObjectPtr = nullptr;

	};
}
