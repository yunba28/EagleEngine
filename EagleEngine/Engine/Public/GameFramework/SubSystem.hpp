#pragma once

#include <Core/Object.hpp>

namespace eagle
{
	class SubSystem : public Object
	{
	public:

		SubSystem() = default;
		~SubSystem() = default;

	protected:

		virtual void update([[maybe_unused]] double inDeltaTime) = 0;

	private:

		friend class LevelBase;
		friend class Level;
		friend class SubLevel;

	};
}
