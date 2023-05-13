#pragma once

#include <GameFramework/LevelBase.hpp>
#include <Core/ObjectClass.hpp>

namespace eagle
{
	class SubLevel : public LevelBase
	{

	};

	template<Concept::IsSubLevel SublevelType>
	ObjectClass CreateSubLevelClass()
	{
		return CreateObjectClass<SubLevelType>();
	}
}
