#pragma once

#include <GameFramework/LevelBase.hpp>
#include <Core/ObjectClass.hpp>

namespace eagle
{
	class SubLevel : public LevelBase
	{
	public:

		SubLevel() = default;
		~SubLevel() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual bool dispose()override { return true; }
		virtual void update([[maybe_unused]] double inDeltaTime)override {}

	public:
	};

	template<Concept::IsSubLevel SubLevelType>
	ObjectClass CreateSubLevelClass()
	{
		return CreateObjectClass<SubLevelType>();
	}
}
