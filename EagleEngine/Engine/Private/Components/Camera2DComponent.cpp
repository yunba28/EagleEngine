#include <Components/Camera2DComponent.hpp>

#include <Core/RendererSubSystem.hpp>
#include <GameFramework/LevelBase.hpp>

#include <Siv3D/Camera2D.hpp>

using namespace s3d;

namespace eagle
{
	Camera2DComponent::Camera2DComponent()
		: mMovableCamera2D()
		, mCameraManager(nullptr)
	{
		setUpdateEnable(false);
		setDrawEnable(false);
	}

	Camera2DComponent::~Camera2DComponent()
	{
	}

	void Camera2DComponent::update(double inDeltaTime)
	{
		mMovableCamera2D->update(inDeltaTime);
	}

	void Camera2DComponent::draw() const
	{
		mMovableCamera2D->draw();
	}

	void Camera2DComponent::_internalConstruct()
	{
		if (auto level = getLevel(); level)
		{
			_internalRegisterRenderSubSystem(level->getRendererSubSystem());
		}

		if (auto subsystem = getRendererSubSystem(); subsystem)
		{
			mMovableCamera2D = std::static_pointer_cast<MovableCamera>(subsystem->getBasicCamera2D());
			mMovableCamera2D->setParameters(Camera2DParameters::MouseOnly());
		}

		WorldObject::_internalConstruct();
	}
}
