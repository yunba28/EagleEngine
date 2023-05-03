#include <Core/ObjectPtr.hpp>

#include <CoreTypes.hpp>
#include <Container/HashSet.hpp>

namespace eagle
{
	HashSet<uintptr> gObjectTable;

	bool RegisterObjectHandle(Object* object)
	{
		if (object == nullptr)
			return false;

		auto [element, result] = gObjectTable.emplace(MakeHandle(object));

		return result;
	}

	bool UnregisterObjectHandle(Object* object)
	{
		if (object == nullptr)
			return false;

		return gObjectTable.erase(MakeHandle(object)) == 1;
	}

	bool IsValidByObject(const Object* object)
	{
		if (object == nullptr)
			return false;

		return gObjectTable.contains(MakeHandle(object));
	}
}
