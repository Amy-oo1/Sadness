module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

export module Math :Implementation;

import :Forward;
import :Common;
import :BoolVector;
import :Scalar;
import :Vector2;
import :Vector3;
import :Vector4;

namespace Math {

	using namespace DirectX;

	Vector2::Vector2(Vector3 v) {
		this->m_Vec = static_cast<XMVECTOR>(v);
	}

	Vector3::Vector3(Vector4 v){
		this->m_Vec = static_cast<XMVECTOR>(v);
	}

}