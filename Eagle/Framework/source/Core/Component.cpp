#include <Core/Component.hpp>

namespace EagleEngine
{
	Component::Component()
		: mComponentEnable(true)
	{
	}

	void Component::_internalUpdate(double _deltaTime)
	{
		if (mComponentEnable)
		{
			Object::_internalUpdate(_deltaTime);
		}
	}

	void Component::setComponentEnable(bool _enabled) noexcept
	{
		if (!isPendingKill())
		{
			if (mComponentEnable != _enabled)
			{
				mComponentEnable = _enabled;
				mComponentEnable ? onEnable() : onDisable();
			}
		}
	}

	bool Component::isComponentEnabled() const noexcept
	{
		return mComponentEnable;
	}
}
