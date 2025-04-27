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

		Scalar(float Value) : m_Vec{ XMVectorReplicate(Value) } {}

		//NOTE : This is not a good idea
		//explicit Scalar(double Value) : m_Vec{ XMVectorReplicate(static_cast<float>(Value)) } {}
		
		explicit Scalar(EZeroTag) : m_Vec{ XMCreateZero() } {}
		explicit Scalar(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		
		explicit Scalar(FXMVECTOR Vec) : m_Vec{ Vec } {}

		operator XMVECTOR(void) const { return m_Vec; }
		operator float(void) const { return XMVectorGetX(m_Vec); }

		[[nodiscard]] Scalar operator+ (void) const { return *this; }
		[[nodiscard]] Scalar operator- (void) const { return Scalar(XMVectorNegate(this->m_Vec)); }

		[[nodiscard]] Scalar operator+ (Scalar s) const { return Scalar(XMVectorAdd(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator- (Scalar s) const { return Scalar(XMVectorSubtract(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator* (Scalar s) const { return Scalar(XMVectorMultiply(this->m_Vec, s)); }
		[[nodiscard]] Scalar operator/ (Scalar s) const { return Scalar(XMVectorDivide(this->m_Vec, s)); }

		[[nodiscard]] Scalar operator+ (float s) const { return *this + Scalar(s); }
		[[nodiscard]] Scalar operator- (float s) const { return *this - Scalar(s); }
		[[nodiscard]] Scalar operator* (float s) const { return *this * Scalar(s); }
		[[nodiscard]] Scalar operator/ (float s) const { return *this / Scalar(s); }

		[[nodiscard]] auto operator<(float s) const { return XMVectorGetX(*this) < s; }
		[[nodiscard]] auto operator>(float s) const { return XMVectorGetX(*this) > s; }
		[[nodiscard]] auto operator<=(float s) const { return XMVectorGetX(*this) <= s; }
		[[nodiscard]] auto operator>=(float s) const { return XMVectorGetX(*this) >= s; }
		[[nodiscard]] bool operator== (float s) const { return XMVectorGetX(this->m_Vec) == s; }
		[[nodiscard]] bool operator!= (float s) const { return XMVectorGetX(this->m_Vec) != s; }

		friend [[nodiscard]] Scalar operator+ (float s1, Scalar s2) { return Scalar{ s1 } + s2; }
		friend [[nodiscard]] Scalar operator- (float s1, Scalar s2) { return Scalar{ s1 } - s2; }
		friend [[nodiscard]] Scalar operator* (float s1, Scalar s2) { return Scalar{ s1 } * s2; }
		friend [[nodiscard]] Scalar operator/ (float s1, Scalar s2) { return Scalar{ s1 } / s2; }

		friend [[nodiscard]] auto operator< (float s1, Scalar s2) { return s1 < XMVectorGetX(s2); }
		friend [[nodiscard]] auto operator> (float s1, Scalar s2) { return s1 > XMVectorGetX(s2); }
		friend [[nodiscard]] auto operator<= (float s1, Scalar s2) { return s1 <= XMVectorGetX(s2); }
		friend [[nodiscard]] auto operator>= (float s1, Scalar s2) { return s1 >= XMVectorGetX(s2); }
		friend [[nodiscard]] bool operator== (float s1, Scalar s2) { return s1 == XMVectorGetX(s2); }
		friend [[nodiscard]] bool operator!= (float s1, Scalar s2) { return s1 != XMVectorGetX(s2); }

	private:
		XMVECTOR m_Vec;
	};
}