#pragma once

#include <GameFramework/Level.hpp>
#include <Misc/Optional.hpp>

namespace eagle
{
	class World : public Object
	{
	public:

		World();
		~World();

	public:

		template<Concept::IsLevel LevelType>
		World& registerLevel(const String& inName)
		{
			if (mFactories.empty())
			{
				mInitLevelName = inName;
			}
			if (!mFactories.contains(inName))
			{
				mFactories.emplace(inName, [newName = inName]()
				{
					Level* newLevel = static_cast<Level*>(CreateObjectClass<LevelType>()(newName));
					{
						newLevel->_internalConstruct();
					}
					return newLevel;
				});
			}
			return *this;
		}

		World& unregisterLevel(const String& inName)
		{
			mFactories.erase(inName);
			return *this;
		}

		bool build(const String& newName);

		bool update();

		bool changeLevel(const String& newName);

		void quitPlay();

	private:

		bool updateLevel();

		void drawLevel()const;

	private:

		HashTable<String, Function<Level*()>> mFactories;

		ObjectPtr<Level> mCurrentLevel;

		ObjectPtr<Level> mNextLevel;

		Optional<String> mInitLevelName;

		bool mIsPlaying;

	};
}
