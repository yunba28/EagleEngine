#pragma once

#include <CoreFwd.hpp>

#include <Misc/Function.hpp>
#include <Utility/TypeID.hpp>
#include <Utility/Concepts.hpp>

namespace EagleEngine
{

	class ObjectClass
	{
	private:

		template<class ClassType>
		struct Identity final
		{
			using Type = ClassType;
		};

		using factory_t = Function<Object*(void)>;

	public:

		ObjectClass() noexcept
			: mFactory(nullptr)
			, mTypeID(typeid(void))
		{}

		ObjectClass(const ObjectClass&) = default;

		ObjectClass& operator=(const ObjectClass&) = default;

		ObjectClass(ObjectClass&&) = default;

		ObjectClass& operator=(ObjectClass&&) = default;

		Object* operator()()const;

		bool operator==(const ObjectClass& _other)const;

		bool operator!=(const ObjectClass& _other)const;

	public:

		const TypeID& getTypeID()const noexcept { return mTypeID; }

		String getClassName()const;

	private:

		template<Concept::IsObject ObjectType>
		ObjectClass(Identity<ObjectType>) noexcept
			: mFactory([]() {return new Identity<ObjectType>::Type(); })
			, mTypeID(typeid(Identity<ObjectType>::Type))
		{}

	private:

		factory_t mFactory = nullptr;

		TypeID mTypeID = typeid(void);

		template<class Type>
		friend ObjectClass CreateObjectClass();

	};

	template<class ObjectType>
	ObjectClass CreateObjectClass()
	{
		return ObjectClass{ ObjectClass::Identity<ObjectType>{} };
	}
}
