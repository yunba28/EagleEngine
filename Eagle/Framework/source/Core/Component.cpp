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

	void Component::_internalUpdateFadeIn(double _deltaTime, double _progress)
	{
		if (mComponentEnable)
		{
			Object::_internalUpdateFadeIn(_deltaTime, _progress);
		}
	}

	void Component::_internalUpdateFadeOut(double _deltaTime, double _progress)
	{
		if (mComponentEnable)
		{
			Object::_internalUpdateFadeOut(_deltaTime, _progress);
		}
	}

	void Component::setComponentEnable(bool _enabled) noexcept
	{
		if (!isPendingKill() && (mComponentEnable != _enabled))
		{
			mComponentEnable = _enabled;
			mComponentEnable ? onEnable() : onDisable();
		}
	}

	bool Component::isComponentEnabled() const noexcept
	{
		return mComponentEnable;
	}
}
