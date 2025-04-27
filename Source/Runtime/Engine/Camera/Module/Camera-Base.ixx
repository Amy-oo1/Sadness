module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Camera :Base;

import Platform;
import Utility;
import Math;

using namespace Math;

export template<typename Derived>
class CameraBase : public MoveAbleOnly {
protected:
	CameraBase(void) = default;

public:
	virtual ~CameraBase(void) = default;

public:
	void Update(void);

	auto Get_Rotation(void) const { return this->m_CameraToWorld.Get_Rotation(); }
	auto Get_RightVec(void)const { return this->m_Basis.Get_X(); }
	auto Get_UpVec(void)const { return this->m_Basis.Get_Y(); }
	auto Get_ForwardVec(void)const { return -this->m_Basis.Get_Z(); }
	auto Get_Position(void)const { return this->m_CameraToWorld.Get_Translation(); }

	auto Get_ViewMatrix(void)const { return this->m_ViewMatrix; };
	auto Get_ProjectionMatrix(void)const { return this->m_ProjectionMatrix; }
	auto Get_ViewProjectionMatrix(void)const { return this->m_ViewProjectionMatrix; }
	auto Get_PrevViewProjectionMatrix(void)const { return this->m_PrevViewProjectionMatrix; }
	auto Get_ViewSpaceFrustum(void)const { return this->m_FrustumVS; }
	auto Get_WorldSpaceFrustum(void)const { return this->m_FrustumWS; }

	void Set_LookDirection(Math::Vector3 forward, Math::Vector3 up) {
		auto ForwardLenSq { Math::LengthSquare(forward) };

		forward = Math::Select(forward * Math::RecipSqrt(ForwardLenSq), -Math::Vector3(Math::EZUnitVector::Z), ForwardLenSq < Math::Scalar { 1.e-5f });

		Vector3 Right { Cross(forward, up) };
		Scalar RightLenSq { LengthSquare(Right) };
		Right = Math::Select(Right * Math::RecipSqrt(RightLenSq), Quaternion { Math::Vector3(Math::EYUnitVector::Y), -g_2PI}*forward, RightLenSq < Math::Scalar { 1.e-5f });

		up = Cross(Right, forward);

		this->m_Basis = Math::Matrix3x3 { Right, up, -forward };
		this->m_CameraToWorld.Set_Rotation(Quaternion { this->m_Basis });
	}

	void Set_EyeAtUp(Math::Vector3 eye, Math::Vector3 at, Math::Vector3 up) {
		this->Set_LookDirection(at - eye, up);
		this->Set_Position(eye);
	}

	void Set_Rotation(Quaternion basisRotation) {
		this->m_CameraToWorld.Set_Rotation(Normalize(basisRotation));
		this->m_Basis = Math::Matrix3x3 { this->m_CameraToWorld.Get_Rotation() };
	}

	void Set_Position(Vector3 WordPos) {
		this->m_CameraToWorld.Set_Translation(WordPos);
	}

	void SetTransform(const AffineTransform& xform) {
		this->Set_LookDirection(-xform.Get_Z(), xform.Get_Y());
		this->Set_Position(xform.Get_Translation());
	}

protected:
	void Set_ProjectionMatrix(const Matrix4x4& proj) {
		this->m_ProjectionMatrix = proj;
	}



protected:
	Math::OrthogonalTransform m_CameraToWorld { Math::EIdentityTag::Identity };
	Math::Matrix3x3 m_Basis { Math::EIdentityTag::Identity };
	Math::Matrix4x4 m_ViewMatrix;
	Math::Matrix4x4 m_ProjectionMatrix;
	Math::Matrix4x4 m_ViewProjectionMatrix;

	Math::Matrix4x4 m_PrevViewProjectionMatrix;
	Math::Matrix4x4 m_ReprojectionMatrix;

	Math::Frustum m_FrustumVS;
	Math::Frustum m_FrustumWS;

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }


};



export template<typename Derived>
void CameraBase<Derived>::Update(void) {
	this->m_PrevViewProjectionMatrix = this->m_ProjectionMatrix;

	this->m_ViewMatrix = Matrix4x4 { ~this->m_CameraToWorld };
	this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
	this->m_ReprojectionMatrix = this->m_PrevViewProjectionMatrix * (Math::Invert(this->m_ViewProjectionMatrix));

	this->m_FrustumVS = Math::Frustum { this->m_ProjectionMatrix };
	this->m_FrustumWS = this->m_CameraToWorld * this->m_FrustumVS;

	if constexpr (requires(Derived & d) { d.Imp_Update(); })
		this->Get_Derived()->Imp_Update();
	else
		static_assert(false, " CameraBase<Derived>::Update() not implemented!"); //TODO : remove static_assert

}