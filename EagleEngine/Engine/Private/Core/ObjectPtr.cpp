#include <Core/ObjectPtr.hpp>

#include <CoreTypes.hpp>
#include <Core/Object.hpp>
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

	bool ObjectPtrBase::isA(const TypeIndex& inTypeIndex) const noexcept
	{
		if (IsValidByObject(mPtr))
		{
			return mPtr->isA(inTypeIndex);
		}
		return false;
	}

	bool ObjectPtrBase::isA(const ObjectPtrBase& inOther) const noexcept
	{
		if (IsValidByObject(mPtr) && IsValidByObject(inOther.mPtr))
		{
			return mPtr->isA(inOther.mPtr);
		}
		return false;
	}

	bool ObjectPtrBase::isA(const Object* const inObject) const noexcept
	{
		if (IsValidByObject(mPtr) && IsValidByObject(inObject))
		{
			return mPtr->isA(inObject);
		}
		return false;
	}
}
