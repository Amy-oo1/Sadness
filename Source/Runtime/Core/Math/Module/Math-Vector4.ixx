module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Vector4;

import :Forward;
import :Common;
import :Scalar;
import :Vector2;
import :Vector3;

namespace Math {
	using namespace DirectX;
	
	export class Vector4 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Vector4)
	public:
		Vector4(float x, float y, float z, float w) : m_Vec{ XMVectorSet(x, y, z, w) } {}
		
		Vector4(XMFLOAT4 vec) : m_Vec{ XMLoadFloat4(&vec) } {}
		Vector4(XMFLOAT4A vec) : m_Vec{ XMLoadFloat4A(&vec) } {}
		
		explicit Vector4(FXMVECTOR vec) : m_Vec{ vec } {}
		
		explicit Vector4(Scalar s) : m_Vec{ s } {}
		explicit Vector4(Vector3 v) : m_Vec{ XMVectorSetW(v, 0.0f) } {}
		explicit Vector4(Vector3 xyz, float w) : m_Vec{ XMVectorSetW(xyz, w) } {}
		explicit Vector4(Vector3 xyz, Scalar W) : m_Vec{ XMVectorSetW(xyz, W) } {}

		explicit Vector4(EZeroTag) : m_Vec{ XMCreateZero() } {}
		explicit Vector4(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		explicit Vector4(EXUnitVector) : m_Vec{ CreateXUnitVertex() } {}
		explicit Vector4(EYUnitVector) : m_Vec{ CreateYUnitVertex() } {}
		explicit Vector4(EZUnitVector) : m_Vec{ CreateZUnitVertex() } {}
		explicit Vector4(EWUnitVector) : m_Vec{ CreateWUnitVertex() } {}
		
		operator XMVECTOR(void) const { return m_Vec; }

	public:
		[[nodiscard]] Vector4 operator+(void) { return *this; }
		[[nodiscard]] Vector4 operator-(void) { return Vector4(XMVectorNegate(this->m_Vec)); }

		[[nodiscard]] Vector4 operator+(Vector4 v) { return Vector4(XMVectorAdd(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator-(Vector4 v) { return Vector4(XMVectorSubtract(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator*(Vector4 v) { return Vector4(XMVectorMultiply(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator/(Vector4 v) { return Vector4(XMVectorDivide(this->m_Vec, v)); }

		[[nodiscard]] Vector4 operator*(Scalar s) { return *this * Vector4(s); }
		[[nodiscard]] Vector4 operator/(Scalar s) { return *this / Vector4(s); }

		[[nodiscard]] Vector4 operator*(float s) { return *this * Scalar(s); }
		[[nodiscard]] Vector4 operator/(float s) { return *this / Scalar(s); }

		friend [[nodiscard]] Vector4 operator*(Scalar v1, Vector4 v2) { return Vector4{ v1 } *v2; }
		friend [[nodiscard]] Vector4 operator/(Scalar v1, Vector4 v2) { return Vector4{ v1 } / v2; }
		
		friend [[nodiscard]] Vector4 operator*(float s1, Vector4 v2) { return Scalar{ s1 } *v2; }
		friend [[nodiscard]] Vector4 operator/(float s1, Vector4 v2) { return Scalar{ s1 } / v2; }

		Vector4& operator*=(Scalar v2) { return *this = *this * v2; }
		Vector4& operator/=(Scalar v2) { return *this = *this / v2; }

		Vector4& operator*=(float s2) { return *this = *this * s2; }
		Vector4& operator/=(float s2) { return *this = *this / s2; }

	public:
		[[nodiscard]] float GetX(void) const { return XMVectorGetX(m_Vec); }
		[[nodiscard]] float GetY(void) const { return XMVectorGetY(m_Vec); }
		[[nodiscard]] float GetZ(void) const { return XMVectorGetZ(m_Vec); }
		[[nodiscard]] float GetW(void) const { return XMVectorGetW(m_Vec); }
		
		void SetX(float x) { this->m_Vec = XMVectorSetX(m_Vec, x); }
		void SetY(float y) { this->m_Vec = XMVectorSetY(m_Vec, y); }
		void SetZ(float z) { this->m_Vec = XMVectorSetZ(m_Vec, z); }
		void SetW(float w) { this->m_Vec = XMVectorSetW(m_Vec, w); }

	private:
		XMVECTOR m_Vec;

	};

}