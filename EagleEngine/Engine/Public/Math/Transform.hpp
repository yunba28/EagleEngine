#pragma once

#include <CoreFwd.hpp>
#include <CoreCommons.hpp>

namespace eagle
{
	class Transform
	{
	public:

		Transform() = default;
		~Transform() = default;

		Transform(Transform&&) = default;
		Transform(const Transform&) = default;
		Transform& operator=(Transform&&) = default;
		Transform& operator=(const Transform&) = default;

		Transform(Vec3 newPosition)
			: mPosition(newPosition)
			, mRotation(Quaternion::Identity())
			, mScale(Vec3::One())
		{}

		Transform(Vec3 newPosition, Quaternion newRotation)
			: mPosition(newPosition)
			, mRotation(newRotation)
			, mScale(Vec3::One())
		{}

		Transform(Vec3 newPosition, Quaternion newRotation, Vec3 newScale)
			: mPosition(newPosition)
			, mRotation(newRotation)
			, mScale(newScale)
		{}

		Transform(Vec3 newPosition, Vec3 newEuler, Vec3 newScale)
			: mPosition(newPosition)
			, mRotation(Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z))
			, mScale(newScale)
		{}

	public:

		void set(const Transform& newTransform)noexcept
		{
			mPosition = newTransform.mPosition;
			mRotation = newTransform.mRotation;
			mScale = newTransform.mScale;
		}

		void setPositionAndRotation(Vec3 newPosition, Quaternion newRotation)noexcept
		{
			mPosition = newPosition;
			mRotation = newRotation;
		}

		void setPositionAndRotation(Vec3 newPosition, Vec3 newEuler)noexcept
		{
			mPosition = newPosition;
			mRotation = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
		}

		void setPosition(Vec3 newPosition)noexcept
		{
			mPosition = newPosition;
		}

		Vec3 getPosition()const noexcept
		{
			return mPosition;
		}

		void setRotation(Quaternion newRotation)noexcept
		{
			mRotation = newRotation;
		}

		void setRotation(Vec3 newEuler)noexcept
		{
			mRotation = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
		}

		Quaternion getRotation()const noexcept
		{
			return mRotation;
		}

		void setScale(Vec3 newScale)noexcept
		{
			mScale = newScale;
		}

		void setScale(double newScale)noexcept
		{
			mScale = Vec3{ newScale,newScale,newScale };
		}

		Vec3 getScale()const noexcept
		{
			return mScale;
		}

		void addPosition(Vec3 inVec)noexcept
		{
			mPosition += inVec;
		}

		void addRotation(Quaternion inQuat)noexcept
		{
			mRotation *= inQuat;
		}

		void addRotation(Vec3 inEuler)noexcept
		{
			mRotation *= Quaternion::RollPitchYaw(inEuler.x, inEuler.y, inEuler.z);
		}

	public:

		static Transform Identity();
		static Vec3 GetLocalPosition(WorldObject* inWorldObject);
		static Vec3 GetWorldPosition(WorldObject* inWorldObject);
		static Quaternion GetLocalRotation(WorldObject* inWorldObject);
		static Quaternion GetWorldRotation(WorldObject* inWorldObject);

	private:

		Vec3 mPosition = Vec3::Zero();
		Quaternion mRotation = Quaternion::Identity();
		Vec3 mScale = Vec3::One();

	};
}
