#pragma once

#include <array>

namespace EagleEngine
{
	template<class Type, size_t ArraySize>
	using FixedArray = std::array<Type, ArraySize>;
}
