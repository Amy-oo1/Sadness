module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Container:Optional;

import std;

export template<typename Type>
using Optional = std::optional<Type>;

export std::nullopt_t NullOpt{ std::nullopt };

export template<class _Ty>
[[nodiscard]] FORCINLINE constexpr	Optional<std::decay_t<_Ty>> MakeOptional(_Ty&& value){
	return std::make_optional(std::forward<_Ty>(value));
}

export template<class Type, typename... Arges>
[[nodiscard]] FORCINLINE constexpr Optional<Type> MakeOptional(Arges&&... args) {
	return std::make_optional(std::forward<Arges>(args)...);
}

export template<typename _Ty, typename _Elem, typename... _Arges>
[[nodiscard]] FORCINLINE constexpr Optional<_Ty> MakeOptional(std::initializer_list<_Elem> _Ilist, _Arges&&... _Args) {
	return std::make_optional(_Ilist, std::forward<_Arges>(_Args)...);
}


export using std::operator==;