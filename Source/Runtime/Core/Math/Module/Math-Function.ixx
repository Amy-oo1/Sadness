module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Math:Function;

import std;

namespace Math {

	export template <class _Ty>
		constexpr const  _Ty& Max(const _Ty& left, const _Ty& right) {
		return std::max(left, right);
	}

	export template <class _Ty, class _Pr>
		constexpr const _Ty& Max(const _Ty& left, const _Ty& right, _Pr& pred) {
		return std::max(left, right, pred);
	}

	export template <class _Ty>
		constexpr _Ty Max(std::initializer_list<_Ty> list) {
		return std::max(list);
	}

	export template <class _Ty, class _Pr>
		constexpr _Ty Max(std::initializer_list<_Ty> list, _Pr pr) {
		return std::max(list, pr);
	}

	export template <class _Ty>
		constexpr const _Ty& Min(const _Ty& left, const _Ty& right) {
		return std::min(left, right);
	}

	export template <class _Ty, class _Pr>
		constexpr const _Ty& Min(const _Ty& left, const _Ty& right, _Pr& pred) {
		return std::min(left, right, pred);
	}

	export template <class _Ty>
		constexpr _Ty Min(std::initializer_list<_Ty> list) {
		return std::min(list);
	}

	export template <class _Ty, class _Pr>
		constexpr _Ty Min(std::initializer_list<_Ty> list, _Pr pr) {
		return std::min(list, pr);
	}

}

