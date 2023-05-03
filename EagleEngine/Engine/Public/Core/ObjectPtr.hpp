#pragma once

#include <Concept/IsType.hpp>

namespace eagle
{
	namespace Concept
	{
		template<class ObjectA, class ObjectB>
		concept ConvertibleObject =
			(Concept::IsObject<ObjectA> && Concept::IsObject<ObjectB>) ||
			std::is_base_of_v<ObjectA,ObjectB> || std::is_base_of_v<ObjectB, ObjectA>;
	}

	bool RegisterObjectHandle(Object* inObject);
	bool UnregisterObjectHandle(Object* inObject);
	bool IsValidByObject(Object* inObject);

	template<class ObjectType>
	class ObjectPtr final
	{

	};
}
