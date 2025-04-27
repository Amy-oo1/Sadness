module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Matrix4x4;

import :Forward;
import :Common;
//TODO : import More ?
import :Vector3;
import :Vector4;
import :Matrix3x3;

namespace Math {

	using namespace DirectX;

	export class alignas(16) Matrix4x4 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Matrix4x4)
	public:

		//Remove
		Matrix4x4(const float* data) : m_Mat{ XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(data)) } {}

		constexpr Matrix4x4(Vector4 x, Vector4 y, Vector4 z, Vector4 w) : m_Mat{ x,y,z,w } {}
		Matrix4x4(Vector3 x, Vector3 y, Vector3 z) :m_Mat{ Vector4{x,0},Vector4{y,0},Vector4{z,0},Vector4{EWUnitVector::W} } {}

		explicit Matrix4x4(Matrix3x3 mat3x3) : m_Mat{ mat3x3.Get_X(), mat3x3.Get_Y(), mat3x3.Get_Z(), Vector4{EWUnitVector::W} } {}
		Matrix4x4(Matrix3x3 mat3x3,Vector3 w) : m_Mat{ mat3x3.Get_X(), mat3x3.Get_Y(), mat3x3.Get_Z(), Vector4{w,0} } {}

		explicit Matrix4x4(FXMMATRIX mat) : m_Mat{mat} {}
		explicit Matrix4x4(EZeroTag) : m_Mat { Vector4{ EZeroTag::Zero }, Vector4{ EZeroTag::Zero }, Vector4{ EZeroTag::Zero }, Vector4{ EZeroTag::Zero } } {}
		explicit Matrix4x4(EIdentityTag) : m_Mat{ XMMatrixIdentity() } {}
	
		operator XMMATRIX(void) const { return m_Mat; }

		operator Matrix3x3(void)const { return Matrix3x3 { this->m_Mat }; }

	public:
		[[nodiscard]] Matrix4x4 operator*(Matrix4x4 rhs) const {
			return Matrix4x4{ XMMatrixMultiply(this->m_Mat, rhs.m_Mat) };
		}

		[[nodiscard]] Vector3 operator*(Vector3 vec) const {
			return Vector3{ XMVector3Transform(vec,this->m_Mat) };
		}

		[[nodiscard]] Vector4 operator*(Vector4 vec) const {
			return Vector4{ XMVector4Transform(vec,this->m_Mat) };
		}
		
	public:
		[[nodiscard]] Vector4 Get_X(void)const { return Vector4{ this->m_Mat.r[0] }; }
		[[nodiscard]] Vector4 Get_Y(void)const { return Vector4{ this->m_Mat.r[1] }; }
		[[nodiscard]] Vector4 Get_Z(void)const { return Vector4{ this->m_Mat.r[2] }; }
		[[nodiscard]] Vector4 Get_W(void)const { return Vector4{ this->m_Mat.r[3] }; }

		void Set_X(Vector4 x) { this->m_Mat.r[0] = x; }
		void Set_Y(Vector4 y) { this->m_Mat.r[1] = y; }
		void Set_Z(Vector4 z) { this->m_Mat.r[2] = z; }
		void Set_W(Vector4 w) { this->m_Mat.r[3] = w; }

	public:


	private:
		XMMATRIX m_Mat;

	};

}