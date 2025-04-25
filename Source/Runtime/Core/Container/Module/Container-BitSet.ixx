module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Container :BitSet;

import std;

import Platform;

export template<typename Size _Size>
using BitSet = std::bitset<_Size>;

export [[nodiscard]] constexpr FORCINLINE Uint32 CountRZero(const BitSet<32>& InBitSet) {
	return std::countr_zero(InBitSet.to_ulong());
}

export [[nodiscard]] constexpr FORCINLINE Uint32 CountRZero(Uint32 Bits) {
	return std::countr_zero(Bits);
}


export using std::operator&;
export using std::operator|;
export using std::operator^;
