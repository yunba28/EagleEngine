#include <Components/Camera2DComponent.hpp>

#include <Core/RendererSubSystem.hpp>
#include <GameFramework/Level.hpp>

#include <Siv3D/Camera2D.hpp>

using namespace s3d;

namespace eagle
{
	Camera2DComponent::Camera2DComponent()
		: mCamera2D()
	{
		setUpdateEnable(false);
		setDrawEnable(false);
	}

	Camera2DComponent::~Camera2DComponent()
	{
	}

	void Camera2DComponent::update(double inDeltaTime)
	{
		mCamera2D->update(inDeltaTime);
	}

	void Camera2DComponent::draw() const
	{
		mCamera2D->draw();
	}

	void Camera2DComponent::_internalConstruct()
	{
		if (auto level = getLevel(); level)
		{
			_internalRegisterRenderSubSystem(level->getRendererSubSystem());
		}

		if (auto subsystem = getRendererSubSystem(); subsystem)
		{
			mCamera2D = std::static_pointer_cast<Camera2D>(subsystem->getBasicCamera2D());
			mCamera2D->setParameters(Camera2DParameters::MouseOnly());
		}

		WorldObject::_internalConstruct();
	}

	void Camera2DComponent::setCenter(const Vec2& newCenter) noexcept
	{
		mCamera2D->setCenter(newCenter);
	}

	const Vec2& Camera2DComponent::getCenter() const noexcept
	{
		return mCamera2D->getCenter();
	}

	void Camera2DComponent::setScale(double newScale) noexcept
	{
		mCamera2D->setScale(newScale);
	}

	double Camera2DComponent::getScale() const noexcept
	{
		return mCamera2D->getScale();
	}

	void Camera2DComponent::jumpTo(const Vec2& newCenter, double newScale) noexcept
	{
		mCamera2D->jumpTo(newCenter, newScale);
	}

	void Camera2DComponent::setCameraParameters(const Camera2DParameters& newParameters)
	{
		mCamera2D->setParameters(newParameters);
	}

	const Camera2DParameters& Camera2DComponent::getcameraParameters() const noexcept
	{
		return mCamera2D->getParameters();
	}
}
