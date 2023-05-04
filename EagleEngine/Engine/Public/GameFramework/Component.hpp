#pragma once

#include <Core/WorldObject.hpp>

namespace eagle
{
	class Component : public WorldObject
	{
	public:

		Component() = default;
		~Component() = default;

	protected:

		virtual bool awake()override { return true; }
		virtual void start()override {}
		virtual void update(double)override {}
		virtual bool dispose()override { return true; }

	private:
	};
}
