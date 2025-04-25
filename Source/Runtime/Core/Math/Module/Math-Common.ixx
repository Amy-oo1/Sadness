module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Platform/Include/PlatformMacros.h"
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Common;

import Platform;
import Utility;

import :Forward;

namespace Math {

	//NOTE : Alignment
	export template<TotallyOrder Type>
	[[nodiscard]] constexpr Type Clamp(Type Value, Type Min, Type Max) {
		if (Value < Min)
			return Min;
		else if (Value > Max)
			return Max;
		return Value;
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr Type AlignUpWithMask(Type Value, Size Mask) {
		return static_cast<Type>((static_cast<Size>(Value) + Mask) & ~Mask);
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr Type AlignDownWithMask(Type Value, Size Mask) {
		return static_cast<Type>(static_cast<Size>(Value) & ~Mask);
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr Type AlignUp(Type Value, Size Alignment) {
		ASSERT(0==Alignment % 2);

		return AlignUpWithMask(Value, Alignment - 1);
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr Type AlignDown(Type Value, Size Alignment) {
		ASSERT(0 == Alignment % 2);

		return AlignDownWithMask(Value, Alignment - 1);
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr bool IsAligned(Type Value, Size Alignment) {
		ASSERT(0 == Alignment % 2);

		return (static_cast<Size>(Value) & (Alignment - 1)) == 0;
	};

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr bool IsPowerOfTwo(Type Value) {
		ASSERT(Value > 0);

		return 0==(Value & (Value - 1));
	}

	export template<UnsignedIntegtal Type> FORCINLINE
	[[nodiscard]] constexpr Type DivideByMultiple(Type Value, Size Alignment) {
		ASSERT(0 == Alignment % 2);

		return static_cast<Type>((static_cast<Size>(Value) + Alignment - 1) / Alignment);
	}


	export template<typename Type> FORCINLINE
	[[nodiscard]] constexpr bool IsDivisible(Type Value, Type Divisor) {
		ASSERT(0 != Divisor);

		return (Value / Divisor) * Divisor == Value;
	}


	//TODO Remoce Windows lib,Use cpp and constexpr
	using namespace DirectX;

	export [[nodiscard]] FORCINLINE XMVECTOR XMCreateZero(void){
        return XMVectorZero();
    }

#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)

	export [[nodiscard]] FORCINLINE XMVECTOR XMCreateOne(FXMVECTOR Zero= XMCreateZero()) {
		const auto  AllBits{ _mm_castps_si128(_mm_cmpeq_ps(Zero, Zero)) };
		return _mm_castsi128_ps(_mm_slli_epi32(_mm_srli_epi32(AllBits, 25), 23));	// return 
	}

	export [[nodiscard]] FORCINLINE XMVECTOR CreateXUnitVertex(FXMVECTOR One = XMCreateOne()) {
		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(One), 12));
	}

	export [[nodiscard]] FORCINLINE XMVECTOR CreateYUnitVertex(FXMVECTOR One = XMCreateOne()) {
		const auto unitx{ CreateXUnitVertex(One) };
		return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(unitx), 4));
	}

	export [[nodiscard]] FORCINLINE XMVECTOR CreateZUnitVertex(FXMVECTOR One = XMCreateOne()) {
		const auto unitx{ CreateXUnitVertex(One) };
		return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(unitx), 8));
	}

	export [[nodiscard]] FORCINLINE XMVECTOR CreateWUnitVertex(FXMVECTOR One = XMCreateOne()) {
		const auto unitx{ CreateXUnitVertex(One) };
		return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(unitx), 12));
	}

	export [[nodiscard]] FORCINLINE XMVECTOR SetWToZero(FXMVECTOR Vec) {
		const auto MaskOffW{ _mm_srli_si128(_mm_castps_si128(_mm_cmpeq_ps(Vec, Vec)), 4) };
		return _mm_and_ps(Vec, _mm_castsi128_ps(MaskOffW));
	}

	export [[nodiscard]] FORCINLINE XMVECTOR SetWToOne(FXMVECTOR Vec) {
		return _mm_movelh_ps(Vec, _mm_unpackhi_ps(Vec, XMCreateOne()));
	}

#else
	//TODO : Add Other Platform

#endif
	enum class EZeroTag :Uint32{ Zero, Origin };
	enum class EIdentityTag :Uint32 { One, Identity };
	enum class EXUnitVector :Uint32 { X };
	enum class EYUnitVector :Uint32 { Y };
	enum class EZUnitVector :Uint32 { Z };
	enum class EWUnitVector :Uint32 { W };

	export using enum EZeroTag;
	export using enum EIdentityTag;
	export using enum EXUnitVector;
	export using enum EYUnitVector;
	export using enum EZUnitVector;
	export using enum EWUnitVector;
}

//TODO : Remove FORCEINLINE ?