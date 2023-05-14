#pragma once

#include <Render/Renderer.hpp>

namespace s3d
{
	class Camera2D;
}

namespace eagle
{
	class CameraManager;

	class Camera2DComponent : public Renderer
	{
	private:

		using MovableCamera = s3d::Camera2D;

	public:

		Camera2DComponent();
		~Camera2DComponent();

	private:

		virtual void update(double inDeltaTime)override;
		virtual void draw()const override;

	public:

		virtual void _internalConstruct()override;

	private:

		std::shared_ptr<MovableCamera> mMovableCamera2D;

		ObjectRef<CameraManager> mCameraManager;

	};
}
