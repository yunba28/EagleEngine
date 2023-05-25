#pragma once

#include <CoreColors.hpp>
#include <GameFramework/SubSystem.hpp>
#include <Render/RenderType.hpp>
#include <Misc/Function.hpp>

#include <Siv3D/Camera2D.hpp>
#include <Siv3D/DebugCamera3D.hpp>
#include <Siv3D/MSRenderTexture.hpp>

namespace eagle
{
	class RendererSubSystem : public SubSystem
	{
	private:

		class RendererListener
		{
		public:

			RendererListener();
			~RendererListener();

			RendererListener(RendererListener&&) = default;
			RendererListener(const RendererListener&) = delete;
			RendererListener& operator=(RendererListener&&) = default;
			RendererListener& operator=(const RendererListener&) = delete;

		public:

			void update();
			bool drawWorldSpace()const;
			bool drawScreen()const;
			bool drawUI()const;
			void addRenderer(Renderer* inRenderer);
			void notifyChangeRenderType()noexcept;

		private:

			void checkHasRenderType();

		private:

			Array<ObjectRef<Renderer>> mRenderers;
			Array<ObjectRef<Renderer>> mQueue;

			bool mHasPendingKill;

			bool mNotifyChangeRenderType;

			bool mHasDrawWorldSpace;
			bool mHasDrawScreen;
			bool mHasDrawUI;

		};

	public:

		RendererSubSystem();
		~RendererSubSystem();

	protected:

		void update(double inDeltaTime);

	public:

		virtual void _internalAttachToLevel(Level* newLevel)override;

	public:

		void draw()const;
		void addRenderer(const TypeIndex& inTypeIndex, Renderer* newRenderer);
		void notifyChangeRenderType(const TypeIndex& inTypeIndex)noexcept;

		std::shared_ptr<BasicCamera2D> getBasicCamera2D()const noexcept
		{
			return mCamera2D;
		}

		std::shared_ptr<BasicCamera3D> getBasicCamera3D()const noexcept
		{
			return mCamera3D;
		}

	private:

		HashTable<TypeIndex,RendererListener> mRendererTable;

		Array<TypeIndex> mExecutionOrder;
		Array<TypeIndex> mOrderQueue;

		std::shared_ptr<s3d::BasicCamera2D> mCamera2D;
		std::shared_ptr<s3d::BasicCamera3D> mCamera3D;

		s3d::MSRenderTexture mRenderTexture;

		Color mBackgroundColor;

	};
}
