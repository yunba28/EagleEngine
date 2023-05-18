#include <Math/Transform.hpp>

#include <Core/WorldObject.hpp>

namespace eagle
{
	Transform Transform::Identity()
	{
		return Transform{ Vec3::Zero(),Quaternion::Identity(),Vec3::One() };
	}

	Vec3 Transform::WorldToLocalPosition(WorldObject* inWorldObject, const Vec3& inWorldPos)
	{
		return Vec3();
	}

	Vec3 Transform::LocalToWorldPosition(WorldObject* inWorldObject, const Vec3& inLocalPos)
	{
		return Vec3();
	}

	Quaternion Transform::WorldToLocalRotation(WorldObject* inWorldObject, const Quaternion& inWorldRot)
	{
		return Quaternion();
	}

	Quaternion Transform::LocalToWorldRotation(WorldObject* inWorldObject, const Quaternion& inLocalRot)
	{
		return Quaternion();
	}

}
