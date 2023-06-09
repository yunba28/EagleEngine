﻿#pragma once

#include <concepts>

#include <CoreFwd.hpp>

namespace EagleEngine::Concept
{
	template<class Int>
	concept Integral = std::is_integral_v<Int>;

	template<class Float>
	concept FloatingPoint = std::is_floating_point_v<Float>;

	template<class ArithmeticType>
	concept Arithmetic = Integral<ArithmeticType> || FloatingPoint<ArithmeticType>;

	template<class InheritedType>
	concept IsObject = std::is_base_of_v<Object, InheritedType>;

	template<class InheritedType>
	concept IsActor = std::is_base_of_v<Actor, InheritedType>;

	template<class InheritedType>
	concept IsComponent = std::is_base_of_v<Component, InheritedType>;
}
