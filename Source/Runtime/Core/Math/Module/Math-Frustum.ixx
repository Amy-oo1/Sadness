module;

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"


export module Math :Frustum;

import Platform;
import Utility;
import Container;

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
import :Bounding;

import :Function;

namespace Math {

    export class Frustum final {
        CONSTEXPR_TRIVIAL_FUNCTION(Frustum)
    public:
        Frustum(const Matrix4x4& ProjectionMatrix);

        enum CornerID {
            kNearLowerLeft, kNearUpperLeft, kNearLowerRight, kNearUpperRight,
            kFarLowerLeft, kFarUpperLeft, kFarLowerRight, kFarUpperRight
        };

        enum PlaneID { kNearPlane, kFarPlane, kLeftPlane, kRightPlane, kTopPlane, kBottomPlane };

        Vector3         GetFrustumCorner(CornerID id) const { return m_FrustumCorners[id]; }
        BoundingPlane   GetFrustumPlane(PlaneID id) const { return m_FrustumPlanes[id]; }

        bool IntersectSphere(BoundingSphere sphere) const;
        bool IntersectBoundingBox(AxisAlignedBox aabb) const;

        friend Frustum  operator* (const OrthogonalTransform& xform, const Frustum& frustum);	// Fast
        friend Frustum  operator* (const AffineTransform& xform, const Frustum& frustum);		// Slow
        friend Frustum  operator* (const Matrix4x4& xform, const Frustum& frustum);				// Slowest (and most general)

    private:
        void ConstructPerspectiveFrustum(float HTan, float VTan, float NearClip, float FarClip);

        void ConstructOrthographicFrustum(float Left, float Right, float Top, float Bottom, float NearClip, float FarClip);

    private:
        Array<Vector3, 8> m_FrustumCorners;
        Array<BoundingPlane, 6> m_FrustumPlanes;
    };

    Frustum::Frustum(const Matrix4x4& ProjMat){
        const float* ProjMatF { (const float*)&ProjMat };

        const float 
            RcpXX { 1.0f / ProjMatF[0] },
            RcpYY { 1.0f / ProjMatF[5] },
            RcpZZ { 1.0f / ProjMatF[10] };

        // Identify if the projection is perspective or orthographic by looking at the 4th row.
        if (ProjMatF[3] == 0.0f && ProjMatF[7] == 0.0f && ProjMatF[11] == 0.0f && ProjMatF[15] == 1.0f){
            // Orthographic
            float 
                Left    { (-1.0f -  ProjMatF[12]) * RcpXX },
                Right   { (1.0f -   ProjMatF[12]) * RcpXX },
                Top     { (1.0f -   ProjMatF[13]) * RcpYY },
                Bottom  { (-1.0f -  ProjMatF[13]) * RcpYY },
                Front   { (0.0f -   ProjMatF[14]) * RcpZZ },
                Back    { (1.0f -   ProjMatF[14]) * RcpZZ };

            // Check for reverse Z here.  The bounding planes need to point into the frustum.
            if (Front < Back)
                ConstructOrthographicFrustum(Left, Right, Top, Bottom, Front, Back);
            else
                ConstructOrthographicFrustum(Left, Right, Top, Bottom, Back, Front);
        }
        else{
            // Perspective
            float NearClip, FarClip;

            if (RcpZZ > 0.0f){
                FarClip = ProjMatF[14] * RcpZZ;
                NearClip = FarClip / (RcpZZ + 1.0f);
            }
            else{
                NearClip = ProjMatF[14] * RcpZZ;
                FarClip = NearClip / (RcpZZ + 1.0f);
            }

            ConstructPerspectiveFrustum(RcpXX, RcpYY, NearClip, FarClip);
        }
    }

    void Frustum::ConstructPerspectiveFrustum(float HTan, float VTan, float NearClip, float FarClip) {
        const float
            NearX { HTan * NearClip },
            NearY { VTan * NearClip },
            FarX { HTan * FarClip },
            FarY { VTan * FarClip };

        // Define the frustum corners
        this->m_FrustumCorners[kNearLowerLeft] = Vector3(-NearX, -NearY, -NearClip);	// Near lower left
        this->m_FrustumCorners[kNearUpperLeft] = Vector3(-NearX, NearY, -NearClip);	// Near upper left
        this->m_FrustumCorners[kNearLowerRight] = Vector3(NearX, -NearY, -NearClip);	// Near lower right
        this->m_FrustumCorners[kNearUpperRight] = Vector3(NearX, NearY, -NearClip);	// Near upper right
        this->m_FrustumCorners[kFarLowerLeft] = Vector3(-FarX, -FarY, -FarClip);	// Far lower left
        this->m_FrustumCorners[kFarUpperLeft] = Vector3(-FarX, FarY, -FarClip);	// Far upper left
        this->m_FrustumCorners[kFarLowerRight] = Vector3(FarX, -FarY, -FarClip);	// Far lower right
        this->m_FrustumCorners[kFarUpperRight] = Vector3(FarX, FarY, -FarClip);	// Far upper right

        const float
            NHx { RecipSqrt(1.0f + HTan * HTan) },
            NHz { -NHx * HTan },
            NVy { RecipSqrt(1.0f + VTan * VTan) },
            NVz { -NVy * VTan };

        // Define the bounding planes
        this->m_FrustumPlanes[kNearPlane] = BoundingPlane(0.0f, 0.0f, -1.0f, -NearClip);
        this->m_FrustumPlanes[kFarPlane] = BoundingPlane(0.0f, 0.0f, 1.0f, FarClip);
        this->m_FrustumPlanes[kLeftPlane] = BoundingPlane(NHx, 0.0f, NHz, 0.0f);
        this->m_FrustumPlanes[kRightPlane] = BoundingPlane(-NHx, 0.0f, NHz, 0.0f);
        this->m_FrustumPlanes[kTopPlane] = BoundingPlane(0.0f, -NVy, NVz, 0.0f);
        this->m_FrustumPlanes[kBottomPlane] = BoundingPlane(0.0f, NVy, NVz, 0.0f);
    }

