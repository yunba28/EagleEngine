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

		Transform(const Vec3& newPosition)
			: mPosition(newPosition)
			, mRotation(Quaternion::Identity())
			, mScale(Vec3::One())
		{}

		Transform(const Vec3& newPosition, const Quaternion& newRotation)
			: mPosition(newPosition)
			, mRotation(newRotation)
			, mScale(Vec3::One())
		{}

		Transform(const Vec3& newPosition, const Quaternion& newRotation, const Vec3& newScale)
			: mPosition(newPosition)
			, mRotation(newRotation)
			, mScale(newScale)
		{}

		Transform(const Vec3& newPosition, const Vec3& newEuler, const Vec3& newScale)
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

		void setPositionAndRotation(const Vec3& newPosition, const Quaternion& newRotation)noexcept
		{
			mPosition = newPosition;
			mRotation = newRotation;
		}

		void setPositionAndRotation(const Vec3& newPosition, const Vec3& newEuler)noexcept
		{
			mPosition = newPosition;
			mRotation = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
		}

		void setPosition(const Vec3& newPosition)noexcept
		{
			mPosition = newPosition;
		}

		const Vec3& getPosition()const noexcept
		{
			return mPosition;
		}

		void setRotation(const Quaternion& newRotation)noexcept
		{
			mRotation = newRotation;
		}

		void setRotation(const Vec3& newEuler)noexcept
		{
			mRotation = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
		}

		const Quaternion& getRotation()const noexcept
		{
			return mRotation;
		}

		void setScale(const Vec3& newScale)noexcept
		{
			mScale = newScale;
		}

		void setScale(double newScale)noexcept
		{
			mScale = Vec3{ newScale,newScale,newScale };
		}

		const Vec3& getScale()const noexcept
		{
			return mScale;
		}

		void addPosition(const Vec3& inVec)noexcept
		{
			mPosition += inVec;
		}

		void addRotation(const Quaternion& inQuat)noexcept
		{
			mRotation *= inQuat;
		}

		void addRotation(const Vec3& inEuler)noexcept
		{
			mRotation *= Quaternion::RollPitchYaw(inEuler.x, inEuler.y, inEuler.z);
		}

	public:

		static Transform Identity();
		static Vec3 GetLocalPosition(const WorldObject* inWorldObject, const Vec3& inWorldPos);
		static Vec3 GetWorldPosition(const WorldObject* inWorldObject, const Vec3& inLocalPos);
		static Quaternion GetLocalRotation(const WorldObject* inWorldObject, const Quaternion& inWorldRot);
		static Quaternion GetWorldRotation(const WorldObject* inWorldObject, const Quaternion& inLocalRot);

	private:

		Vec3 mPosition = Vec3::Zero();
		Quaternion mRotation = Quaternion::Identity();
		Vec3 mScale = Vec3::One();

	};
}
