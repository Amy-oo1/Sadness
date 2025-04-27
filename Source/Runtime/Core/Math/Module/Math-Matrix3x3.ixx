module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Matrix3x3;

import :Forward;
import :Common;
//TODO : import More ?
import :Vector3;

namespace Math {
	using namespace DirectX;

	export  class alignas(16) Matrix3x3 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Matrix3x3)
	public:
		constexpr Matrix3x3(Vector3 x, Vector3 y, Vector3 z) : m_Mat{ x,y,z } {}

		Matrix3x3(Quaternion q);

		explicit Matrix3x3(FXMMATRIX mat) : m_Mat{ Vector3{mat.r[0]}, Vector3{mat.r[1]}, Vector3{mat.r[2]} } {}

		Matrix3x3(EZeroTag) : m_Mat{ Vector3{EZeroTag::Zero}, Vector3{EZeroTag::Zero}, Vector3{EZeroTag::Zero} } {}
		Matrix3x3(EIdentityTag) : m_Mat{ Vector3{EXUnitVector::X}, Vector3{EYUnitVector::Y}, Vector3{EZUnitVector::Z} } {}

		operator XMMATRIX(void) const { return XMMATRIX { this->m_Mat[0], this->m_Mat[1], this->m_Mat[2], XMVectorZero() }; }


	public:
		Matrix3x3 operator*(Scalar s)const { return Matrix3x3 { this->m_Mat[0] * s, this->m_Mat[1] * s, this->m_Mat[2] * s }; }
		Vector3 operator*(Vector3 vec) const { return Vector3 { XMVector3TransformNormal(vec, *this) }; }
		Matrix3x3 operator*(Matrix3x3 mat)const { return Matrix3x3 { *this * mat.Get_X(), *this * mat.Get_Y(), *this * mat.Get_Z() }; }


	public:
		[[nodiscard]] Vector3 Get_X(void)const { return this->m_Mat[0]; }
		[[nodiscard]] Vector3 Get_Y(void)const { return this->m_Mat[1]; }
		[[nodiscard]] Vector3 Get_Z(void)const { return this->m_Mat[2]; }


		void Set_X(Vector3 x) { this->m_Mat[0] = x; }
		void Set_Y(Vector3 y) { this->m_Mat[1] = y; }
		void Set_Z(Vector3 z) { this->m_Mat[2] = z; }


	public:
		static Matrix3x3 MakeXRotation(float angle) { return Matrix3x3 { XMMatrixRotationX(angle) }; }
		static Matrix3x3 MakeYRotation(float angle) { return Matrix3x3 { XMMatrixRotationY(angle) }; }
		static Matrix3x3 MakeZRotation(float angle) { return Matrix3x3 { XMMatrixRotationZ(angle) }; }
		static Matrix3x3 MakeScale(float scale) { return Matrix3x3 { XMMatrixScaling(scale, scale, scale) }; }
		static Matrix3x3 MakeScale(float sx, float sy, float sz) { return Matrix3x3 { XMMatrixScaling(sx, sy, sz) }; }
		static Matrix3x3 MakeScale(const XMFLOAT3& scale) { return Matrix3x3 { XMMatrixScaling(scale.x, scale.y, scale.z) }; }
		static Matrix3x3 MakeScale(Vector3 scale) { return Matrix3x3 { XMMatrixScalingFromVector(scale) }; }

		
	private:
		Vector3 m_Mat[3];
	};
}