module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Camera :Common;

import Platform;
import Utility;
import Math;

import :Base;

using namespace Math;

export class Camera final : public CameraBase<Camera> {
	friend class CameraBase<Camera>;
public:
	Camera(void) :
		m_ReverseZ { true },
		m_InfiniteZ { false } {

		this->SetPerspectiveMatrix(
			g_PIDIV4,
			9.f/16.f,
			1.0f,
			1000.0f
		);
	}

public:
	float GetFOV() const { return this->m_VerticalFOV; }
	float GetNearClip() const { return this->m_NearClip; }
	float GetFarClip() const { return this->m_FarClip; }
	float GetClearDepth() const { return this->m_ReverseZ ? 0.0f : 1.0f; }

	void SetPerspectiveMatrix(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip)
	{
		this->m_VerticalFOV = verticalFovRadians;
		this->m_AspectRatio = aspectHeightOverWidth;
		this->m_NearClip = nearZClip;
		this->m_FarClip = farZClip;

		this->UpdateProjMatrix();

		this->m_PrevViewProjectionMatrix = this->m_ProjectionMatrix;
	}

	void SetFOV(float verticalFovInRadians) { this->m_VerticalFOV = verticalFovInRadians; this->UpdateProjMatrix(); }
	void SetAspectRatio(float heightOverWidth) { this->m_AspectRatio = heightOverWidth; this->UpdateProjMatrix(); }
	void SetZRange(float nearZ, float farZ) { m_NearClip = nearZ; m_FarClip = farZ; this->UpdateProjMatrix(); }
	void ReverseZ(bool enable) { this->m_ReverseZ = enable; this->UpdateProjMatrix(); }



private:
	void UpdateProjMatrix(void);

private:
	void Imp_Update(void) {}


private:
	float m_VerticalFOV;
	float m_AspectRatio;
	float m_NearClip;
	float m_FarClip;
	bool m_ReverseZ;
	bool m_InfiniteZ;

};

void Camera::UpdateProjMatrix(void) {
	float Y { 1.0f / Tanf(this->m_VerticalFOV * 0.5f) };
	float X { Y * this->m_AspectRatio };

	float Q1, Q2;

	// ReverseZ puts far plane at Z=0 and near plane at Z=1.  This is never a bad idea, and it's
	// actually a great idea with F32 depth buffers to redistribute precision more evenly across
	// the entire range.  It requires clearing Z to 0.0f and using a GREATER variant depth test.
	// Some care must also be done to properly reconstruct linear W in a pixel shader from hyperbolic Z.
	if (this->m_ReverseZ) {
		if (this->m_InfiniteZ) {
			Q1 = 0.0f;
			Q2 = this->m_NearClip;
		}
		else {
			Q1 = this->m_NearClip / (this->m_FarClip - this->m_NearClip);
			Q2 = Q1 * this->m_FarClip;
		}
	}
	else {
		if (this->m_InfiniteZ) {
			Q1 = -1.0f;
			Q2 = -this->m_NearClip;
		}
		else {
			Q1 = this->m_FarClip / (this->m_NearClip - this->m_FarClip);
			Q2 = Q1 * this->m_NearClip;
		}
	}

	this->Set_ProjectionMatrix(Matrix4x4 {
			Vector4{X, 0.0f, 0.0f, 0.0f},
			Vector4{0.0f, Y, 0.0f, 0.0f},
			Vector4{0.0f, 0.0f, Q1, -1.0f},
			Vector4{0.0f, 0.0f, Q2, 0.0f}
		}
	);
}


