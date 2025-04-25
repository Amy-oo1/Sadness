module;
#include "ThirdParty/Windows/DirectX/Include/DirectXMath.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Color;

import Platform;

using namespace DirectX;

export class Color final {
	CONSTEXPR_TRIVIAL_FUNCTION(Color);
public:
	Color(FXMVECTOR vec) {
		this->m_Value.v = vec;
	}

	Color(float r, float g, float b, float a=1.f) {
		this->m_Value.v = XMVectorSet(r, g, b, a);
	}

	Color(Uint16 r, Uint16 g, Uint16 b, Uint16 a, Uint16 bitDepth)
	{
		this->m_Value.v = XMVectorScale(XMVectorSet(r, g, b, a), 1.0f / ((1 << bitDepth) - 1));
	}

	explicit Color(Uint32 u32) {
		float r { static_cast<float>((u32 >> 0) & 0xFF) };
		float g { static_cast<float>((u32 >> 8) & 0xFF) };
		float b { static_cast<float>((u32 >> 16) & 0xFF) };
		float a { static_cast<float>((u32 >> 24) & 0xFF) };
		this->m_Value.v = XMVectorScale(XMVectorSet(r, g, b, a), 1.0f / 255.0f);
	}


public:

	bool operator==(const Color& rhs)const { return XMVector4Equal(this->m_Value, rhs.m_Value); }
	bool operator!=(const Color& rhs)const { return !((*this) == rhs); }

	float& operator[](Uint32 index) {
		ASSERT(index < 4);
		
		return this->Get_Ptr()[index];
	}

	const float& operator[](Uint32 index)const {
		ASSERT(index < 4);

		return this->Get_Ptr()[index];
	}

public:
	float R(void)const { return XMVectorGetX(this->m_Value); }
	float G(void)const { return XMVectorGetY(this->m_Value); }
	float B(void)const { return XMVectorGetZ(this->m_Value); }
	float A(void)const { return XMVectorGetW(this->m_Value); }

	void Set_R(float r) { this->m_Value.f[0] = r; }
	void Set_G(float g) { this->m_Value.f[1] = g; }
	void Set_B(float b) { this->m_Value.f[2] = b; }
	void Set_A(float a) { this->m_Value.f[3] = a; }

	void Set_RGB(float r, float g, float b) {
		this->m_Value.f[0] = r;
		this->m_Value.f[1] = g;
		this->m_Value.f[2] = b;
	}

private:
	float* Get_Ptr(void) { return reinterpret_cast<float*>(this); }
	const float* Get_Ptr(void)const { return reinterpret_cast<const float*>(this); }
	

private:
	XMVECTORF32 m_Value { g_XMOne };

};