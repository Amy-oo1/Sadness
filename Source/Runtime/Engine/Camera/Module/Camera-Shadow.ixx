module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Camera :Shadow;

import Platform;
import Utility;
import Math;

import :Base;

using namespace Math;


export class ShadowCamera final :public CameraBase<ShadowCamera> {
	friend class CameraBase<ShadowCamera>;
public:
	ShadowCamera(void) = default;

	~ShadowCamera(void) = default;

    void UpdateMatrix(
        Vector3 LightDirection,		// Direction parallel to light, in direction of travel
        Vector3 ShadowCenter,		// Center location on far bounding plane of shadowed region
        Vector3 ShadowBounds,		// Width, height, and depth in world space represented by the shadow buffer
        Uint32 BufferWidth,		// Shadow buffer width
        Uint32 BufferHeight,		// Shadow buffer height--usually same as width
        Uint32 BufferPrecision	// Bit depth of shadow buffer--usually 16 or 24
    );


public:
	const Matrix4x4& Get_ShadowMatrix() const { return m_ShadowMatrix; }

private:
    void Imp_Update() {}

private:

	Matrix4x4 m_ShadowMatrix;
};

void ShadowCamera::UpdateMatrix(
    Vector3 LightDirection, Vector3 ShadowCenter, Vector3 ShadowBounds,
    Uint32 BufferWidth, Uint32 BufferHeight, Uint32 BufferPrecision)
{
    this->Set_LookDirection(LightDirection, Vector3(EZUnitVector::Z));

    // Converts world units to texel units so we can quantize the camera position to whole texel units
    Vector3 RcpDimensions = Recip(ShadowBounds);
    Vector3 QuantizeScale = Vector3((float)BufferWidth, (float)BufferHeight, (float)((1 << BufferPrecision) - 1)) * RcpDimensions;

    //
    // Recenter the camera at the quantized position
    //

    // Transform to view space
    ShadowCenter = ~Get_Rotation() * ShadowCenter;
    // Scale to texel units, truncate fractional part, and scale back to world units
    ShadowCenter = Floor(ShadowCenter * QuantizeScale) / QuantizeScale;
    // Transform back into world space
    ShadowCenter = Get_Rotation() * ShadowCenter;

    Set_Position(ShadowCenter);

    Set_ProjectionMatrix(Matrix4x4::MakeScale(Vector3(2.0f, 2.0f, 1.0f) * RcpDimensions));

    this->Update();

    // Transform from clip space to texture space
    m_ShadowMatrix = Matrix4x4(AffineTransform(Matrix3x3::MakeScale(0.5f, -0.5f, 1.0f), Vector3(0.5f, 0.5f, 0.0f))) * this->m_ViewProjectionMatrix;
}
