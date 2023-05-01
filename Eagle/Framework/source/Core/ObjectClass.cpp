#include <Core/ObjectClass.hpp>
#include <Core/Object.hpp>

#include <Siv3D/Char.hpp>

namespace EagleEngine
{
	Object* ObjectClass::createObject() const
	{
		return m_factory();
	}

	String ObjectClass::getClassName() const
	{
		return Unicode::FromUTF8(m_typeInfo->name()).removed(U"class ");
	}
}
