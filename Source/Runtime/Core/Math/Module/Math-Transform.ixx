module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

export module Math:Transform;

import Platform;
import Utility;

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

import :Function;




namespace Math {

	using namespace DirectX;

	export class OrthogonalTransform final {
		CONSTEXPR_TRIVIAL_FUNCTION(OrthogonalTransform)
	public:
		OrthogonalTransform(EIdentityTag) {};

		OrthogonalTransform(Quaternion rotate) : m_Rotation { rotate } {};
		OrthogonalTransform(Vector3 translate) : m_Translation { translate } {};
		OrthogonalTransform(Quaternion rotate, Vector3 translate) : m_Rotation { rotate }, m_Translation { translate } {};

		OrthogonalTransform(Matrix3x3 mat) :m_Rotation { mat } {}
		OrthogonalTransform(Matrix3x3 mat, Vector3 translate) :m_Rotation { mat }, m_Translation { translate } {}

		explicit OrthogonalTransform(const XMMATRIX& mat) { *this = OrthogonalTransform { Matrix3x3 { mat }, Vector3 { mat.r[3] } }; }

	public:

		Vector3 operator* (Vector3 vec) const { return this->m_Rotation * vec + this->m_Translation; }
		Vector4 operator* (Vector4 vec) const {
			return
				Vector4 { SetWToZero(this->m_Rotation * Vector3 { vec }) } +
				Vector4 { SetWToOne(this->m_Translation) } *vec.GetW();
		}
		/*BoundingSphere operator* (BoundingSphere sphere) const {
			return BoundingSphere(*this * sphere.Get_Center(), sphere.Get_Radius());
		}*/

		OrthogonalTransform operator* (const OrthogonalTransform& xform) const { 
			return OrthogonalTransform { 
				this->m_Rotation * xform.m_Rotation, 
				this->m_Rotation * xform.m_Translation + this->m_Translation 
			}; 
		}

		OrthogonalTransform operator~ () const {
			Quaternion invertedRotation { ~m_Rotation };
			return OrthogonalTransform { invertedRotation, invertedRotation * this->m_Translation };
		}

	public:
		Quaternion Get_Rotation(void) const { return this->m_Rotation; }
		Vector3 Get_Translation(void) const { return this->m_Translation; }

		void Set_Rotation(Quaternion rotate) { this->m_Rotation = rotate; }
		void Set_Translation(Vector3 translate) { this->m_Translation = translate; }

	public:
		static OrthogonalTransform MakeXRotation(float angle) { return OrthogonalTransform { Quaternion { Vector3 { EXUnitVector::X }, angle } }; }
		static OrthogonalTransform MakeYRotation(float angle) { return OrthogonalTransform { Quaternion { Vector3 { EYUnitVector::Y }, angle } }; }
		static OrthogonalTransform MakeZRotation(float angle) { return OrthogonalTransform { Quaternion { Vector3 { EZUnitVector::Z }, angle } }; }
		static OrthogonalTransform MakeTranslation(Vector3 translate) { return OrthogonalTransform { translate }; }

	private: 
		Quaternion m_Rotation {EIdentityTag::Identity};
		Vector3 m_Translation { EZeroTag::Zero };
	};

	export class ScaleAndTranslation final {
		CONSTEXPR_TRIVIAL_FUNCTION(ScaleAndTranslation)
	public:
		ScaleAndTranslation(EIdentityTag) {};

		ScaleAndTranslation(float tx, float ty, float tz, float scale): m_repr(tx, ty, tz, scale) {}
		ScaleAndTranslation(Vector3 translate, Scalar scale) :m_repr { translate } { m_repr.SetW(scale); }
		explicit ScaleAndTranslation(FXMVECTOR v) : m_repr(v) {}

	public:
		Scalar Get_Scale(void) const { return this->m_repr.GetW(); }
		Vector3 Get_Translation(void) const { return Vector3 { this->m_repr }; }

		void Set_Scale(Scalar scale) { this->m_repr.SetW(scale); }
		void Set_Translation(Vector3 translate) { this->m_repr = Vector4 { translate }; }



	public:



	private:
		Vector4 m_repr { EWUnitVector::W};
	
	};

	export class UniformTransform final{
		CONSTEXPR_TRIVIAL_FUNCTION(UniformTransform)
	public:
		UniformTransform(EIdentityTag) {}

