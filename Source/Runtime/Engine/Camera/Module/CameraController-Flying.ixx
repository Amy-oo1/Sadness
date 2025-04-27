module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

export module CameraController :Flying;

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


export class FlyingCameraController final :public CameraControllerBase<FlyingCameraController> {
	friend class CameraControllerBase<FlyingCameraController>;

public:
	FlyingCameraController(Camera& targetCamera)
		:CameraControllerBase<FlyingCameraController> { targetCamera } {
	}
	~FlyingCameraController(void) = default;

public:

    void SlowMovement(bool enable) { m_FineMovement = enable; }
    void SlowRotation(bool enable) { m_FineRotation = enable; }

    void SetHeadingPitchAndPosition(float heading, float pitch, const Vector3& position);

private:
    void Imp_Update(float dt);



private:
    Vector3 m_WorldUp;
    Vector3 m_WorldNorth;
    Vector3 m_WorldEast;
    float m_HorizontalLookSensitivity;
    float m_VerticalLookSensitivity;
    float m_MoveSpeed;
    float m_StrafeSpeed;
    float m_MouseSensitivityX;
    float m_MouseSensitivityY;

    float m_CurrentHeading;
    float m_CurrentPitch;

    bool m_FineMovement;
    bool m_FineRotation;
    bool m_Momentum;

    float m_LastYaw;
    float m_LastPitch;
    float m_LastForward;
    float m_LastStrafe;
    float m_LastAscent;

};

void FlyingCameraController::SetHeadingPitchAndPosition(float heading, float pitch, const Vector3& position)
{
    m_CurrentHeading = heading;
    if (m_CurrentHeading > XM_PI)
        m_CurrentHeading -= XM_2PI;
    else if (m_CurrentHeading <= -XM_PI)
        m_CurrentHeading += XM_2PI;

    m_CurrentPitch = pitch;
    m_CurrentPitch = XMMin(XM_PIDIV2, m_CurrentPitch);
    m_CurrentPitch = XMMax(-XM_PIDIV2, m_CurrentPitch);

    Matrix3x3 orientation =
        Matrix3x3(m_WorldEast, m_WorldUp, -m_WorldNorth) *
        Matrix3x3::MakeYRotation(m_CurrentHeading) *
        Matrix3x3::MakeXRotation(m_CurrentPitch);

    m_TargetCamera.SetTransform(AffineTransform(orientation, position));
    m_TargetCamera.Update();
}

void FlyingCameraController::Imp_Update(float deltaTime){

    constexpr float timeScale = 1.f;

    using enum DigitalInput;

    if (IsFirstPressed(kLThumbClick) || IsFirstPressed(kKey_lshift))
        m_FineMovement = !m_FineMovement;

    if (IsFirstPressed(kRThumbClick))
        m_FineRotation = !m_FineRotation;

    float speedScale = (m_FineMovement ? 0.1f : 1.0f) * timeScale;
    float panScale = (m_FineRotation ? 0.5f : 1.0f) * timeScale;

    float yaw = GetTimeCorrectedAnalogInput(kAnalogRightStickX) * m_HorizontalLookSensitivity * panScale;
    float pitch = GetTimeCorrectedAnalogInput(kAnalogRightStickY) * m_VerticalLookSensitivity * panScale;
    float forward = m_MoveSpeed * speedScale * (
        GetTimeCorrectedAnalogInput(kAnalogLeftStickY) +
        (IsPressed(kKey_w) ? deltaTime : 0.0f) +
        (IsPressed(kKey_s) ? -deltaTime : 0.0f)
        );
    float strafe = m_StrafeSpeed * speedScale * (
        GetTimeCorrectedAnalogInput(kAnalogLeftStickX) +
        (IsPressed(kKey_d) ? deltaTime : 0.0f) +
        (IsPressed(kKey_a) ? -deltaTime : 0.0f)
        );
    float ascent = m_StrafeSpeed * speedScale * (
        GetTimeCorrectedAnalogInput(kAnalogRightTrigger) -
        GetTimeCorrectedAnalogInput(kAnalogLeftTrigger) +
        (IsPressed(kKey_e) ? deltaTime : 0.0f) +
        (IsPressed(kKey_q) ? -deltaTime : 0.0f)
        );

    if (m_Momentum)
    {
        ApplyMomentum(m_LastYaw, yaw, deltaTime);
        ApplyMomentum(m_LastPitch, pitch, deltaTime);
        ApplyMomentum(m_LastForward, forward, deltaTime);
        ApplyMomentum(m_LastStrafe, strafe, deltaTime);
        ApplyMomentum(m_LastAscent, ascent, deltaTime);
    }

    // don't apply momentum to mouse inputs
    yaw += GetAnalogInput(kAnalogMouseX) * m_MouseSensitivityX;
    pitch += GetAnalogInput(kAnalogMouseY) * m_MouseSensitivityY;

    m_CurrentPitch += pitch;
    m_CurrentPitch = XMMin(XM_PIDIV2, m_CurrentPitch);
    m_CurrentPitch = XMMax(-XM_PIDIV2, m_CurrentPitch);

    m_CurrentHeading -= yaw;
    if (m_CurrentHeading > XM_PI)
        m_CurrentHeading -= XM_2PI;
    else if (m_CurrentHeading <= -XM_PI)
        m_CurrentHeading += XM_2PI;

    Matrix3x3 orientation = Matrix3x3(m_WorldEast, m_WorldUp, -m_WorldNorth) * Matrix3x3::MakeYRotation(m_CurrentHeading) * Matrix3x3::MakeXRotation(m_CurrentPitch);
    Vector3 position = orientation * Vector3(strafe, ascent, -forward) + m_TargetCamera.Get_Position();
    m_TargetCamera.SetTransform(AffineTransform(orientation, position));
    m_TargetCamera.Update();
}
