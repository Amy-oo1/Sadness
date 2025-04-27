export module Math :Forward;

namespace Math {
	export class BoolVedctor;
	export class Scalar;
	export class Vector2;
	export class Vector3;
	export class Vector4;
	export class Matrix3x3;
	export class Matrix4x4;
	export class Quaternion;

	Matrix4x4 Transpose(Matrix4x4 mat);
	Matrix4x4 Invert(Matrix4x4 mat);
	Matrix4x4 OrthoInvert(Matrix4x4 xform);

}