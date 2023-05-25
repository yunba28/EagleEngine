#pragma once

#include <CoreFwd.hpp>
#include <CoreTypes.hpp>

namespace eagle
{
	enum class ObjectInherited : uint8
	{
		None			= 0b0000'0000'0000,
		Object			= 0b0000'0000'0001,
		WorldObject		= 0b0000'0000'0010,
		Actor			= 0b0000'0000'0100,
		Component		= 0b0000'0000'1000,
		Renderer		= 0b0000'0001'0000,
		Level			= 0b0000'0010'0000,
		SubSystem		= 0b0000'0100'0000,
		World			= 0b0000'1000'0000
	};

	template<class ObjectType>
	inline constexpr ObjectInherited GetObjectInherited()noexcept
	{
		if constexpr (std::is_base_of_v<Actor, ObjectType>)
			return ObjectInherited::Actor;
		else if constexpr (std::is_base_of_v<Component, ObjectType>)
			return ObjectInherited::Component;
		else if constexpr (std::is_base_of_v<Renderer, ObjectType>)
			return ObjectInherited::Renderer;
		else if constexpr (std::is_base_of_v<Level, ObjectType>)
			return ObjectInherited::Level;
		else if constexpr (std::is_base_of_v<SubSystem, ObjectType>)
			return ObjectInherited::SubSystem;
		else if constexpr (std::is_base_of_v<WorldObject, ObjectType>)
			return ObjectInherited::WorldObject;
		else if constexpr (std::is_base_of_v<Object, ObjectType>)
			return ObjectInherited::Object;
		else if constexpr (std::is_base_of_v<World, ObjectType>)
			return ObjectInherited::World;
		else return ObjectInherited::None;
	}

	template<class ObjectType>
	inline ObjectInherited GetObjectInheritedBits()noexcept
	{
		using OI = ObjectInherited;
		uint8 result = 0;

		if constexpr (std::is_base_of_v<Actor, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject) | FromEnum(OI::Actor);
		else if constexpr (std::is_base_of_v<Component, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject) | FromEnum(OI::Component);
		else if constexpr (std::is_base_of_v<Renderer, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject) | FromEnum(OI::Component) | FromEnum(OI::Renderer);
		else if constexpr (std::is_base_of_v<Level, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::Level);
		else if constexpr (std::is_base_of_v<SubSystem, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::SubSystem);
		else if constexpr (std::is_base_of_v<WorldObject, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject);
		else if constexpr (std::is_base_of_v<Object, ObjectType>)
			result = FromEnum(OI::Object);
		else if constexpr (std::is_base_of_v<World, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::World);

		return static_cast<ObjectInherited>(result);
	}
}
