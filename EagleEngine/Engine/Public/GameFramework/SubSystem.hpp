#pragma once

#include <Core/Object.hpp>
#include <Interface/IUpdatable.hpp>

namespace eagle
{
	class SubSystem : public Object, public IUpdatable
	{
	public:

		SubSystem() = default;
		~SubSystem() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual bool dispose()override { return true; }
		virtual void start()override {}

	public:

		virtual void _internalUpdate(double inDeltaTime)override;
		virtual void _internalAttachToLevel(Level* newLevel);

	public:

		ObjectRef<Level> getLevel()const noexcept
		{
			return mLevel;
		}

	private:

		ObjectRef<Level> mLevel = nullptr;

		bool mStarted = false;

		friend class Level;

	};
}
