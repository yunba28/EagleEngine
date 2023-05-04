#pragma once

#include <GameFramework/SubSystem.hpp>
#include <Container/Array.hpp>
#include <Container/HashTable.hpp>

namespace eagle
{
	class WorldObjectSubSystem : public SubSystem
	{
	private:

		class WorldObjectListener
		{
		public:

			WorldObjectListener() = default;
			~WorldObjectListener() = default;

			WorldObjectListener(WorldObjectListener&&) = default;
			WorldObjectListener(const WorldObjectListener&) = delete;
			WorldObjectListener& operator=(WorldObjectListener&&) = default;
			WorldObjectListener& operator=(const WorldObjectListener&) = delete;

		public:

			void update(double inDeltaTime);
			void addWorldObject(WorldObject* inWorldObject);

		private:

			Array<ObjectPtr<WorldObject>> mObjects;
			Array<ObjectPtr<WorldObject>> mQueue;

			bool mHasPendingKill;

		};

	public:

		WorldObjectSubSystem() = default;
		~WorldObjectSubSystem() = default;

	public:

		void update(double inDeltaTime)override final;

		// create object

	private:

		HashTable<TypeIndex, WorldObjectListener> mWorldObjectTable;

		Array<TypeIndex> mExecuteOrder;
		Array<TypeIndex> mOrderQueue;

	};
}
