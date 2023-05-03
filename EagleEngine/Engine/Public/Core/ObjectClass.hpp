#pragma once

#include <CoreFwd.hpp>
#include <CoreTypes.hpp>

namespace eagle
{
	enum class ObjectInherited : uint8
	{
		Object = 0b00000001,
		WorldObject = 0b00000011,
		Actor = 0b00000111,
		Component = 0b00001011,
		Level = 0b00010001,
		SubLevel = 0b00100001,
		World = 0b01000001
	};

	class ObjectClass
	{
	public:

	private:



	};
}
