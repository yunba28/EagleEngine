#pragma once

#include <Core/Level.hpp>

namespace EagleEngine
{
	class World : public Object
	{
	public:

		World();

		~World();

	private:

		virtual bool awake()override;

		virtual void start()override;

		virtual void update(double _deltaTime)override;

		virtual bool dispose()override;

	public:

		World& registerLevel();

		template<Concept::IsLevel LevelType>
		World& registerLevel(const String& _name);

	public:

		static bool UpdateWorld(World* world);

	private:

		HashTable<String, ObjectClass> mLevelFactories;

		ObjectPtr<Level> mCurrentLevel;

		String mNextLevelName;

		bool mIsPlaying;

	};
}
