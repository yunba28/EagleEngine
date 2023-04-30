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

		virtual bool awake()override;

		virtual void start()override;

		virtual void update(double _deltaTime)override;

		virtual bool dispose()override;

	public:



	private:

		

	};
}
