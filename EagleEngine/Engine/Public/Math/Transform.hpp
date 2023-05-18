#pragma once

#include <CoreFwd.hpp>
#include <CoreMath.hpp>

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
			, mScale(Vec3::One())
			, mRotation(Quaternion::Identity())
		{}

		Transform(const Vec3& newPosition, const Quaternion& newRotation)
			: mPosition(newPosition)
			, mScale(Vec3::One())
			, mRotation(newRotation)
		{}

		Transform(const Vec3& newPosition, const Quaternion& newRotation, const Vec3& newScale)
			: mPosition(newPosition)
			, mScale(newScale)
			, mRotation(newRotation)
		{}

		Transform(const Vec3& newPosition, const Vec3& newEuler, const Vec3& newScale)
			: mPosition(newPosition)
			, mScale(newScale)
			, mRotation(Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z))
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

		void addScale(const Vec3& inVec)noexcept
		{
			mScale += inVec;
		}

	public:

		static Transform Identity();
		static Vec3 WorldToLocalPosition(WorldObject* inWorldObject, const Vec3& inWorldPos);
		static Vec3 LocalToWorldPosition(WorldObject* inWorldObject, const Vec3& inLocalPos);
		static Quaternion WorldToLocalRotation(WorldObject* inWorldObject, const Quaternion& inWorldRot);
		static Quaternion LocalToWorldRotation(WorldObject* inWorldObject, const Quaternion& inLocalRot);

	private:

		Vec3 mPosition = Vec3::Zero();
		Vec3 mScale = Vec3::One();
		Quaternion mRotation = Quaternion::Identity();

	};

}
