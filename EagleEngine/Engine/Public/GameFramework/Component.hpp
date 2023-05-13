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
		virtual void onEnable() {}
		virtual void onDisable(){}

	public:

		virtual void _internalUpdate(double inDeltaTime)override
		{
			if (getOwner().invalid())
			{
				destroy();
				return;
			}

			if (mEnabled)
			{
				WorldObject::_internalUpdate(inDeltaTime);
			}
		}

	public:

		void setEnable(bool newEnabled)noexcept
		{
			if (mEnabled == newEnabled)
				return;

			mEnabled = newEnabled;

			mEnabled ? onEnable() : onDisable();
		}

		bool isEnabled()const noexcept
		{
			return mEnabled;
		}

	private:

		bool mEnabled = true;

	};

	template<Concept::IsComponent ComponentType>
	ObjectClass CreateComponentClass()
	{
		return CreateObjectClass<ComponentType>();
	}
}
