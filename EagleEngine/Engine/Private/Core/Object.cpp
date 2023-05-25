#include <Core/Object.hpp>

#include <Siv3D/Char.hpp>

#include <GameFramework/Actor.hpp>
#include <GameFramework/Level.hpp>

namespace eagle
{
	Object::Object()
		: mName(NoneName)
		, mTypeIndex(typeid(void))
		, mInherited(ObjectInherited::None)
	{
	}

	Object::~Object()
	{
	}

	void Object::_internalConstruct()
	{
		ensure(awake(), "Failed to execute the awake function");
	}

	void Object::_internalDestruct()
	{
		ensure(dispose(), "Failed to execute the dispose function");
		Name::Release(mName);
	}

	String Object::typeName() const
	{
		std::string name = type().base().name();
		return Unicode::FromUTF8(name).removed(U"class ");
	}
}
