#include <Core/RendererSubSystem.hpp>

#include <Render/Renderer.hpp>
#include <Core/ExecutionOrder.hpp>
#include <GameFramework/SubLevel.hpp>

namespace eagle
{
	RendererSubSystem::RendererListener::RendererListener()
		: mRenderers()
		, mQueue()
		, mHasPendingKill(false)
		, mNotifyChangeRenderType(false)
		, mHasDrawWorldSpace(false)
		, mHasDrawScreen(false)
		, mHasDrawUI(false)
	{
	}

	RendererSubSystem::RendererListener::~RendererListener()
	{
		mRenderers.clear();
	}

	void RendererSubSystem::RendererListener::update()
	{
		if (mHasPendingKill)
		{
			mRenderers.remove_if([](const ObjectRef<Renderer>& inTarget)
			{
				return inTarget.invalid() || inTarget->pendingKill();
			});
			mHasPendingKill = false;
			mNotifyChangeRenderType = true;
		}

		if (!mQueue.empty())
		{
			mRenderers.insert(
				mRenderers.end(),
				std::move_iterator{ mQueue.begin() },
				std::move_iterator{ mQueue.end() }
			);
			mQueue.clear();
			mNotifyChangeRenderType = true;
		}

		if (mNotifyChangeRenderType)
		{
			checkHasRenderType();
			mNotifyChangeRenderType = false;
		}

		for (const auto& renderer : mRenderers)
		{
			if (renderer.invalid() || renderer->pendingKill())
			{
				mHasPendingKill = true;
				break;
			}
		}
	}

	bool RendererSubSystem::RendererListener::drawWorldSpace() const
	{
		if (!mHasDrawWorldSpace)
			return false;

		for (const auto& renderer : mRenderers)
		{
			if (renderer->isRenderType(RenderType::WorldSpace))
			{
				renderer->_internalDraw();
			}
		}

		return true;
	}

	bool RendererSubSystem::RendererListener::drawScreen() const
	{
		if (!mHasDrawScreen)
			return false;

		for (const auto& renderer : mRenderers)
		{
			if (renderer->isRenderType(RenderType::Screen))
			{
				renderer->_internalDraw();
			}
		}

		return true;
	}

	bool RendererSubSystem::RendererListener::drawUI() const
	{
		if (!mHasDrawUI)
			return false;

		for (const auto& renderer : mRenderers)
		{
			if (renderer->isRenderType(RenderType::UI))
			{
				renderer->_internalDraw();
			}
		}

		return true;
	}

	void RendererSubSystem::RendererListener::addRenderer(Renderer* inRenderer)
	{
		mQueue.push_back(ObjectRef<Renderer>(inRenderer));
	}

	void RendererSubSystem::RendererListener::notifyChangeRenderType() noexcept
	{
		mNotifyChangeRenderType = true;
	}

	void RendererSubSystem::RendererListener::checkHasRenderType()
	{
		mHasDrawWorldSpace = false;
		mHasDrawScreen = false;
		mHasDrawUI = false;
		for (const auto& renderer : mRenderers)
		{
			mHasDrawWorldSpace |= renderer->isRenderType(RenderType::WorldSpace);
			mHasDrawScreen |= renderer->isRenderType(RenderType::Screen);
			mHasDrawUI |= renderer->isRenderType(RenderType::UI);
		}
	}

	using namespace s3d;

	RendererSubSystem::RendererSubSystem()
		: mRendererTable()
		, mExecutionOrder()
		, mOrderQueue()
		, mCamera2D(std::make_shared<Camera2D>(s3d::Scene::CenterF()))
		, mCamera3D(std::make_shared<DebugCamera3D>(s3d::Scene::Size(), 30_deg, Vec3{ 10, 16, -32 }))
		, mRenderTexture(s3d::Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes)
		, mBackgroundColor(Palette::DefaultBackground)
		, mIsSubLevel(false)
	{
	}

	RendererSubSystem::~RendererSubSystem()
	{
	}

	void RendererSubSystem::update(double inDeltaTime)
	{
		(void)inDeltaTime;

		if (!mOrderQueue.isEmpty())
		{
			mExecutionOrder.append(mOrderQueue);
			mOrderQueue.clear();
			mExecutionOrder.sort_by([](const TypeIndex& inLeft, const TypeIndex& inRight)
			{
				return ExecutionOrder::Get(inLeft, ExecutionOrder::RendererOrder{}) < ExecutionOrder::Get(inRight, ExecutionOrder::RendererOrder{});
			});
		}

		for (const auto& type : mExecutionOrder)
		{
			mRendererTable[type].update();
		}
	}

	void RendererSubSystem::_internalAttachToLevel(LevelBase* newLevel)
	{
		SubSystem::_internalAttachToLevel(newLevel);
		mIsSubLevel = newLevel->hasInherited(ObjectInherited::SubLevel);
	}

	void RendererSubSystem::draw() const
	{
		// Drawing WorldSpace
		if(mIsSubLevel)
		{
			for (const auto& level : mSubLevels)
			{
				level->_internalDraw();
			}

			for (const auto& type : mExecutionOrder)
			{
				mRendererTable.at(type).drawWorldSpace();
			}
		}
		else
		{
			// 3D シーンにカメラを設定
			Graphics3D::SetCameraTransform(*mCamera3D);

			// mRenderTexture を背景色で塗りつぶし、
			// mRenderTexture を 3D 描画のレンダーターゲットに
			const ScopedRenderTarget3D target{ mRenderTexture.clear(mBackgroundColor.removeSRGBCurve()) };

			// 3D描画が行われたかどうか
			bool processed = false;

			for (const auto& type : mExecutionOrder)
			{
				processed |= mRendererTable.at(type).drawWorldSpace();
			}

			// 3D シーンを 2D シーンに描画
			if(processed)
			{
				// renderTexture を resolve する前に 3D 描画を実行する
				Graphics3D::Flush();

				// マルチサンプル・テクスチャのリゾルブ
				mRenderTexture.resolve();

				// リニアレンダリングされた renderTexture をシーンに転送
				Shader::LinearToScreen(mRenderTexture);
			}
			// 3D描画が行われていなければ背景を2D用に塗りつぶす
			else
			{
				s3d::Scene::SetBackground(mBackgroundColor);
			}
		}

		// Drawing Screen
		{
			const auto transformer = mCamera2D->createTransformer();

			for (const auto& type : mExecutionOrder)
			{
				mRendererTable.at(type).drawScreen();
			}
		}

		// Drawing UI
		for (const auto& type : mExecutionOrder)
		{
			mRendererTable.at(type).drawUI();
		}
	}

	void RendererSubSystem::addRenderer(const TypeIndex& inTypeIndex, Renderer* newRenderer)
	{
		if (!mRendererTable.contains(inTypeIndex))
		{
			mRendererTable.emplace(inTypeIndex, RendererListener{});
			mOrderQueue.push_back(inTypeIndex);
		}

		mRendererTable[inTypeIndex].addRenderer(newRenderer);
	}

	void RendererSubSystem::notifyChangeRenderType(const TypeIndex& inTypeIndex) noexcept
	{
		if (mRendererTable.contains(inTypeIndex))
		{
			mRendererTable[inTypeIndex].notifyChangeRenderType();
		}
	}
}
