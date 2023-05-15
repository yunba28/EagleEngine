#include <Math/Transform.hpp>

#include <GameFramework/Actor.hpp>

namespace eagle
{
	Transform Transform::Identity()
	{
		return Transform{ Vec3::Zero(),Quaternion::Identity(),Vec3::One() };
	}

	Vec3 Transform::GetLocalPosition(ITransformable* inTransform, const Vec3& inWorldPosition)
	{
		Vec3 localPos = inWorldPosition;
		auto owner = inTransform->getParent();
		while (owner)
		{
			const Transform& transform = inTransform->getTransform();;
			localPos -= transform.getPosition();
			owner = owner->getParent();
		}
		return localPos;
	}

	Vec3 Transform::GetWorldPosition(ITransformable* inTransform, const Vec3& inLocalPosition)
	{
		Vec3 worldPos = inLocalPosition;
		auto owner = inTransform->getParent();
		while (owner)
		{
			const Transform& transform = inTransform->getTransform();;
			worldPos += transform.getPosition();
			owner = owner->getParent();
		}
		return worldPos;
	}

	Quaternion Transform::GetLocalRotation(ITransformable* inTransform, const Quaternion& inWorldRotation)
	{
		Quaternion localRot = inWorldRotation;
		auto owner = inTransform->getParent();
		while (owner)
		{
			const Transform& transform = inTransform->getTransform();
			localRot *= transform.getRotation().conjugated();
			owner = owner->getParent();
		}
		return localRot;
	}

	Quaternion Transform::GetWorldRotation(ITransformable* inTransform, const Quaternion& inLocalRotation)
	{
		Quaternion worldRot = inLocalRotation;
		auto owner = inTransform->getParent();
		while (owner)
		{
			const Transform& transform = inTransform->getTransform();
			worldRot *= transform.getRotation();
			owner = owner->getParent();
		}
		return worldRot;
	}

}
