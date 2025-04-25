module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Math :BoolVector;

import :Forward;

namespace Math {

	using namespace DirectX;
	
	export class BoolVector final {
		CONSTEXPR_TRIVIAL_FUNCTION(BoolVector)
	public:
		BoolVector(FXMVECTOR Vec) : m_Vec{ Vec } {}

		operator XMVECTOR(void) const { return m_Vec; }

	public:
		XMVECTOR m_Vec;

	};

}