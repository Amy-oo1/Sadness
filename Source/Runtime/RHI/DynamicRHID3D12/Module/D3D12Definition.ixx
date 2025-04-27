module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module D3D12Definition;

import Platform;

export	constexpr D3D_FEATURE_LEVEL g_MinFeatureLevelSupport { D3D_FEATURE_LEVEL_12_0 };
export constexpr D3D_ROOT_SIGNATURE_VERSION g_RootSignatureVersion { D3D_ROOT_SIGNATURE_VERSION_1_1 };

//SLI Nv max is 4
export constexpr Uint32 g_MaxGPUCount { 1 };


export constexpr Uint32 g_ZeroBufferSize { 4 * (1 << 20) };

export constexpr Uint32 g_MaxRootParameterCast { 32 };
export constexpr Uint32 g_PerRootDescriptorTableCost { 1 };
export constexpr Uint32 g_PerRootDescriptorCost { 2 };
export constexpr Uint32 g_PerRootC32BitValueCost { 1 };

export constexpr Uint32 g_DefualtBindingSpace { 0 };
export constexpr Uint32 g_MaxRootCBVCount { 16 };//NOTE :choose use Tanble Or 32Bit


export enum class ShaderVisibility :Uint8{//NOTE :SRV+CBV+UVA+Sampler
	Vertex ,
	Pixel ,
	Hull ,
	Domain ,
	Geometry,
	All,
	//NOTE :this enum to ignore d3d12 other visibility such as RayTracing
	Count
};
export constexpr D3D12_SHADER_VISIBILITY T0_D3D12ShaderVisibility(ShaderVisibility InVisibility) {
	switch (InVisibility) {
		using enum ShaderVisibility;
	case Vertex: return D3D12_SHADER_VISIBILITY_VERTEX;
	case Pixel: return D3D12_SHADER_VISIBILITY_PIXEL;
	case Hull: return D3D12_SHADER_VISIBILITY_HULL;
	case Domain: return D3D12_SHADER_VISIBILITY_DOMAIN;
	case Geometry: return D3D12_SHADER_VISIBILITY_GEOMETRY;
	case All: return D3D12_SHADER_VISIBILITY_ALL;
	default:
		ASSERT(false);
		return D3D12_SHADER_VISIBILITY_ALL;
	}
}

export constexpr int g_DescriptorCountPerHeap { 256 };

export constexpr int g_MaxSwapChainBufferCount { 3 };

//TODO Move To WIndows Platform Include
export const HANDLE g_InvalidHandleValue { INVALID_HANDLE_VALUE };

//constexpr D3D12_SHADER_RESOURCE_VIEW_DESC g_DefaultSRVDesc {
//	.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
//	.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
//	.Texture2D {
//	
//	}
//};