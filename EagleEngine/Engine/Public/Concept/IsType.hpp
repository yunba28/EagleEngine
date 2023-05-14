#pragma once

#include <concepts>
#include <CoreFwd.hpp>

#define DECLARE_ISTYPE(type)\
template<class Type>\
concept Is##type = std::is_base_of_v<type,Type>

namespace eagle::Concept
{
	DECLARE_ISTYPE(Object);
	DECLARE_ISTYPE(WorldObject);
	DECLARE_ISTYPE(Actor);
	DECLARE_ISTYPE(Component);
	DECLARE_ISTYPE(Renderer);
	DECLARE_ISTYPE(Level);
	DECLARE_ISTYPE(SubLevel);
	DECLARE_ISTYPE(SubSystem);
}
