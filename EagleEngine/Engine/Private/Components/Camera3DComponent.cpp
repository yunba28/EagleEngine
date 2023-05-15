#include <Components/Camera3DComponent.hpp>

#include <Core/RendererSubSystem.hpp>
#include <GameFramework/LevelBase.hpp>

#include <Siv3D/DebugCamera3D.hpp>

using namespace s3d;

namespace eagle
{
	Camera3DComponent::Camera3DComponent()
		: mCamera3D()
	{
		setUpdateEnable(false);
	}

	Camera3DComponent::~Camera3DComponent()
	{
	}

	void Camera3DComponent::update(double inDeltaTime)
	{
		mCamera3D->update(inDeltaTime);
	}

	void Camera3DComponent::_internalConstruct()
	{
		if (auto level = getLevel(); level)
		{
			if (auto subsystem = level->getRendererSubSystem(); subsystem)
			{
				mCamera3D = std::static_pointer_cast<DebugCamera3D>(subsystem->getBasicCamera3D());
			}
		}

		WorldObject::_internalConstruct();
	}
}
