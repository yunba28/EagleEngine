#pragma once

namespace eagle
{
	class IUpdatable
	{
	protected:

		virtual void start() = 0;
		virtual void update([[maybe_unused]] double inDeltaTime) = 0;

	public:

		virtual void _internalUpdate([[maybe_unused]] double inDeltaTime) = 0;

	};
}
