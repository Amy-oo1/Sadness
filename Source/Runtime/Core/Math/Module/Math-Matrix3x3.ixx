module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :Matrix3x3;

import :Forward;
import :Common;
//TODO : import More ?
import :Vector3;

namespace Math {
	using namespace DirectX;

	export  class alignas(16) Matrix3x3 final {
		CONSTEXPR_TRIVIAL_FUNCTION(Matrix3x3)
	public:
		constexpr Matrix3x3(Vector3 x, Vector3 y, Vector3 z) : m_Mat{ x,y,z } {}

		explicit Matrix3x3(FXMMATRIX mat) : m_Mat{ Vector3{mat.r[0]}, Vector3{mat.r[1]}, Vector3{mat.r[2]} } {}

		explicit Matrix3x3(EZeroTag) : m_Mat{ Vector3{EZeroTag::Zero}, Vector3{EZeroTag::Zero}, Vector3{EZeroTag::Zero} } {}
		explicit Matrix3x3(EIdentityTag) : m_Mat{ Vector3{EXUnitVector::X}, Vector3{EYUnitVector::Y}, Vector3{EZUnitVector::Z} } {}


	public:
		[[nodiscard]] Vector3 Get_X(void)const { return this->m_Mat[0]; }
		[[nodiscard]] Vector3 Get_Y(void)const { return this->m_Mat[1]; }
		[[nodiscard]] Vector3 Get_Z(void)const { return this->m_Mat[2]; }


		void Set_X(Vector3 x) { this->m_Mat[0] = x; }
		void Set_Y(Vector3 y) { this->m_Mat[1] = y; }
		void Set_Z(Vector3 z) { this->m_Mat[2] = z; }
		
	private:
		Vector3 m_Mat[3];
	};
}