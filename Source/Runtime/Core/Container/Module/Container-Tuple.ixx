export module Container :Tuple;

import std;

export template <class _Ty1, class _Ty2>
using Pair = std::pair<_Ty1, _Ty2>;

export template <class _This, class... _Rest>
using Tuple = std::tuple<_This, _Rest...>;

export template <class _Ty1, class _Ty2>
[[nodiscard]] constexpr std::pair<std::_Unrefwrap_t<_Ty1>, std::_Unrefwrap_t<_Ty2>> MakePair(_Ty1&& _Arg1, _Ty2&& _Arg2) {
	return std::make_pair(std::forward<_Ty1>(_Arg1), std::forward<_Ty2>(_Arg2));
}

export template <class... _Types>
[[nodiscard]] constexpr auto MakeTuple(_Types&&... _Args) {
	return std::make_tuple(std::forward<_Types>(_Args)...);
}

export template <class... _Types>
[[nodiscard]] constexpr auto Tie(_Types&... _Args) {
	return std::tie(_Args...);
}