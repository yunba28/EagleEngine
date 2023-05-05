﻿#include <Core/ExecutionOrder.hpp>

#include <GameFramework/Actor.hpp>
#include <Container/HashTable.hpp>

namespace eagle
{

	HashTable<TypeIndex, int32> gOrderTable
	{
		{typeid(Actor),1000}
	};

	void ExecutionOrder::Set(const TypeIndex& inTypeIndex, int32 newOrder)
	{
		gOrderTable[inTypeIndex] = newOrder;
	}

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex, ObjectInherited inInherited)
	{
		if (inInherited == ObjectInherited::None)
		{
			return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : INT32_MIN;
		}

		if (inInherited == ObjectInherited::Actor)
		{
			return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : 1000;
		}

		if (inInherited == ObjectInherited::Component)
		{
			return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : 1500;
		}

		if (inInherited == ObjectInherited::SubLevel)
		{
			return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : 500;
		}

		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : INT32_MIN;
	}
}
