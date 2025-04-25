module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Scalar;

import :Forward;
import :Common;

namespace Math {

	using namespace DirectX;

	export class Scalar final {
		CONSTEXPR_TRIVIAL_FUNCTION(Scalar)
	public:

		explicit Scalar(float Value) : m_Vec{ XMVectorReplicate(Value) } {}

		//NOTE : This is not a good idea
		//explicit Scalar(double Value) : m_Vec{ XMVectorReplicate(static_cast<float>(Value)) } {}
		
		explicit Scalar(EZeroTag) : m_Vec{ XMCreateZero() } {}
		explicit Scalar(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		
		explicit Scalar(FXMVECTOR Vec) : m_Vec{ Vec } {}

		operator XMVECTOR(void) const { return m_Vec; }
		operator float(void) const { return XMVectorGetX(m_Vec); }

		[[nodiscard]] Scalar operator+ (void) { return *this; }
		[[nodiscard]] Scalar operator- (void) { return Scalar(XMVectorNegate(this->m_Vec)); }

		[[nodiscard]] Scalar operator+ (Scalar s) { return Scalar(XMVectorAdd(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator- (Scalar s) { return Scalar(XMVectorSubtract(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator* (Scalar s) { return Scalar(XMVectorMultiply(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator/ (Scalar s) { return Scalar(XMVectorDivide(this->m_Vec, s)); }

		[[nodiscard]] Scalar operator+ (float s) { return *this + Scalar(s); }
		[[nodiscard]] Scalar operator- (float s) { return *this - Scalar(s); }
		[[nodiscard]] Scalar operator* (float s) { return *this * Scalar(s); }
		[[nodiscard]] Scalar operator/ (float s) { return *this / Scalar(s); }

		friend [[nodiscard]] Scalar operator+ (float s1, Scalar s2) { return Scalar{ s1 } + s2; }
		friend [[nodiscard]] Scalar operator- (float s1, Scalar s2) { return Scalar{ s1 } - s2; }
		friend [[nodiscard]] Scalar operator* (float s1, Scalar s2) { return Scalar{ s1 } * s2; }
		friend [[nodiscard]] Scalar operator/ (float s1, Scalar s2) { return Scalar{ s1 } / s2; }

	private:
		XMVECTOR m_Vec;
	};
}