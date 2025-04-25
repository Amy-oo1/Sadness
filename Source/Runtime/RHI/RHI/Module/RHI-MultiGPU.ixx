module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module RHI :MultiGPU;

import Platform;
import Utility;
import Container;

export class RHIGPUMask final {
	TRIVIAL_FUNCTION(RHIGPUMask)//NOTE :Bitset can not be constexpr
public:
	explicit RHIGPUMask(Uint32 mask) : m_Mask(mask) {}

	//I DO NOT Make a Array to Index GPUMask(Single) So
	[[nodiscard]] Uint32 Get_Index(void) const {
		ASSERT(this->Is_SingleIndex());

		return static_cast<Uint32>(CountRZero(this->m_Mask));
	}

	[[nodiscard]] Uint32 Get_Native(void)const {
		return this->m_Mask.to_ulong();
	}

	[[nodiscard]] static RHIGPUMask All(void) { return RHIGPUMask { 1 }; }

private:
	[[nodiscard]] bool Is_SingleIndex(void) const {
		return m_Mask.count();
	}

private:
	static constexpr Uint32 MaskCount { 32 };

	BitSet<MaskCount> m_Mask{ 1 };// Default Mase is 0x00000001

};