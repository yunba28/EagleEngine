#include <GameFramework/SubSystem.hpp>

#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	void SubSystem::_internalAttachToLevel(LevelBase* newLevel)
	{
		ensure(mLevel.invalid(), "The SubSystem is already registered in Level");
		mLevel = newLevel;
	}
}