		UniformTransform(Quaternion rotation, ScaleAndTranslation transScale): m_Rotation(rotation), m_TranslationScale(transScale){}
		UniformTransform(Quaternion rotation, Scalar scale, Vector3 translation): m_Rotation(rotation), m_TranslationScale(translation, scale){}

	public:
		Quaternion GetRotation() const { return this->m_Rotation; }
		Scalar Get_Scale() const { return this->m_TranslationScale.Get_Scale(); }
		Vector3 Get_Translation() const { return this->m_TranslationScale.Get_Translation(); }

		void Set_Rotation(Quaternion r) { this->m_Rotation = r; }
		void Set_Scale(Scalar s) { this->m_TranslationScale.Set_Scale(s); }
		void Set_Translation(Vector3 t) { this->m_TranslationScale.Set_Translation(t); }




		Vector3 operator*(Vector3 vec) const{return this->m_Rotation * (vec * m_TranslationScale.Get_Scale()) + m_TranslationScale.Get_Translation();}

		/*BoundingSphere operator*(BoundingSphere sphere) const
		{
			return BoundingSphere(*this * sphere.Get_Center(), Get_Scale() * sphere.Get_Radius());
		}*/

	private:
		Quaternion m_Rotation {EIdentityTag::Identity};
		ScaleAndTranslation m_TranslationScale { EIdentityTag::Identity };

	};

	export class AffineTransform final {
		CONSTEXPR_TRIVIAL_FUNCTION(AffineTransform)
	public:
		AffineTransform(EIdentityTag) {}

		AffineTransform(Vector3 x, Vector3 y, Vector3 z, Vector3 w): m_basis(x, y, z), m_translation(w) {}
		AffineTransform(Vector3 translate): m_translation(translate) {}
		AffineTransform(const Matrix3x3& mat, Vector3 translate = Vector3 { EZeroTag::Zero }): m_basis(mat), m_translation(translate) {}
		AffineTransform(Quaternion rot, Vector3 translate = Vector3 { EZeroTag::Zero }): m_basis(rot), m_translation(translate) {}
		AffineTransform(const OrthogonalTransform& xform): m_basis(xform.Get_Rotation()), m_translation(xform.Get_Translation()) {}
		AffineTransform(const UniformTransform& xform) :m_basis { Matrix3x3 { xform.GetRotation() } *xform.Get_Scale() }, m_translation { xform.Get_Translation() } {}
		explicit AffineTransform(const XMMATRIX& mat): m_basis(mat), m_translation(mat.r[3]) {}

		operator XMMATRIX() const { return (XMMATRIX&)*this; }
	
	public:
		Vector3 operator* (Vector3 vec) const { return m_basis * vec + m_translation; }
		AffineTransform operator* (const AffineTransform& mat) const { return AffineTransform(m_basis * mat.m_basis, *this * mat.Get_Translation()); }

	public:

		Vector3 Get_X() const { return this->m_basis.Get_X(); }
		Vector3 Get_Y() const { return this->m_basis.Get_Y(); }
		Vector3 Get_Z() const { return this->m_basis.Get_Z(); }
		Vector3 Get_Translation() const { return this->m_translation; }
		Matrix3x3 Get_Basis() const { return this->m_basis; }

		void Set_X(Vector3 x) { this->m_basis.Set_X(x); }
		void Set_Y(Vector3 y) { this->m_basis.Set_Y(y); }
		void Set_Z(Vector3 z) { this->m_basis.Set_Z(z); }
		void Set_Translation(Vector3 w) { this->m_translation = w; }
		void Set_Basis(Matrix3x3 basis) { this->m_basis = basis; }

		
	public:
		static AffineTransform MakeXRotation(float angle) { return AffineTransform { Matrix3x3::MakeXRotation(angle) }; }
		static AffineTransform MakeYRotation(float angle) { return AffineTransform { Matrix3x3::MakeYRotation(angle) }; }
		static AffineTransform MakeZRotation(float angle) { return AffineTransform { Matrix3x3::MakeZRotation(angle) }; }
		static AffineTransform MakeScale(float scale) { return AffineTransform { Matrix3x3::MakeScale(scale) }; }
		static AffineTransform MakeScale(Vector3 scale) { return AffineTransform { Matrix3x3::MakeScale(scale) }; }
		static AffineTransform MakeTranslation(Vector3 translate) { return AffineTransform(translate); }

		
	private:
		Matrix3x3 m_basis {};
		Vector3 m_translation {};

	};


}