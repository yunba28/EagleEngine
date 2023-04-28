#pragma once

#include <array>

namespace EagleEngine
{
	template<class Type, size_t Size>
	using FixedArray = std::array<Type, Size>;
}
