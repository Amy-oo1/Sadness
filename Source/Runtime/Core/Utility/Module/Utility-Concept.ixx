export module Utility : Concept;

import std;

export template<typename Enum>
using UnderlyingtType = std::underlying_type_t<Enum>;

export template<typename Type>
concept Integral = std::integral<Type>;

export template<typename Type>
concept UnsignedIntegtal = std::unsigned_integral<Type>;

export template<typename Type>
concept SignedIntegral = std::signed_integral<Type>;

export template<typename Type>
concept FloatingPoint = std::floating_point<Type>;

export template<typename Type>
concept TotallyOrder = std::totally_ordered<Type>;

export template<typename Type>
concept Trivial = requires(Type t) {
	{ std::is_trivially_constructible_v<Type> };
	{ std::is_trivially_destructible_v<Type> };
	{ std::is_trivially_copyable_v<Type> };
};

export template<typename Type>
concept StandardLayout = requires{
	{std::is_standard_layout_v<Type>};
};

export template<typename Type>
concept POD = Trivial<Type> && StandardLayout<Type>;