module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

export module CameraController :Orbit;

import Platform;
import Utility;
import Math;
import Logger;

import Camera;

import Input;

import :Base;

using namespace Math;
using namespace Input;

using namespace DirectX;

export class OrbitCameraController final :public CameraControllerBase<OrbitCameraController> {
	friend class CameraControllerBase<OrbitCameraController>;
public:
	OrbitCameraController(Camera& targetCamera, BoundingSphere focus, Vector3 upVec = Vector3 {EYUnitVector::Y}):
        CameraControllerBase<OrbitCameraController> { targetCamera },
        m_ModelBounds { focus },
        m_WorldUp { Normalize(upVec) }{

        m_JoystickSensitivityX = 2.0f;
        m_JoystickSensitivityY = 2.0f;

        m_MouseSensitivityX = 1.0f;
        m_MouseSensitivityY = 1.0f;

        m_CurrentPitch = 0.0f;
        m_CurrentHeading = 0.0f;
        m_CurrentCloseness = 0.5f;

        m_Momentum = true;

        m_LastYaw = 0.0f;
        m_LastPitch = 0.0f;

	}

	~OrbitCameraController(void) = default;


public:
    void EnableMomentum(bool enable) { m_Momentum = enable; }


private:
    void Imp_Update(float dt);

    OrbitCameraController& operator=(const OrbitCameraController&) { return *this; }

private:
    Math::BoundingSphere m_ModelBounds;
    Math::Vector3 m_WorldUp;

    float m_JoystickSensitivityX;
    float m_JoystickSensitivityY;

    float m_MouseSensitivityX;
    float m_MouseSensitivityY;

    float m_CurrentHeading;
    float m_CurrentPitch;
    float m_CurrentCloseness;

    bool m_Momentum;

    float m_LastYaw;
    float m_LastPitch;
    float m_LastForward;
};

void OrbitCameraController::Imp_Update(float deltaTime){
    constexpr float timeScale = 1.f;

    float yaw = GetTimeCorrectedAnalogInput(kAnalogLeftStickX) * timeScale * m_JoystickSensitivityX;
    float pitch = GetTimeCorrectedAnalogInput(kAnalogLeftStickY) * timeScale * m_JoystickSensitivityY;
    float closeness = GetTimeCorrectedAnalogInput(kAnalogRightStickY) * timeScale;

    if (m_Momentum)
    {
        ApplyMomentum(m_LastYaw, yaw, deltaTime);
        ApplyMomentum(m_LastPitch, pitch, deltaTime);
    }

    // don't apply momentum to mouse inputs
    yaw += GetAnalogInput(kAnalogMouseX) * m_MouseSensitivityX;
    pitch += GetAnalogInput(kAnalogMouseY) * m_MouseSensitivityY;
    closeness += GetAnalogInput(kAnalogMouseScroll) * 0.1f;

    m_CurrentPitch += pitch;
    m_CurrentPitch = XMMin(XM_PIDIV2, m_CurrentPitch);
    m_CurrentPitch = XMMax(-XM_PIDIV2, m_CurrentPitch);

    m_CurrentHeading -= yaw;
    if (m_CurrentHeading > XM_PI)
        m_CurrentHeading -= XM_2PI;
    else if (m_CurrentHeading <= -XM_PI)
        m_CurrentHeading += XM_2PI;

    m_CurrentCloseness += closeness;
    m_CurrentCloseness = Clamp(m_CurrentCloseness, 0.0f, 1.0f);

    Matrix3x3 orientation = Matrix3x3::MakeYRotation(m_CurrentHeading) * Matrix3x3::MakeXRotation(m_CurrentPitch);
    Vector3 position = orientation.Get_Z() * (m_ModelBounds.Get_Radius() * Lerp(3.0f, 1.0f, m_CurrentCloseness) + m_TargetCamera.GetNearClip());
    m_TargetCamera.SetTransform(AffineTransform(orientation, position + m_ModelBounds.Get_Center()));
    m_TargetCamera.Update();
}
