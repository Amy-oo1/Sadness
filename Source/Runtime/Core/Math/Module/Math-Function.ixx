module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

export module Math:Function;

import :Forward;
import :Common;
import :Scalar;
import :BoolVector;
import :Vector2;
import :Vector3;
import :Vector4;
import :Matrix3x3;
import :Matrix4x4;
import :Quaternion;



namespace Math {
	//TODO :
	using namespace DirectX;

#define CREATE_SIMD_FUNCTIONS(TYPE)\
export FORCINLINE TYPE Sqrt( TYPE s) {return  static_cast<TYPE>(XMVectorSqrt(s));}\
export FORCINLINE TYPE Recip( TYPE s) { return static_cast<TYPE>(XMVectorReciprocal(s));} \
export FORCINLINE TYPE RecipSqrt( TYPE s){ return static_cast<TYPE>(XMVectorReciprocalSqrt(s));}\
export FORCINLINE TYPE Floor( TYPE s ) { return static_cast<TYPE>(XMVectorFloor(s)); } \
export FORCINLINE TYPE Ceiling( TYPE s ) { return static_cast<TYPE>(XMVectorCeiling(s)); } \
export FORCINLINE TYPE Round( TYPE s ) { return static_cast<TYPE>(XMVectorRound(s)); } \
export FORCINLINE TYPE Abs( TYPE s ) { return static_cast<TYPE>(XMVectorAbs(s)); } \
export FORCINLINE TYPE Exp( TYPE s ) { return static_cast<TYPE>(XMVectorExp(s)); } \
export FORCINLINE TYPE Pow( TYPE b, TYPE e ) { return static_cast<TYPE>(XMVectorPow(b, e)); } \
export FORCINLINE TYPE Log( TYPE s ) { return static_cast<TYPE>(XMVectorLog(s)); } \
export FORCINLINE TYPE Sin( TYPE s ) { return static_cast<TYPE>(XMVectorSin(s)); } \
export FORCINLINE TYPE Cos( TYPE s ) { return static_cast<TYPE>(XMVectorCos(s)); } \
export FORCINLINE TYPE Tan( TYPE s ) { return static_cast<TYPE>(XMVectorTan(s)); } \
export FORCINLINE TYPE ASin( TYPE s ) { return static_cast<TYPE>(XMVectorASin(s)); } \
export FORCINLINE TYPE ACos( TYPE s ) { return static_cast<TYPE>(XMVectorACos(s)); } \
export FORCINLINE TYPE ATan( TYPE s ) { return static_cast<TYPE>(XMVectorATan(s)); } \
export FORCINLINE TYPE ATan2( TYPE y, TYPE x ) { return static_cast<TYPE>(XMVectorATan2(y, x)); } \
export FORCINLINE TYPE Lerp( TYPE a, TYPE b, TYPE t ) { return static_cast<TYPE>(XMVectorLerpV(a, b, t)); } \
export FORCINLINE TYPE Lerp( TYPE a, TYPE b, float t ) { return static_cast<TYPE>(XMVectorLerp(a, b, t)); } \
export FORCINLINE TYPE Max( TYPE a, TYPE b ) { return static_cast<TYPE>(XMVectorMax(a, b)); } \
export FORCINLINE TYPE Min( TYPE a, TYPE b ) { return static_cast<TYPE>(XMVectorMin(a, b)); } \
export FORCINLINE TYPE Clamp( TYPE v, TYPE a, TYPE b ) { return Min(Max(v, a), b); } \
export FORCINLINE BoolVector operator<  ( TYPE lhs, TYPE rhs ) { return XMVectorLess(lhs, rhs); } \
export FORCINLINE BoolVector operator<= ( TYPE lhs, TYPE rhs ) { return XMVectorLessOrEqual(lhs, rhs); } \
export FORCINLINE BoolVector operator>  ( TYPE lhs, TYPE rhs ) { return XMVectorGreater(lhs, rhs); } \
export FORCINLINE BoolVector operator>= ( TYPE lhs, TYPE rhs ) { return XMVectorGreaterOrEqual(lhs, rhs); } \
export FORCINLINE BoolVector operator== ( TYPE lhs, TYPE rhs ) { return XMVectorEqual(lhs, rhs); } \
export FORCINLINE BoolVector operator!= ( TYPE lhs, TYPE rhs ) { return XMVectorNotEqual(lhs, rhs); } \
export FORCINLINE TYPE Select( TYPE lhs, TYPE rhs, BoolVector mask ) { return TYPE(XMVectorSelect(lhs, rhs, mask)); }\

CREATE_SIMD_FUNCTIONS(Scalar)
CREATE_SIMD_FUNCTIONS(Vector2)
CREATE_SIMD_FUNCTIONS(Vector3)
CREATE_SIMD_FUNCTIONS(Vector4)
#undef CREATE_SIMD_FUNCTIONS


export FORCINLINE float Sqrt(float s) { return Sqrt(Scalar(s)); }
export FORCINLINE float Recip(float s) { return Recip(Scalar(s)); }
export FORCINLINE float RecipSqrt(float s) { return RecipSqrt(Scalar(s)); }
export FORCINLINE float Floor(float s) { return Floor(Scalar(s)); }
export FORCINLINE float Ceiling(float s) { return Ceiling(Scalar(s)); }
export FORCINLINE float Round(float s) { return Round(Scalar(s)); }
export FORCINLINE float Abs(float s) { return s < 0.0f ? -s : s; }
export FORCINLINE float Exp(float s) { return Exp(Scalar(s)); }
export FORCINLINE float Pow(float b, float e) { return Pow(Scalar(b), Scalar(e)); }
export FORCINLINE float Log(float s) { return Log(Scalar(s)); }
export FORCINLINE float Sin(float s) { return Sin(Scalar(s)); }
export FORCINLINE float Cos(float s) { return Cos(Scalar(s)); }
export FORCINLINE float Tan(float s) { return Tan(Scalar(s)); }
export FORCINLINE float ASin(float s) { return ASin(Scalar(s)); }
export FORCINLINE float ACos(float s) { return ACos(Scalar(s)); }
export FORCINLINE float ATan(float s) { return ATan(Scalar(s)); }
export FORCINLINE float ATan2(float y, float x) { return ATan2(Scalar(y), Scalar(x)); }
export FORCINLINE float Lerp(float a, float b, float t) { return a + (b - a) * t; }
export FORCINLINE float Max(float a, float b) { return a > b ? a : b; }
export FORCINLINE float Min(float a, float b) { return a < b ? a : b; }
export FORCINLINE float Clamp(float v, float a, float b) { return Min(Max(v, a), b); }

export FORCINLINE Scalar Length(Vector3 v) { return Scalar(XMVector3Length(v)); }
export FORCINLINE Scalar LengthSquare(Vector3 v) { return Scalar(XMVector3LengthSq(v)); }
export FORCINLINE Scalar LengthRecip(Vector3 v) { return Scalar(XMVector3ReciprocalLength(v)); }
export FORCINLINE Scalar Dot(Vector3 v1, Vector3 v2) { return Scalar(XMVector3Dot(v1, v2)); }
export FORCINLINE Scalar Dot(Vector4 v1, Vector4 v2) { return Scalar(XMVector4Dot(v1, v2)); }
export FORCINLINE Vector3 Cross(Vector3 v1, Vector3 v2) { return Vector3(XMVector3Cross(v1, v2)); }
export FORCINLINE Vector3 Normalize(Vector3 v) { return Vector3(XMVector3Normalize(v)); }
export FORCINLINE Vector4 Normalize(Vector4 v) { return Vector4(XMVector4Normalize(v)); }

export FORCINLINE Matrix3x3 Transpose(const Matrix3x3& mat) { return static_cast<Matrix3x3>(XMMatrixTranspose(mat)); }
export FORCINLINE Matrix3x3 InverseTranspose(const Matrix3x3& mat){
	const Vector3 x { mat.Get_X() };
	const Vector3 y { mat.Get_Y() };
	const Vector3 z { mat.Get_Z() };

	const Vector3 inv0 { Cross(y, z) };
	const Vector3 inv1 { Cross(z, x) };
	const Vector3 inv2 { Cross(x, y) };
	const Scalar  rDet { Recip(Dot(z, inv2)) };

	// Return the adjoint / determinant
	return Matrix3x3 { inv0, inv1, inv2 } * rDet;
}

export FORCINLINE Matrix4x4 Transpose(Matrix4x4 mat) { return Matrix4x4(XMMatrixTranspose(mat)); }
export FORCINLINE Matrix4x4 Invert(Matrix4x4 mat) { return Matrix4x4 { XMMatrixInverse(nullptr, mat) }; }

export FORCINLINE Matrix4x4 OrthoInvert(Matrix4x4 xform) {
		auto  basis { Transpose(xform) };
		Vector3 translate { basis * -Vector3(xform.Get_W()) };

		return Matrix4x4 { basis, translate };
	}

export FORCINLINE Quaternion Normalize(Quaternion q) { return Quaternion(XMQuaternionNormalize(q)); }
export FORCINLINE Quaternion Slerp(Quaternion a, Quaternion b, float t) { return Normalize(Quaternion(XMQuaternionSlerp(a, b, t))); }
export FORCINLINE Quaternion Lerp(Quaternion a, Quaternion b, float t) { return Normalize(Quaternion(XMVectorLerp(a, b, t))); }

}

