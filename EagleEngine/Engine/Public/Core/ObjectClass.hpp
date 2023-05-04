#pragma once

#include <CoreFwd.hpp>
#include <CoreTypes.hpp>
#include <CoreUtility.hpp>
#include <Core/TypeIndex.hpp>
#include <Concept/IsType.hpp>
#include <Misc/Function.hpp>

namespace eagle
{
	enum class ObjectInherited : uint8
	{
		None = 0b00000000,
		Object = 0b00000001,
		WorldObject = 0b00000010,
		Actor = 0b00000100,
		Component = 0b00001000,
		Level = 0b00010000,
		SubLevel = 0b00100000,
		SubSystem = 0b01000000,
		World = 0b10000000
	};

	template<class ObjectType>
	inline constexpr ObjectInherited GetObjectInherited()noexcept
	{
		if constexpr (std::is_base_of_v<Actor, ObjectType>)
			return ObjectInherited::Actor;
		else if constexpr (std::is_base_of_v<Component, ObjectType>)
			return ObjectInherited::Component;
		else if constexpr (std::is_base_of_v<Level, ObjectType>)
			return ObjectInherited::Level;
		else if constexpr (std::is_base_of_v<SubLevel, ObjectType>)
			return ObjectInherited::SubLevel;
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
	inline constexpr ObjectInherited GetObjectInheritedBits()noexcept
	{
		using OI = ObjectInherited;
		uint8 result = 0;

		if constexpr (std::is_base_of_v<Actor, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject) | FromEnum(OI::Actor);
		else if constexpr (std::is_base_of_v<Component, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::WorldObject) | FromEnum(OI::Component);
		else if constexpr (std::is_base_of_v<Level, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::Level);
		else if constexpr (std::is_base_of_v<SubLevel, ObjectType>)
			result = FromEnum(OI::Object) | FromEnum(OI::SubLevel);
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

	class ObjectClass
	{
	private:

		template<class Type>
		struct init
		{
			using type = Type;
		};

	public:

		ObjectClass() = default;
		~ObjectClass() = default;

		ObjectClass(ObjectClass&&) = default;
		ObjectClass(const ObjectClass&) = default;
		ObjectClass& operator=(ObjectClass&&) = default;
		ObjectClass& operator=(const ObjectClass&) = default;

		template<Concept::IsObject ObjectType>
		ObjectClass(init<ObjectType>)
			: mFactory([]() {return static_cast<Object*>(new ObjectType()); })
			, mTypeIndex(typeid(ObjectType))
			, mInheried(GetObjectInheritedBits<ObjectType>())
		{}

	public:

		Object* operator()()const noexcept;
		Object* operator()(const String& newName)const noexcept;

		bool operator==(const ObjectClass& inClass)const noexcept
		{
			return mTypeIndex == inClass.mTypeIndex;
		}

		bool operator!=(const ObjectClass& inClass)const noexcept
		{
			return mTypeIndex != inClass.mTypeIndex;
		}

	public:

		const Function<Object*()>& factory()const noexcept
		{
			return mFactory;
		}

		const TypeIndex& type()const noexcept
		{
			return mTypeIndex;
		}

		size_t hash()const noexcept
		{
			return mTypeIndex.hash();
		}

		ObjectInherited inherit()const noexcept
		{
			return mInheried;
		}

		bool hasInherited(ObjectInherited inInherited)const noexcept
		{
			uint8 bits = static_cast<uint8>(inInherited);
			return static_cast<uint8>(mInheried) & bits;
		}

	private:

		Function<Object*()> mFactory = nullptr;

		TypeIndex mTypeIndex = typeid(void);

		ObjectInherited mInheried = ObjectInherited::None;

		template<class ObjectClassType>
		friend ObjectClass CreateObjectClass();

	};

	template<class ObjectClassType>
	ObjectClass CreateObjectClass()
	{
		return ObjectClass(ObjectClass::init<ObjectClassType>{});
	}
}
