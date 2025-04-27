module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Vector3;

import :Forward;
import :Common;
import :Scalar;
import :Vector2;

namespace Math {

	using namespace DirectX;

	export class Vector3 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Vector3)
	public:
		Vector3(float x, float y, float z) : m_Vec{ XMVectorSet(x, y, z, 0.0f) } {}
		
		Vector3(XMFLOAT3 vec) : m_Vec{ XMLoadFloat3(&vec) } {}
		Vector3(XMFLOAT3A vec) : m_Vec{ XMLoadFloat3A(&vec) } {}
		
		explicit Vector3(FXMVECTOR vec) : m_Vec{ vec } {}
		
		explicit Vector3(Scalar s) : m_Vec{ s } {}
		explicit Vector3(Vector4 v);
		
		explicit Vector3(EZeroTag) : m_Vec{ XMCreateZero() } {}
		explicit Vector3(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		explicit Vector3(EXUnitVector) : m_Vec{ CreateXUnitVertex() } {}
		explicit Vector3(EYUnitVector) : m_Vec{ CreateYUnitVertex() } {}
		explicit Vector3(EZUnitVector) : m_Vec{ CreateZUnitVertex() } {}
		
		operator XMVECTOR(void) const { return m_Vec; }

	public:
		[[nodiscard]] Vector3 operator+(void) const { return *this; }
		[[nodiscard]] Vector3 operator-(void) const { return Vector3(XMVectorNegate(this->m_Vec)); }
		
		[[nodiscard]] Vector3 operator+(Vector3 v) const { return Vector3(XMVectorAdd(this->m_Vec, v)); }
		[[nodiscard]] Vector3 operator-(Vector3 v) const { return Vector3(XMVectorSubtract(this->m_Vec, v)); }
		[[nodiscard]] Vector3 operator*(Vector3 v) const { return Vector3(XMVectorMultiply(this->m_Vec, v)); }
		[[nodiscard]] Vector3 operator/(Vector3 v) const { return Vector3(XMVectorDivide(this->m_Vec, v)); }
		
		[[nodiscard]] Vector3 operator*(Scalar s) const { return *this * Vector3(s); }
		[[nodiscard]] Vector3 operator/(Scalar s) const { return *this / Vector3(s); }
		
		[[nodiscard]] Vector3 operator*(float s) const { return *this * Scalar(s); }
		[[nodiscard]] Vector3 operator/(float s) const { return *this / Scalar(s); }

		friend [[nodiscard]] Vector3 operator*(Scalar v1, Vector3 v2) { return Vector3{ v1 } *v2; }
		friend [[nodiscard]] Vector3 operator/(Scalar v1, Vector3 v2) { return Vector3(v1) / v2; }

		friend [[nodiscard]] Vector3 operator*(float v1, Vector3 v2) { return Scalar{ v1 } *v2; }
		friend [[nodiscard]] Vector3 operator/(float v1, Vector3 v2) { return Scalar{ v1 } / v2; }

		Vector3& operator+=(Vector3 v) { *this = *this + v; return *this; }
		Vector3& operator-=(Vector3 v) { *this = *this - v; return *this; }
		Vector3& operator*=(Vector3 v) { *this = *this * v; return *this; }
		Vector3& operator/=(Vector3 v) { *this = *this / v; return *this; }

	public:
		[[nodiscard]] Scalar Get_X(void) const { return Scalar(XMVectorGetX(m_Vec)); }
		[[nodiscard]] Scalar Get_Y(void) const { return Scalar(XMVectorGetY(m_Vec)); }
		[[nodiscard]] Scalar Get_Z(void) const { return Scalar(XMVectorGetZ(m_Vec)); }
		
		void Set_X(Scalar s) { m_Vec = XMVectorSetX(m_Vec, s); }
		void Set_Y(Scalar s) { m_Vec = XMVectorSetY(m_Vec, s); }
		void Set_Z(Scalar s) { m_Vec = XMVectorSetZ(m_Vec, s); }
	
	private:
		XMVECTOR m_Vec;

	};
}