    void Frustum::ConstructOrthographicFrustum(float Left, float Right, float Top, float Bottom, float Front, float Back){
        // Define the frustum corners
        this->m_FrustumCorners[kNearLowerLeft] = Vector3(Left, Bottom, -Front);	// Near lower left
        this->m_FrustumCorners[kNearUpperLeft] = Vector3(Left, Top, -Front);	// Near upper left
        this->m_FrustumCorners[kNearLowerRight] = Vector3(Right, Bottom, -Front);	// Near lower right
        this->m_FrustumCorners[kNearUpperRight] = Vector3(Right, Top, -Front);	// Near upper right
        this->m_FrustumCorners[kFarLowerLeft] = Vector3(Left, Bottom, -Back);	// Far lower left
        this->m_FrustumCorners[kFarUpperLeft] = Vector3(Left, Top, -Back);	// Far upper left
        this->m_FrustumCorners[kFarLowerRight] = Vector3(Right, Bottom, -Back);	// Far lower right
        this->m_FrustumCorners[kFarUpperRight] = Vector3(Right, Top, -Back);	// Far upper right

        // Define the bounding planes
        this->m_FrustumPlanes[kNearPlane] = BoundingPlane(0.0f, 0.0f, 1.0f, -Front);
        this->m_FrustumPlanes[kFarPlane] = BoundingPlane(0.0f, 0.0f, -1.0f, Back);
        this->m_FrustumPlanes[kLeftPlane] = BoundingPlane(1.0f, 0.0f, 0.0f, -Left);
        this->m_FrustumPlanes[kRightPlane] = BoundingPlane(-1.0f, 0.0f, 0.0f, Right);
        this->m_FrustumPlanes[kBottomPlane] = BoundingPlane(0.0f, 1.0f, 0.0f, -Bottom);
        this->m_FrustumPlanes[kTopPlane] = BoundingPlane(0.0f, -1.0f, 0.0f, Top);
    }

    inline bool Frustum::IntersectBoundingBox(AxisAlignedBox aabb) const{
        for (auto& p: this->m_FrustumPlanes) {
            Vector3 farCorner = Select(aabb.Get_Min(), aabb.Get_Max(), p.Get_Normal() > Vector3 { EZeroTag::Zero });
            if (p.DistanceFromPoint(farCorner) < 0.0f)
                return false;
        }

        return true;
    }

    inline Frustum operator* (const OrthogonalTransform& xform, const Frustum& frustum){
        Frustum result {};

        for (int Index = 0; const auto& corner : frustum.m_FrustumCorners)
            result.m_FrustumCorners[Index++] = xform * corner;

		for (int Index = 0; const auto& plane : frustum.m_FrustumPlanes)
			result.m_FrustumPlanes[Index++] = xform * plane;

        return result;
    }

    inline Frustum operator* (const AffineTransform& xform, const Frustum& frustum){
        Frustum result {};

        for (int Index = 0; const auto& corner : frustum.m_FrustumCorners)
            result.m_FrustumCorners[Index++] = xform * corner;

        Matrix4x4 XForm { Transpose(Invert(Matrix4x4{xform})) };

		for (int Index = 0; const auto& plane : frustum.m_FrustumPlanes)
			result.m_FrustumPlanes[Index++] = BoundingPlane { XForm * Vector4{ plane } };

        return result;
    }

    inline Frustum operator* (const Matrix4x4& mtx, const Frustum& frustum){
        Frustum result {};

		for (int Index = 0; const auto& corner : frustum.m_FrustumCorners)
			result.m_FrustumCorners[Index++] = mtx * corner;

        Matrix4x4 XForm { Transpose(Invert(mtx)) };

		for (int Index = 0  ; const auto& plane : frustum.m_FrustumPlanes)
			result.m_FrustumPlanes[Index++] = BoundingPlane { XForm * Vector4{ plane } };

        return result;
    }

    inline bool Frustum::IntersectSphere(BoundingSphere sphere) const{
        float radius { sphere.Get_Radius() };

        for (const auto & plane : this->m_FrustumPlanes)
            if (plane.DistanceFromPoint(sphere.Get_Center()) + radius < 0.0f)
                return false;

        return true;
    }

}