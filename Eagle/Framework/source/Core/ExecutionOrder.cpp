#include <Core/ExecutionOrder.hpp>
#include <Core/Actor.hpp>
#include <Container/HashTable.hpp>

namespace EagleEngine::ExecutionOrder
{
	HashTable<TypeID, int32> gObjectOrders
	{
		{TypeID(typeid(Actor)),500}
	};

	Array<Pair<String, int32>> EnumrateExecutionOrder()
	{
		using KeyVal = Pair<String, int32>;

		Array<KeyVal> result{ s3d::Arg::reserve = gObjectOrders.size() };

		for (const auto& [key,val] : gObjectOrders)
		{
			result.push_back({ key.name(),val });
		}

		result.stable_sort_by([](const KeyVal& a, const KeyVal& b)
		{
			return a.second < b.second;
		});

		return result;
	}

	void SetOrder(const TypeID& typeID, int32 order)
	{
		gObjectOrders[typeID] = order;
	}

	int32 GetOrder(const TypeID& typeID)
	{
		return gObjectOrders.contains(typeID) ? gObjectOrders[typeID] : 500;
	}
}
