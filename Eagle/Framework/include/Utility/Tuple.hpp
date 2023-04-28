#pragma once

#include <tuple>

namespace EagleEngine
{
	template<class ...Types>
	using Tuple = std::tuple<Types...>;

	template<class First, class Second>
	using Pair = std::pair<First, Second>;
}
