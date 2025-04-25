module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Quaternion;

import :Forward;
import :Common;
import :Scalar;
import :Vector3;
//TODO : import More ?
import :Matrix3x3;

namespace Math {

	using namespace DirectX;

	export class alignas(16) Quaternion final {
		CONSTEXPR_TRIVIAL_FUNCTION(Quaternion)
	public:
		Quaternion(FXMVECTOR vec) : m_Quat{ vec } {}
		Quaternion(FXMMATRIX mat) : m_Quat{ XMQuaternionRotationMatrix(mat) } {}

		Quaternion(Vector3 Axis, Scalar Angle) : m_Quat{ XMQuaternionRotationAxis(Axis, Angle) } {}
		Quaternion(float patch, float yaw, float roll) : m_Quat{ XMQuaternionRotationRollPitchYaw(patch, yaw, roll) } {}

		Quaternion(EIdentityTag) : m_Quat{ XMQuaternionIdentity() } {}

	public:
		[[nodiscard]] Quaternion operator+(void) { return *this; }
		[[nodiscard]] Quaternion operator-(void) { return Quaternion{ XMQuaternionInverse(this->m_Quat) }; }
		
		[[nodiscard]] Quaternion operator*(Quaternion rhi) const {return Quaternion{ XMQuaternionMultiply(rhi.m_Quat,this->m_Quat) };}//NOTE :  // Returns the product Q2*Q1 (which is the concatenation of a rotation Q1 followed by the rotation Q2)
		[[nodiscard]] Vector3 operator*(Vector3 v) const {return Vector3{ XMVector3Rotate(v, this->m_Quat) };}

		Quaternion& operator*=(Quaternion rhi) { *this=*this * rhi; return *this; }
		
	public:
		Quaternion Conjugate(void) const {return Quaternion{ XMQuaternionConjugate(this->m_Quat) };}

	private:
		XMVECTOR m_Quat{ XMQuaternionIdentity() };
	};
}