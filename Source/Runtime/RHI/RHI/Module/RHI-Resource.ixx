module;
#include "d3d12.h"

#include "ThirdParty/Vulkan/Include/vulkan/vulkan.h"

export module RHI:Resource;

import Platform;
import Utility;
import Container;

import :Platform;

export enum class RHIPixelFormat :Uint32 {
	UNKNOWN = 0,

	R8G8B8A8_UNORM = 1,
	R8G8B8A8_UNORM_SRGB = 2,
	R8G8B8A8_UINT = 3,
	R8G8B8A8_SNORM = 4,


	COUNT
};

export Size BitsPerPixel(RHIPixelFormat format) {
	using enum RHIPixelFormat;
	switch (format) {
	case R8G8B8A8_UNORM:
	case R8G8B8A8_UNORM_SRGB:
	case R8G8B8A8_UINT:
	case R8G8B8A8_SNORM:
		return 32;
	default:
		return 0;
	}
}

export template<typename APITag = g_CurrentAPI>
class RHIPixelFormatManager final :public Singleton {
	static_assert(requires { typename APITag::PixelFormat; },
		"APITag must contain PixelFormat type");
private:
	using PlatformPixelFormat = typename RHITypeTraits<APITag>::PixelFormat;

	struct RHIPixelFormatInfo final {
		RHIPixelFormat Format;
		PlatformPixelFormat Platform;
	};

private:
	RHIPixelFormatManager(void) = default;

public:
	~RHIPixelFormatManager(void) = default;

public:
	static RHIPixelFormatManager& Get(void) {
		static RHIPixelFormatManager instance {};

		return instance;
	}

	void AddTransFormat(RHIPixelFormat format, PlatformPixelFormat platformFormat) {
		this->m_PixelFormatInfos[Tounderlying(format)].Format = format;
		this->m_PixelFormatInfos[Tounderlying(format)].Platform = platformFormat;
	}

	PlatformPixelFormat operator[](RHIPixelFormat format) const {
		return this->m_PixelFormatInfos[Tounderlying(format)].Platform;
	}

private:
	Array<RHIPixelFormatInfo, static_cast<Uint32>(RHIPixelFormat::COUNT)> m_PixelFormatInfos;

};


export enum class RHITextureFlags :uint32_t
{
	UNKNOWN = 0,
	SHADER_RESOURCE = 1 << 0,
	RENDER_TARGET = 1 << 1,
	DEPTH_STENCIL = 1 << 2,
	UNORDERED_ACCESS = 1 << 3,
	COUNT
};
export constexpr RHITextureFlags operator|(RHITextureFlags lhs, RHITextureFlags rhs) {
	return	static_cast<RHITextureFlags>(Tounderlying(lhs) | Tounderlying(rhs));
}
export constexpr RHITextureFlags& operator|=(RHITextureFlags& lhs, RHITextureFlags rhs) {
	return lhs = lhs | rhs;
}
// NOTE :Type Strong ,enum class can not use this , but c enum can do
//export constexpr RHITextureFlags operator&(RHITextureFlags lhs, RHITextureFlags rhs) {
//	return static_cast<RHITextureFlags>(Tounderlying(lhs) & Tounderlying(rhs));
//}
//export constexpr RHITextureFlags operator&=(RHITextureFlags& lhs, RHITextureFlags rhs) {
//	return lhs = lhs & rhs;
//}
//export constexpr RHITextureFlags operator~(RHITextureFlags lhs) {
//	return static_cast<RHITextureFlags>(~Tounderlying(lhs));
//}

export template<typename APITag = g_CurrentAPI>
class RHITextureFlagsManager final :public Singleton {
	static_assert(requires { typename APITag::TextureFlags; },
		"APITag must contain TextureFlags type");
private:
	using PlatformTextureFlags = typename RHITypeTraits<APITag>::TextureFlags;

	struct RHITextureFlagsInfo final {
		RHITextureFlags Flags;
		PlatformTextureFlags Platform;
	};

private:
	RHITextureFlagsManager(void) = default;
public:
	~RHITextureFlagsManager(void) = default;
public:
	static RHITextureFlagsManager& Get(void) {
		static RHITextureFlagsManager instance {};

		return instance;
	}
	//NOTE : format can not be op ,platformFormat can be op ,make sure a rhiformt reflection to a(some) platform
	void AddTransFormat(RHITextureFlags format, PlatformTextureFlags platformFormat) {
		this->m_TextureFlagsInfos[CountRZero(Tounderlying(format))].Flags = format;
		this->m_TextureFlagsInfos[CountRZero(Tounderlying(format))].Platform = platformFormat;
	}

	//NOTE : format can be op
	PlatformTextureFlags operator[](RHITextureFlags format) const {
		//TODO Be Bitset to use Clear
		Uint32 BitValue { Tounderlying(format) };
		PlatformTextureFlags Result { 0 };

		while (0!=BitValue) {
			Uint32 Bit { CountRZero(BitValue) };
			Result |= this->m_TextureFlagsInfos[Bit].Platform;
			BitValue &= ~Bit;
		}

		return Result;
	}

private:
	//NOTE : CountRZero(Count) is can do 
	Array<RHITextureFlagsInfo, 32> m_TextureFlagsInfos;

};


export union RHIClearValue {
	Array<float, 4> Color;
	struct {
		float Depth;
		Uint8 Stencil;
	} DepthStencil;
};

export class RHIClearValueBulider final {
public:
	RHIClearValueBulider(void) = default;
public:
	RHIClearValueBulider& SetColor(float r, float g, float b, float a) {
		this->m_ClearValue.Color = { r,g,b,a };

		return *this;
	}

	RHIClearValueBulider& SetDepthStencil(float depth, Uint8 stencil) {
		this->m_ClearValue.DepthStencil.Depth = depth;
		this->m_ClearValue.DepthStencil.Stencil = stencil;

		return *this;
	}

	[[nodiscard]] RHIClearValue Build(void) const {
		return this->m_ClearValue;
	}

private:
	RHIClearValue m_ClearValue {};

};

export struct RHIExtent2D final {
	Uint32 Width { 0 };
	Uint32 Height { 0 };
};

export struct RHIExtent3D final {
	Uint32 Width { 0 };
	Uint32 Height { 0 };
	Uint32 Depth { 0 };
};

