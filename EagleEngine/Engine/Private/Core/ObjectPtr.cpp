#include <Core/ObjectPtr.hpp>

#include <CoreTypes.hpp>
#include <Core/Object.hpp>
#include <Container/HashSet.hpp>

//#include <Siv3D/TextWriter.hpp>

namespace eagle
{
	HashSet<uintptr> gObjectTable;

	bool RegisterObject(Object* object)
	{
		if (object == nullptr)
			return false;

		auto [element, result] = gObjectTable.emplace(MakeHandle(object));

		return result;
	}

	bool UnregisterAndDestroyObject(Object* object)
	{
		if (object == nullptr)
			return false;

		bool result = gObjectTable.erase(MakeHandle(object)) == 1;

		object->destroy();
		delete object;

		return result;
	}

	bool IsValidByObject(const Object* object)
	{
		if (object == nullptr)
			return false;

		return gObjectTable.contains(MakeHandle(object));
	}

	void _DumpObjectMemryLeaks()
	{
#if _DEBUG

		TextWriter writer(U"Debug/dump_log.txt");

		if (gObjectTable.empty())
		{
			writer.writeln(U"The object does not have any memory leaks.");
			return;
		}

		writer.writeln(U"Object Memory Leaks!");

		for (auto ptr : gObjectTable)
		{
			Object* object = reinterpret_cast<Object*>(ptr);
			writer.writeln(U"  {} at 0X{:X}"_fmt(object->typeName(), ptr));
		}
#endif
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

	void ObjectPtrBase::CallDestroy(Object* inObject)
	{
		inObject->destroy();
	}
}
