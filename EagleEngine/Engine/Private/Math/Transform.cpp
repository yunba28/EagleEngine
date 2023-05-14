#include <Math/Transform.hpp>

#include <GameFramework/Actor.hpp>

namespace eagle
{
	Transform Transform::Identity()
	{
		return Transform{ Vec3::Zero(),Quaternion::Identity(),Vec3::One() };
	}

	Vec3 Transform::GetLocalPosition(const WorldObject* inWorldObject, const Vec3& inWorldPos)
	{
		Vec3 localPos = inWorldPos;
		auto owner = inWorldObject->getOwner();
		while (owner)
		{
			localPos -= inWorldObject->getLocalPosition();
			owner = inWorldObject->getOwner();
		}
		return localPos;
	}

	Vec3 Transform::GetWorldPosition(const WorldObject* inWorldObject, const Vec3& inLocalPos)
	{
		return Vec3();
	}

	Quaternion Transform::GetLocalRotation(const WorldObject* inWorldObject, const Quaternion& inWorldRot)
	{
		return Quaternion();
	}

	Quaternion Transform::GetWorldRotation(const WorldObject* inWorldObject, const Quaternion& inLocalRot)
	{
		return Quaternion();
	}
}
