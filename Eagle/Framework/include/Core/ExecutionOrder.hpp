#pragma once

#include <Container/Array.hpp>

#include <String/String.hpp>

#include <Utility/Int.hpp>
#include <Utility/Tuple.hpp>
#include <Utility/TypeID.hpp>
#include <Utility/Concepts.hpp>

namespace EagleEngine::ExecutionOrder
{

	Array<Pair<String, int32>> EnumrateExecutionOrder();

	void SetOrder(const TypeID& typeID, int32 order);

	template<Concept::IsObject ObjectType>
	void SetOrder(int32 order)
	{
		return SetOrder(typeid(ObjectType), order);
	}

	int32 GetOrder(const TypeID& typeID);

	template<Concept::IsObject ObjectType>
	int32 GetOrder()
	{
		return GetOrder(typeid(ObjectType));
	}
}
