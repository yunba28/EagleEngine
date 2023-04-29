#pragma once

#include <Math/Quaternion.hpp>
#include <Math/Vector3.hpp>

namespace EagleEngine
{
	class Transform
	{
	public:

		Transform()noexcept = default;

		Transform(const Vector3& _pos)noexcept :mPosition(_pos) {}

		Transform(const Vector3& _pos, const Quaternion& _rot)noexcept :mPosition(_pos), mRotation(_rot) {}

		Transform(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scl)noexcept :mPosition(_pos), mRotation(_rot), mScale(_scl) {}

		Transform(const Transform&)noexcept = default;

		Transform& operator=(const Transform&)noexcept = default;

		Transform(Transform&&)noexcept = default;

		Transform& operator=(Transform&&)noexcept = default;

	public:

		void set(const Transform& _other)noexcept;

		void set(Transform&& _other)noexcept;

		void setPosition(const Vector3& _position)noexcept;

		Vector3 getPosition()const noexcept;

		void setRotation(const Quaternion& _rotation)noexcept;

		Quaternion getRotation()const noexcept;

		void setScale(const Vector3& _scale)noexcept;

		Vector3 getScale()const noexcept;

		void setPositionAndRotation(const Vector3& _position, const Quaternion& _rotation)noexcept;

		Vector3 getForward()const;

		Vector3 getRight()const;

		Vector3 getUp()const;

		void translation(const Vector3& _velocity)noexcept;

		void translation(double _x, double _y, double _z)noexcept;

		void rotate(const Quaternion& _rotation);

		void rotate(double _x, double _y, double _z);

		void scaling(const Vector3& _scale);

		void scaling(double _x, double _y, double _z);

	private:

		Vector3 mPosition{ 0,0,0 };

		Quaternion mRotation{ 0,0,0,1 };

		Vector3 mScale{ 1,1,1 };

	};

	inline void Transform::set(const Transform& _other)noexcept
	{
		mPosition = _other.mPosition;
		mRotation = _other.mRotation;
		mScale = _other.mScale;
	}

	inline void Transform::set(Transform&& _other)noexcept
	{
		mPosition = std::move(_other.mPosition);
		mRotation = std::move(_other.mRotation);
		mScale = std::move(_other.mScale);
	}

	inline void Transform::setPosition(const Vector3& _position)noexcept
	{
		mPosition = _position;
	}

	inline Vector3 Transform::getPosition()const noexcept
	{
		return mPosition;
	}

	inline void Transform::setRotation(const Quaternion& _rotation)noexcept
	{
		mRotation = _rotation;
	}

	inline Quaternion Transform::getRotation()const noexcept
	{
		return mRotation;
	}

	inline void Transform::setScale(const Vector3& _scale)noexcept
	{
		mScale = _scale;
	}

	inline Vector3 Transform::getScale()const noexcept
	{
		return mScale;
	}

	inline void Transform::setPositionAndRotation(const Vector3& _position, const Quaternion& _rotation)noexcept
	{
		mPosition = _position;
		mRotation = _rotation;
	}

	inline Vector3 Transform::getForward()const
	{
		return mRotation * Float3::Forward();
	}

	inline Vector3 Transform::getRight()const
	{
		return mRotation * Float3::Right();
	}

	inline Vector3 Transform::getUp()const
	{
		return mRotation * Float3::Up();
	}

	inline void Transform::translation(const Vector3& _translation)noexcept
	{
		mPosition += _translation;
	}

	inline void Transform::translation(double _x, double _y, double _z)noexcept
	{
		mPosition.moveBy(_x, _y, _z);
	}

	inline void Transform::rotate(const Quaternion& _rotation)
	{
		mRotation *= _rotation;
	}

	inline void Transform::rotate(double _x, double _y, double _z)
	{
		mRotation *= Quaternion::RollPitchYaw(_x, _y, _z);
	}

	inline void Transform::scaling(const Vector3& _scale)
	{
		mScale *= _scale;
	}

	inline void Transform::scaling(double _x, double _y, double _z)
	{
		mScale *= Vector3(_x, _y, _z);
	}

}
