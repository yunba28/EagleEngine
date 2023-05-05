#include <GameFramework/World.hpp>

#include <Siv3D/Scene.hpp>

namespace eagle
{
	World::World()
		: mFactories()
		, mCurrentLevel(nullptr)
		, mNextLevel(nullptr)
		, mInitLevelName(unspecified)
		, mIsPlaying(true)
	{
	}

	World::~World()
	{
		if (mCurrentLevel)
		{
			delete mCurrentLevel.get();
		}
		if (mNextLevel)
		{
			delete mNextLevel.get();
		}
	}

	bool World::build(const String& newName)
	{
		if (mCurrentLevel)
		{
			return false;
		}

		if (mFactories.contains(newName))
		{
			mCurrentLevel = mFactories.at(newName)();
		}
		else
		{
			return false;
		}

		return mIsPlaying;
	}

	bool World::update()
	{
		if (updateLevel())
		{
			drawLevel();
			return true;
		}

		return false;
	}

	bool World::changeLevel(const String& newName)
	{
		if (mFactories.contains(newName))
		{
			mNextLevel = mFactories.at(newName)();
			return true;
		}
		return false;
	}

	void World::quitPlay()
	{
		mIsPlaying = false;
	}

	bool World::updateLevel()
	{
		if (!mIsPlaying)
			return false;

		if (mCurrentLevel == nullptr)
		{
			if (!mInitLevelName)
			{
				return true;
			}
			if (!build(*mInitLevelName))
			{
				return false;
			}
		}

		if (mNextLevel)
		{
			ensure(mCurrentLevel, "empty level");
			mCurrentLevel->_internalDestruct();
			delete mCurrentLevel.get();
			mCurrentLevel = mNextLevel;
			mNextLevel = nullptr;
		}

		mCurrentLevel->_internalUpdate(s3d::Scene::DeltaTime());

		return mIsPlaying;
	}

	void World::drawLevel() const
	{
		if (mCurrentLevel)
		{
			mCurrentLevel->_internalDraw();
		}
	}
}
