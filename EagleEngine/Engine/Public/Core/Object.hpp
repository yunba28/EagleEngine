#pragma once

#include <CoreUtility.hpp>
#include <Core/Name.hpp>
#include <Core/ObjectPtr.hpp>

namespace eagle
{
	class Object : Uncopyable
	{
	public:

		explicit Object();
		virtual ~Object() = 0;

	public:

	public:

		bool invalid()const noexcept
		{
			return !IsValidByObject(this);
		}

	private:



	};
}
