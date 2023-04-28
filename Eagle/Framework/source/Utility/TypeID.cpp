#include <Utility/TypeID.hpp>
#include <Siv3D/Char.hpp>

namespace EagleEngine
{
	using namespace s3d;

	String TypeID::name() const
	{
		return Unicode::FromUTF8(mTypeInfo->name());
	}

}
