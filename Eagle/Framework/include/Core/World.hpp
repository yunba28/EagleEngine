#pragma once

#include <Core/Level.hpp>

namespace EagleEngine
{
	class World : public Object
	{
	public:

		World();

		~World();

	public:

		virtual bool awake()override { return true; }

		virtual void start()override {}

		virtual void update([[maybe_unused]] double _deltaTime)override {}

		virtual void draw()const {}

		virtual bool dispose()override { return true; }

	private:

	};
}
