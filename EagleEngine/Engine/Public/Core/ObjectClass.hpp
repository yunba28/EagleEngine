#pragma once

#include <CoreFwd.hpp>
#include <CoreUtility.hpp>
#include <Core/ObjectInherited.hpp>
#include <Core/TypeIndex.hpp>
#include <Concept/IsType.hpp>
#include <Misc/Function.hpp>

namespace eagle
{

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
