#pragma once

#include <CoreFwd.hpp>

#include <Misc/Function.hpp>
#include <Utility/Int.hpp>
#include <Utility/TypeID.hpp>
#include <Utility/Concepts.hpp>

namespace EagleEngine
{
	enum class ObjectInherited : uint8
	{
		None = 0b000000,
		Object = 0b000001,
		Actor = 0b000011,
		Component = 0b000101,
		Level = 0b001001,
		SubLevel = 0b010001,
		World = 0b100001
	};

	template<class ObjectType>
	inline constexpr ObjectInherited MakeObjectInherited()noexcept
	{
		if constexpr (std::is_base_of_v<Actor, ObjectType>)
		{
			return ObjectInherited::Actor;
		}
		else if constexpr (std::is_base_of_v<Component, ObjectType>)
		{
			return ObjectInherited::Component;
		}
		else if constexpr (std::is_base_of_v<Level, ObjectType>)
		{
			return ObjectInherited::Level;
		}
		else if constexpr (std::is_base_of_v<SubLevel, ObjectType>)
		{
			return ObjectInherited::SubLevel;
		}
		else if constexpr (std::is_base_of_v<World, ObjectType>)
		{
			return ObjectInherited::World;
		}
		else if constexpr (std::is_base_of_v<Object, ObjectType>)
		{
			return ObjectInherited::Object;
		}
		else
		{
			return ObjectInherited::None;
		}
	}

	class ObjectClass
	{
	private:

		template<class InitType>
		struct init_t
		{
			using type = InitType;
		};

		using factory_t = Function<Object* (void)>;

	public:

		ObjectClass() noexcept
			: m_typeInfo(&typeid(void))
			, m_factory(nullptr)
			, m_inherited(ObjectInherited::None)
		{}

		ObjectClass(ObjectClass&&)noexcept = default;
		ObjectClass(const ObjectClass&)noexcept = default;
		ObjectClass& operator=(ObjectClass&&)noexcept = default;
		ObjectClass& operator=(const ObjectClass&)noexcept = default;

		Object* operator()()const
		{
			return createObject();
		}

		bool operator==(const ObjectClass& _class)const noexcept
		{
			return m_typeInfo == _class.m_typeInfo;
		}

		bool operator!=(const ObjectClass& _class)const noexcept
		{
			return m_typeInfo != _class.m_typeInfo;
		}

	public:

		Object* createObject()const;

		String getClassName()const;

		bool hasInherited(ObjectInherited _inherited)const noexcept
		{
			const uint8 bits = static_cast<uint8>(_inherited);
			return static_cast<uint8>(m_inherited) & bits;
		}

	private:

		template<Concept::IsObject ObjectType>
		ObjectClass(init_t<ObjectType>) noexcept
			: m_typeInfo(&typeid(ObjectType))
			, m_factory([]() {return new ObjectType(); })
			, m_inherited(MakeObjectInherited<ObjectType>())
		{}

	private:

		const std::type_info* m_typeInfo;

		factory_t m_factory;

		ObjectInherited m_inherited;

		template<class Type>
		friend ObjectClass CreateObjectClass();
	};

	template<class ObjectType>
	inline ObjectClass CreateObjectClass()
	{
		return ObjectClass(ObjectClass::init_t<ObjectType>{});
	}
}
