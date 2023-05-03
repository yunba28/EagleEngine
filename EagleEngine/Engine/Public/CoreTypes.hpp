#pragma once

#include <cstdint>
#include <tuple>
#include <initializer_list>

namespace eagle
{
	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;

	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using uintptr = std::uintptr_t;

	template<class First, class Second>
	using Pair = std::pair<First, Second>;

	template<class ...Args>
	using Tuple = std::tuple<Args...>;

	template<class Type>
	using InitializerList = std::initializer_list<Type>;

	inline uintptr MakeHandle(const void* inPtr)
	{
		return reinterpret_cast<uintptr>(inPtr);
	}
}
