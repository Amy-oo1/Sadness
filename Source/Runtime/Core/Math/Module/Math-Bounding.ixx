module;

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"


export module Math :Bounding;

import Platform;
import Utility;

import :Common;
import :BoolVector;
import :Scalar;
import :Vector2;
import :Vector3;
import :Vector4;
import :Quaternion;
import :Matrix3x3;
import :Matrix4x4;
import :Transform;

import :Function;

namespace Math {

	using namespace DirectX;

	export class AxisAlignedBox final {
		CONSTEXPR_TRIVIAL_FUNCTION(AxisAlignedBox)
	public:
		AxisAlignedBox(EZeroTag) {};
		AxisAlignedBox(Vector3 min, Vector3 max) : m_Min(min), m_Max(max) {};

	public:
		void AddPoint(Vector3 point) {
			this->m_Min = Min(this->m_Min, point);
			this->m_Max = Max(this->m_Max, point);
		}

		//TODO Dont use this 
		AxisAlignedBox Union(AxisAlignedBox box) const { return AxisAlignedBox { Min(this->m_Min, box.m_Min), Max(this->m_Max, box.m_Max) }; }

		Vector3 Get_Min(void)const { return this->m_Min; }
		Vector3 Get_Max(void)const { return this->m_Max; }
		Vector3 Get_Center(void)const { return (this->m_Min + this->m_Max) * 0.5f; }
		Vector3 Get_Dimensions(void)const { Max(this->m_Max - this->m_Min, Vector3{EZeroTag::Zero}); }


	private:
		Vector3 m_Min { Scalar{Max_Float} };
		Vector3 m_Max { Scalar{Min_Float} };
	};

	export class OrientedBox final {
		CONSTEXPR_TRIVIAL_FUNCTION(OrientedBox)
	public:
		OrientedBox(const AxisAlignedBox& box) {
			m_repr.Set_Basis(Matrix3x3::MakeScale(box.Get_Max() - box.Get_Min()));
			m_repr.Set_Translation(box.Get_Min());
		}

		explicit OrientedBox(AffineTransform data) : m_repr { data } {}

	public:

		friend OrientedBox operator* (const AffineTransform& xform, const OrientedBox& obb) { return OrientedBox{ xform * obb.m_repr}; }


	public:
		Vector3 Get_Center(void) const { return this->m_repr.Get_Translation() + this->Get_tDimensions() * 0.5f; }
		Vector3 Get_tDimensions(void) const { return this->m_repr.Get_X() + this->m_repr.Get_Y() + this->m_repr.Get_Z(); }

	private:
		AffineTransform m_repr;
	};

	export class BoundingPlane final {
		CONSTEXPR_TRIVIAL_FUNCTION(BoundingPlane)
	public:
		BoundingPlane(Vector3 normalToPlane, float distanceFromOrigin) : m_repr { normalToPlane, distanceFromOrigin } {}
		BoundingPlane(Vector3 pointOnPlane, Vector3 normalToPlane) {
			normalToPlane = Normalize(normalToPlane);
			m_repr = Vector4 { normalToPlane, -Dot(pointOnPlane, normalToPlane) };
		}
		BoundingPlane(float A, float B, float C, float D) : m_repr(A, B, C, D) {}
		explicit BoundingPlane(Vector4 plane) : m_repr(plane) {}

		operator Vector4() const { return m_repr; }

	public:
		Vector3 Get_Normal(void) const { return Vector3 { m_repr }; }

		Vector3 Get_PointOnPlane(void) const { return -Get_Normal() * m_repr.GetW(); }

		Scalar DistanceFromPoint(Vector3 point) const { return Dot(point, Get_Normal()) + m_repr.GetW(); }
		Scalar DistanceFromPoint(Vector4 point) const { return Dot(point, m_repr); }

		friend BoundingPlane operator* (const OrthogonalTransform& xform, BoundingPlane plane){
			const auto  normalToPlane { xform.Get_Rotation() * plane.Get_Normal() };
			float distanceFromOrigin { plane.m_repr.GetW() - Dot(normalToPlane, xform.Get_Translation()) };
			return BoundingPlane { normalToPlane, distanceFromOrigin };
		}

		friend BoundingPlane operator* (const Matrix4x4& mat, BoundingPlane plane) { return BoundingPlane(Transpose(Invert(mat)) * plane.m_repr); }

	public:
		static BoundingPlane PlaneFromPointsCCW(Vector3 A, Vector3 B, Vector3 C) { return BoundingPlane { A, Cross(B - A, C - A) }; }

	private:
		Vector4 m_repr;
	};

	export class BoundingSphere final {
		CONSTEXPR_TRIVIAL_FUNCTION(BoundingSphere)
	public:
		BoundingSphere(float x, float y, float z, float r) : m_repr(x, y, z, r) {}
		BoundingSphere(const XMFLOAT4* unaligned_array) : m_repr(*unaligned_array) {}
		BoundingSphere(Vector3 center, Scalar radius) :m_repr { center, radius } {}
		BoundingSphere(EZeroTag) : m_repr(EZeroTag::Zero) {}

		explicit BoundingSphere(const XMVECTOR& v) : m_repr(v) {}
		explicit BoundingSphere(const XMFLOAT4& f4) : m_repr(f4) {}
		explicit BoundingSphere(Vector4 sphere) : m_repr(sphere) {}
		explicit operator Vector4() const { return Vector4(m_repr); }

		Vector3 Get_Center(void) const { return Vector3(m_repr); }
		Scalar Get_Radius(void) const { return m_repr.GetW(); }

		BoundingSphere Union(const BoundingSphere& rhs) {
			float radA { Get_Radius() };
			if (radA == 0.0f)
				return rhs;

			float radB { rhs.Get_Radius() };
			if (radB == 0.0f)
				return *this;

			Vector3 diff { Get_Center() - rhs.Get_Center() };
			float dist { Length(diff) };

			diff = dist < 1e-6f ? Vector3(EXUnitVector::X) : diff * Recip(dist);

			Vector3 extremeA { Get_Center() + diff * Max(radA, radB - dist) };
			Vector3 extremeB { rhs.Get_Center() - diff * Max(radB, radA - dist) };

			return BoundingSphere { (extremeA + extremeB) * 0.5f, Length(extremeA - extremeB) * 0.5f };
		}

	private:

		Vector4 m_repr;
	};

}