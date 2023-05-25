#include <Core/ExecutionOrder.hpp>

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

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex)
	{
		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : INT32_MIN;
	}

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex, ActorOrder)
	{
		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : ActorOrder::DefaultValue;
	}

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex, ComponentOrder)
	{
		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : ComponentOrder::DefaultValue;
	}

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex, RendererOrder)
	{
		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : RendererOrder::DefaultValue;
	}

	int32 ExecutionOrder::Get(const TypeIndex& inTypeIndex, SubLevelOrder)
	{
		return gOrderTable.contains(inTypeIndex) ? gOrderTable.at(inTypeIndex) : SubLevelOrder::DefaultValue;
	}
}
