#include <GameFramework/SubSystem.hpp>

#include <GameFramework/Level.hpp>

namespace eagle
{
	void SubSystem::_internalUpdate(double inDeltaTime)
	{
		if (!mStarted)
		{
			start();
			mStarted = true;
		}

		update(inDeltaTime);
	}

	void SubSystem::_internalAttachToLevel(Level* newLevel)
	{
		ensure(mLevel.invalid(), "The SubSystem is already registered in Level");
		mLevel = newLevel;
	}
}
