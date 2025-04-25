module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Utility : Function;

import std;

//NOTE : Move To Math.ixx
//export template<typename Type>
//[[nodiscard]] constexpr const Type& Clamp(const Type& Value, const Type& Min, const Type& Max) {
//	return std::clamp(Value, Min, Max);
//}

//NOTE  : this Function in cpp23 ,so i write by myself
export template <typename Enum>
[[nodiscard]] FORCINLINE constexpr std::underlying_type_t<Enum> Tounderlying(Enum e) noexcept {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

export template<typename Type>
[[nodiscard]] FORCINLINE constexpr std::remove_reference_t<Type>&& MoveTemp(Type&& value) noexcept {
	return std::move(value);
}

export template<typename _Ty>
[[nodiscard]] FORCINLINE constexpr _Ty&& Forward(std::remove_reference_t<_Ty>& value) noexcept {
	return std::forward(value);
}

export template<typename _Ty>
[[nodiscard]] FORCINLINE constexpr _Ty&& Forward(std::remove_reference_t<_Ty>&& value) noexcept {
	return std::forward(value);
}

export template<typename _Ty>
[[nodiscard]] FORCINLINE constexpr _Ty* Addressof(_Ty& value) noexcept {
	return std::addressof(value);
}

export template<typename _Ty>
[[nodiscard]] FORCINLINE constexpr _Ty* Addressof(_Ty&& value) noexcept {
	return std::addressof(value);
}
