#include <Core/ObjectPtr.hpp>

#include <CoreTypes.hpp>
#include <Container/HashSet.hpp>

namespace eagle
{
	HashSet<uintptr> gObjectTable;

	bool RegisterObjectHandle(Object* inObject)
	{
		
		return false;
	}
	bool UnregisterObjectHandle(Object* inObject)
	{
		return false;
	}
	bool IsValidByObject(Object* inObject)
	{
		return false;
	}
}
