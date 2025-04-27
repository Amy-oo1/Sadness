module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Math :Implementation;

import :Forward;
import :Common;
import :BoolVector;
import :Scalar;
import :Vector2;
import :Vector3;
import :Vector4;
import :Matrix3x3;
import :Matrix4x4;
import :Quaternion;
import :Transform;
import :Bounding;
import :Function;

namespace Math {

	using namespace DirectX;

	Vector2::Vector2(Vector3 v) :m_Vec { static_cast<XMVECTOR>(v) } {}

	Vector3::Vector3(Vector4 v) :m_Vec { static_cast<XMVECTOR>(v) } {}

	Matrix3x3::Matrix3x3(Quaternion q) { *this = Matrix3x3 { XMMatrixRotationQuaternion(q) }; }


    FORCINLINE OrientedBox operator* (const UniformTransform& xform, const OrientedBox& obb) { return AffineTransform { xform } *obb; }

	FORCINLINE OrientedBox operator* (const UniformTransform& xform, const AxisAlignedBox& aabb) { return AffineTransform { xform } * OrientedBox { aabb }; }


}