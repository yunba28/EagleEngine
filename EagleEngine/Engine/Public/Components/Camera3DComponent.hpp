#pragma once

#include <GameFramework/Component.hpp>

namespace s3d
{
	class DebugCamera3D;
}

namespace eagle
{
	class Camera3DComponent : public Component
	{
	public:

		Camera3DComponent();
		~Camera3DComponent();

	private:

		virtual void update(double inDeltaTime)override;

	public:

		virtual void _internalConstruct()override;

	private:

		std::shared_ptr<s3d::DebugCamera3D> mCamera3D;

	};
}
