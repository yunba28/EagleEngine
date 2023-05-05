#pragma once

#include <GameFramework/Level.hpp>

namespace eagle
{
	class World
	{
	public:

	private:

		Function<LevelBase*()> mFactory;

		ObjectPtr<LevelBase> mCurrentLevel;



	};
}
