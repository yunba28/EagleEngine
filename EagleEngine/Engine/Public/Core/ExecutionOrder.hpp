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
		static constexpr int32 DefaultRendererOrder = 2000;
		static constexpr int32 DefaultSubLevelOrder = 500;

		struct ActorOrder
		{
			static constexpr int32 DefaultValue = 1000;
		};

		struct ComponentOrder
		{
			static constexpr int32 DefaultValue = 1500;
		};

		struct RendererOrder
		{
			static constexpr int32 DefaultValue = 2000;
		};

		struct SubLevelOrder
		{
			static constexpr int32 DefaultValue = 500;
		};

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

		template<Concept::IsRenderer RendererType>
		static void SetForRenderer(int32 newOrder)
		{
			Set(typeid(RendererType), newOrder);
		}

		static int32 Get(const TypeIndex& inTypeIndex);
		static int32 Get(const TypeIndex& inTypeIndex, ActorOrder);
		static int32 Get(const TypeIndex& inTypeIndex, ComponentOrder);
		static int32 Get(const TypeIndex& inTypeIndex, RendererOrder);
		static int32 Get(const TypeIndex& inTypeIndex, SubLevelOrder);

		template<Concept::IsActor ActorType>
		static int32 GetForActor()
		{
			return Get(typeid(ActorType), ActorOrder{});
		}

		template<Concept::IsComponent ComponentType>
		static int32 GetForComponent()
		{
			return Get(typeid(ComponentType), ComponentOrder{});
		}

		template<Concept::IsRenderer RendererType>
		static int32 GetForRenderer()
		{
			return Get(typeid(RendererType), RendererOrder{});
		}
	};
}
