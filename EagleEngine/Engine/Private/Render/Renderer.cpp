#include <Render/Renderer.hpp>

#include <Core/RendererSubSystem.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	void Renderer::_internalConstruct()
	{
		if (auto level = getLevel(); level)
		{
			if (auto subsystem = level->getRendererSubSystem(); subsystem)
			{
				_internalRegisterRenderSubSystem(subsystem);
			}
		}

		WorldObject::_internalConstruct();
	}

	void Renderer::_internalDraw() const
	{
		if (mDrawEnabled)
		{
			draw();
		}
	}

	void Renderer::_internalRegisterRenderSubSystem(RendererSubSystem* inSubSystem)
	{
		mRendererSubSystem = inSubSystem;
		if (mRendererSubSystem)
		{
			mRendererSubSystem->addRenderer(type(), this);
		}
	}

	void Renderer::setRenderType(RenderType newRenderType) noexcept
	{
		mRenderType = newRenderType;
		if (mRendererSubSystem)
		{
			mRendererSubSystem->notifyChangeRenderType(type());
		}
	}
}
