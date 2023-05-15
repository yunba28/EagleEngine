#pragma once

#include <Render/Renderer.hpp>

#include <Siv3D/Camera2DParameters.hpp>

namespace s3d
{
	class Camera2D;
}

namespace eagle
{
	class Camera2DComponent : public Renderer
	{
	public:

		Camera2DComponent();
		~Camera2DComponent();

	private:

		virtual void update(double inDeltaTime)override;
		virtual void draw()const override;

	public:

		virtual void _internalConstruct()override;

	public:

		void setCenter(const Vec2& newCenter)noexcept;
		const Vec2& getCenter()const noexcept;
		void setScale(double newScale)noexcept;
		double getScale()const noexcept;

		void jumpTo(const Vec2& newCenter, double newScale)noexcept;

		void setCameraParameters(const Camera2DParameters& newParameters);
		const Camera2DParameters& getcameraParameters()const noexcept;

	private:

		std::shared_ptr<s3d::Camera2D> mCamera2D;

	};
}
