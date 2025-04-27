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

		Vector4(EZeroTag) : m_Vec{ XMCreateZero() } {}
		Vector4(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		Vector4(EXUnitVector) : m_Vec{ CreateXUnitVertex() } {}
		Vector4(EYUnitVector) : m_Vec{ CreateYUnitVertex() } {}
		Vector4(EZUnitVector) : m_Vec{ CreateZUnitVertex() } {}
		explicit Vector4(EWUnitVector) : m_Vec{ CreateWUnitVertex() } {}//TODO :here cobine a explicit ,
		
		operator XMVECTOR(void) const { return m_Vec; }

	public:
		[[nodiscard]] Vector4 operator+(void) const { return *this; }
		[[nodiscard]] Vector4 operator-(void) const { return Vector4(XMVectorNegate(this->m_Vec)); }

		[[nodiscard]] Vector4 operator+(Vector4 v) const { return Vector4(XMVectorAdd(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator-(Vector4 v) const { return Vector4(XMVectorSubtract(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator*(Vector4 v) const { return Vector4(XMVectorMultiply(this->m_Vec, v)); }
		[[nodiscard]] Vector4 operator/(Vector4 v) const { return Vector4(XMVectorDivide(this->m_Vec, v)); }

		[[nodiscard]] Vector4 operator*(Scalar s) const { return *this * Vector4(s); }
		[[nodiscard]] Vector4 operator/(Scalar s) const { return *this / Vector4(s); }

		[[nodiscard]] Vector4 operator*(float s) const { return *this * Scalar(s); }
		[[nodiscard]] Vector4 operator/(float s) const { return *this / Scalar(s); }

		friend [[nodiscard]] Vector4 operator*(Scalar v1, Vector4 v2) { return Vector4{ v1 } *v2; }
		friend [[nodiscard]] Vector4 operator/(Scalar v1, Vector4 v2) { return Vector4{ v1 } / v2; }
		
		friend [[nodiscard]] Vector4 operator*(float s1, Vector4 v2) { return Scalar{ s1 } *v2; }
		friend [[nodiscard]] Vector4 operator/(float s1, Vector4 v2) { return Scalar{ s1 } / v2; }

		Vector4& operator*=(Scalar v2) { return *this = *this * v2; }
		Vector4& operator/=(Scalar v2) { return *this = *this / v2; }

		Vector4& operator*=(float s2) { return *this = *this * s2; }
		Vector4& operator/=(float s2) { return *this = *this / s2; }

	public:
		[[nodiscard]] Scalar Get_X(void) const { return XMVectorGetX(m_Vec); }
		[[nodiscard]] Scalar Get_Y(void) const { return XMVectorGetY(m_Vec); }
		[[nodiscard]] Scalar Get_Z(void) const { return XMVectorGetZ(m_Vec); }
		[[nodiscard]] Scalar GetW(void) const { return XMVectorGetW(m_Vec); }
		
		void Set_X(float x) { this->m_Vec = XMVectorSetX(m_Vec, x); }
		void Set_Y(float y) { this->m_Vec = XMVectorSetY(m_Vec, y); }
		void Set_Z(float z) { this->m_Vec = XMVectorSetZ(m_Vec, z); }
		void SetW(float w) { this->m_Vec = XMVectorSetW(m_Vec, w); }

	private:
		XMVECTOR m_Vec;

	};

}