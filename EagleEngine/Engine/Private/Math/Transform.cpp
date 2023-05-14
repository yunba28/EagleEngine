#include <Math/Transform.hpp>

namespace eagle
{
	Transform Transform::Identity()
	{
		return Transform{ Vec3::Zero(),Quaternion::Identity(),Vec3::One() };
	}

	Vec3 Transform::GetLocalPosition(WorldObject* inWorldObject)
	{

		return Vec3();
	}

	Vec3 Transform::GetWorldPosition(WorldObject* inWorldObject)
	{
		return Vec3();
	}

	Quaternion Transform::GetLocalRotation(WorldObject* inWorldObject)
	{
		return Quaternion();
	}

	Quaternion Transform::GetWorldRotation(WorldObject* inWorldObject)
	{
		return Quaternion();
	}
}
