#pragma once

#include <Core/Object.hpp>

namespace EagleEngine
{
	class Component : public Object
	{
	public:

		Component();

	protected:

		/* category 'event' */

		virtual bool awake()override { return true; }

		virtual void start() = 0;

		virtual void update([[maybe_unused]] double _deltaTime) = 0;

		virtual bool dispose()override { return true; }

		virtual void onEnable() {}

		virtual void onDisable() {}

		/* category 'advanced' */

		virtual void updateFadeIn([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) {}

		virtual void updateFadeOut([[maybe_unused]] double _deltaTime, [[maybe_unused]] double _progress) {}

	public:

		virtual void _internalUpdate(double _deltaTime)override;

		virtual void _internalUpdateFadeIn(double _deltaTime, double _progress)override;

		virtual void _internalUpdateFadeOut(double _deltaTime, double _progress)override;

	public:

		/* category 'common' */

		void setComponentEnable(bool _enabled)noexcept;

		bool isComponentEnabled()const noexcept;

	private:

		bool mComponentEnable;

	};
}
