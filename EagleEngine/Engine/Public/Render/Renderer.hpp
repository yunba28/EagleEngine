#pragma once

#include <GameFramework/Component.hpp>
#include <Render/RenderType.hpp>

namespace eagle
{
	class Renderer : public Component
	{
	public:

		Renderer() = default;
		~Renderer() = default;

	protected:

		virtual void draw()const = 0;

	public:

		virtual void _internalConstruct()override;
		virtual void _internalDraw()const;
		virtual void _internalRegisterRenderSubSystem(class RendererSubSystem* inSubSystem);

	protected:

		ObjectRef<RendererSubSystem> getRendererSubSystem()const noexcept
		{
			return mRendererSubSystem;
		}

	public:

		void setRenderType(RenderType newRenderType)noexcept;

		RenderType getRenderType()const noexcept
		{
			return mRenderType;
		}

		bool isRenderType(RenderType inRenderType)const noexcept
		{
			return mRenderType == inRenderType;
		}

		void setDrawEnable(bool newEnabled)noexcept
		{
			mDrawEnabled = newEnabled;
		}

		bool isDrawEnabled()const noexcept
		{
			return mDrawEnabled;
		}

	private:

		ObjectRef<class RendererSubSystem> mRendererSubSystem;

		RenderType mRenderType = RenderType::Screen;

		bool mDrawEnabled = true;

	};
}
