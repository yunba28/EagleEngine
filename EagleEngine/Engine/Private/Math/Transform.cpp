#include <Math/Transform.hpp>

#include <Core/WorldObject.hpp>

namespace eagle
{
	Transform Transform::Identity()
	{
		return Transform{ Vec3::Zero(),Quaternion::Identity(),Vec3::One() };
	}

	Vec3 Transform::WorldToLocalPosition(const WorldObject* const inWorldObject, const Vec3& inWorldPos)
	{
		Vec3 result = inWorldPos;
		auto owner = inWorldObject->getOwner();
		while (owner)
		{
			result = result * owner->getLocalRotation().conjugated();
			result = result - owner->getLocalPosition();
			owner = owner->getOwner();
		}
		return result;
	}

	Vec3 Transform::LocalToWorldPosition(const WorldObject* const inWorldObject, const Vec3& inLocalPos)
	{
		Vec3 result = inLocalPos;
		auto owner = inWorldObject->getOwner();
		while (owner)
		{
			result = result * owner->getLocalRotation();
			result = result + owner->getLocalPosition();
			owner = owner->getOwner();
		}
		return result;
	}

	Quaternion Transform::WorldToLocalRotation(const WorldObject* const inWorldObject, const Quaternion& inWorldRot)
	{
		Quaternion result = inWorldRot;
		auto owner = inWorldObject->getOwner();
		while (owner)
		{
			result *= owner->getLocalRotation().conjugated();
			owner = owner->getOwner();
		}
		return result;
	}

	Quaternion Transform::LocalToWorldRotation(const WorldObject* const inWorldObject, const Quaternion& inLocalRot)
	{
		Quaternion result = inLocalRot;
		auto owner = inWorldObject->getOwner();
		while (owner)
		{
			result *= owner->getLocalRotation();
			owner = owner->getOwner();
		}
		return result;
	}

}
