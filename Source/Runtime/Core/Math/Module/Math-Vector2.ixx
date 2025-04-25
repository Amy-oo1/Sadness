module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Vector2;

import :Forward;
import :Common;
import :Scalar;

namespace Math {

	using namespace DirectX;

	export class Vector2 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Vector2)

	public:
		Vector2(float x, float y) : m_Vec{ XMVectorSet(x, y, 0.0f, 0.0f) } {}

		Vector2(XMFLOAT2 vec) : m_Vec{ XMLoadFloat2(&vec) } {}
		Vector2(XMFLOAT2A vec) : m_Vec{ XMLoadFloat2A(&vec) } {}

		explicit Vector2(FXMVECTOR vec) : m_Vec{ vec } {}

		explicit Vector2(Scalar s) : m_Vec{ s } {}
		explicit Vector2(Vector3 v);

		explicit Vector2(EZeroTag) : m_Vec{ XMCreateZero() } {}
		explicit Vector2(EIdentityTag) : m_Vec{ XMCreateOne() } {}
		explicit Vector2(EXUnitVector) : m_Vec{ CreateXUnitVertex() } {}
		explicit Vector2(EYUnitVector) : m_Vec{ CreateYUnitVertex() } {}

		operator XMVECTOR(void) const { return m_Vec; }

	public:
		[[nodiscard]] Vector2 operator+(void) { return *this; }
		[[nodiscard]] Vector2 operator-(void) { return Vector2(XMVectorNegate(this->m_Vec)); }

		[[nodiscard]] Vector2 operator+(Vector2 v) { return Vector2(XMVectorAdd(this->m_Vec, v)); }
		[[nodiscard]] Vector2 operator-(Vector2 v) { return Vector2(XMVectorSubtract(this->m_Vec, v)); }
		[[nodiscard]] Vector2 operator*(Vector2 v) { return Vector2(XMVectorMultiply(this->m_Vec, v)); }
		[[nodiscard]] Vector2 operator/(Vector2 v) { return Vector2(XMVectorDivide(this->m_Vec, v)); }

		[[nodiscard]] Vector2 operator*(Scalar s) { return *this * Vector2(s); }
		[[nodiscard]] Vector2 operator/(Scalar s) { return *this / Vector2(s); }

		[[nodiscard]] Vector2 operator*(float s) { return *this * Scalar(s); }
		[[nodiscard]] Vector2 operator/(float s) { return *this / Scalar(s); }

		friend [[nodiscard]] Vector2 operator*(Scalar v1, Vector2 v2) { return Vector2{ v1 } *v2; }
		friend [[nodiscard]] Vector2 operator/(Scalar v1, Vector2 v2) { return Vector2{v1 } / v2;
	}
		friend [[nodiscard]] Vector2 operator*(float s1, Vector2 v2) { return Scalar{ s1 } *v2; }
		friend [[nodiscard]] Vector2 operator/(float s1, Vector2 v2) { return Scalar{s1 } / v2;
	}

		Vector2& operator+=(Vector2 v) { *this = *this + v; return *this; }
		Vector2& operator-=(Vector2 v) { *this = *this - v; return *this; }
		Vector2& operator*=(Vector2 v) { *this = *this * v; return *this; }
		Vector2& operator/=(Vector2 v) { *this = *this / v; return *this; }

	public:
		[[nodiscard]] Scalar Get_X(void) const { return Scalar(XMVectorGetX(m_Vec)); }
		[[nodiscard]] Scalar Get_Y(void) const { return Scalar(XMVectorGetY(m_Vec)); }

		void Set_X(Scalar s) { this->m_Vec = XMVectorSetX(m_Vec, s); }
		void Set_Y(Scalar s) { this->m_Vec = XMVectorSetY(m_Vec, s); }

	private:
		XMVECTOR m_Vec;
	};

}