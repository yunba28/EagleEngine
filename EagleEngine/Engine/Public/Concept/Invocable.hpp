#pragma once

#include <concepts>

namespace eagle::Concept
{
	template<class FunctionType, class ReturnType, class ...Args>
	concept Invocable = std::is_invocable_r_v<ReturnType, FunctionType, Args...>;

	template<class FunctionType,class ...Args>
	concept Predicate = std::predicate<FunctionType, Args...>;
}
