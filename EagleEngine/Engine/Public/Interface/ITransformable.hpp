#pragma once

#include <Core/ObjectPtr.hpp>

namespace eagle
{
	class Transform;
	class Actor;

	class ITransformable
	{
	public:
		virtual const Transform& getTransform()const = 0;
		virtual ObjectRef<Actor> getParent()const = 0;
	};
}
