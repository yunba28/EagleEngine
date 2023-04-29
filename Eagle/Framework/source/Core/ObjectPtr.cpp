#include <Core/ObjectPtr.hpp>
#include <Core/Object.hpp>
#include <Container/HashSet.hpp>

namespace EagleEngine
{

	HashSet<std::uintptr_t> gObjectHandleSet{};

	bool RegisterObjectHandle(Object* object)
	{
		if (object == nullptr)
			return false;

		auto [element, result] = gObjectHandleSet.emplace(reinterpret_cast<uintptr_t>(object));

		return result;
	}

	bool UnregisterObjectHandle(Object* object)
	{
		if (object == nullptr)
			return false;

		return gObjectHandleSet.erase(reinterpret_cast<uintptr_t>(object)) == 1;
	}

	bool IsValidByObject(const Object* object)
	{
		if (object == nullptr)
			return false;

		return gObjectHandleSet.contains(reinterpret_cast<uintptr_t>(object));
	}

}
