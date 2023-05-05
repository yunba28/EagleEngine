#pragma once

#include <CoreTypes.hpp>
#include <Core/ObjectInherited.hpp>
#include <Core/TypeIndex.hpp>
#include <Concept/IsType.hpp>

namespace eagle
{
	struct ExecutionOrder
	{
		static constexpr int32 DefaultActorOrder = 1000;
		static constexpr int32 DefaultComponentOrder = 1500;
		static constexpr int32 DefaultSubLevelOrder = 500;

		static void Set(const TypeIndex& inTypeIndex, int32 newOrder);

		template<Concept::IsActor ActorType>
		static void SetForActor(int32 newOrder)
		{
			Set(typeid(ActorType), newOrder);
		}

		template<Concept::IsComponent ComponentType>
		static void SetForComponent(int32 newOrder)
		{
			Set(typeid(ComponentType), newOrder);
		}

		template<Concept::IsSubLevel SubLevelType>
		static void SetForSubLevel(int32 newOrder)
		{
			Set(typeid(SubLevelType), newOrder);
		}

		static int32 Get(const TypeIndex& inTypeIndex, ObjectInherited inInherited = ObjectInherited::None);

		template<Concept::IsActor ActorType>
		static int32 GetForActor()
		{
			int32 result = Get(typeid(ActorType));
			return (result == INT32_MIN) ? 1000 : result;
		}

		template<Concept::IsComponent ComponentType>
		static int32 GetForComponent()
		{
			int32 result = Get(typeid(ComponentType));
			return (result == INT32_MIN) ? 1500 : result;
		}

		template<Concept::IsSubLevel SubLevelType>
		static int32 GetForSubLevel()
		{
			int32 result = Get(typeid(SubLevelType));
			return (result == INT32_MIN) ? 500 : result;
		}
	};
}
