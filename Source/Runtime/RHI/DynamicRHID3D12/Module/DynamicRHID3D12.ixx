module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12;

import Platform;
import Utility;
import Container;
import Memory;
import Logger;

import RHI;
import DynamicRHI;

import D3D12Definition;

export import :Forward;
export import :Common;
export import :Device;
export import :Descriptor;
//export import :TextureManager;
export import :LinearAllocation;
export import :Resource;
export import :Fence;
export import :GPUNode;
export import :RootSignatureManager;
export import :PipelineState;//TODO Cache?
export import :CommandList;
export import :CommandListManager;
export import :SamplerManager;
export import :TextureManager;
export import :StateCache;
export import :CommandContext;
export import :Implementation;



export class DynamicRHID3D12 final: public DynamicRHI<DynamicRHID3D12> {
	friend class DynamicRHI<DynamicRHID3D12>;
private:
	DynamicRHID3D12(void);

public:
	~DynamicRHID3D12(void) = default;

	//NOTE : Singleton class First Return a Ref, unless it is a dynamic class
	static DynamicRHID3D12* Get(void) {
		static DynamicRHID3D12 instance{};

		return &instance;
	}

public:
	void FindAdapterAndCreateDevices(void);


private:
	//TODO Comple Resource Is Out !!!
	RHIPixelFormatManager<D3D12Tag>& m_PixelFormatTranslate;
	RHISamplerFilterManager<D3D12Tag>& m_SamplerFilterTranslate;
	RHISamplerCompareFuncManager<D3D12Tag>& m_SamplerCompareFuncTranslate;
	RHISamplerAddressModeManager<D3D12Tag>& m_SamplerAddressModeTranslate;

	//TODO : Becaule Init be split ,so i defualt cont them

private:
	void TranslatePixelFormat(void);
	void TranslateSamplerFilter(void);
	void TranslateSamplerCompareFunc(void);
	void TranslateSamplerAddressMode(void);

private:
	

private:
	void Imp_RHIPerInitialize(void);

	void Imp_RHIPostInitialize(void);

	void Imp_RHICreateStaticSampler(const RHISamplerDesc& StateSampler);

	void Imp_RHICreateTextur2D(const RHITextrure2DDesc& Desc, Uint8* pdata);


private:



private:

	ComPtr<IDXGIFactory6> m_IDXGIFactory {};

	Vector<SharedPtr<Device>> m_Devices;

private:

// Allocate a buffer of zeroes. This is used when we need to pass D3D memory
// that we don't care about and will overwrite with valid data in the future.
	MonotonicBufferResource m_ZeroBuffer { g_ZeroBufferSize, Get_DefaultResource() };



};



DynamicRHID3D12::DynamicRHID3D12(void):
	DynamicRHI<DynamicRHID3D12> {},
	m_PixelFormatTranslate { RHIPixelFormatManager<D3D12Tag>::Get() },
	m_SamplerFilterTranslate { RHISamplerFilterManager<D3D12Tag>::Get() },
	m_SamplerAddressModeTranslate { RHISamplerAddressModeManager<D3D12Tag>::Get() },
	m_SamplerCompareFuncTranslate { RHISamplerCompareFuncManager<D3D12Tag>::Get() } {


	this->FindAdapterAndCreateDevices();

	this->TranslatePixelFormat();
	this->TranslateSamplerFilter();
	this->TranslateSamplerCompareFunc();
	this->TranslateSamplerAddressMode();
}

void DynamicRHID3D12::FindAdapterAndCreateDevices(void){

	Uint32 nDXGIFactoryFlags = 0;
	
	//NOTE :Debug
	{
#ifdef _DEBUG
		ComPtr<ID3D12Debug> pID3D12Debug = nullptr;
		D3D12_CHECK(D3D12GetDebugInterface(IID_PPV_ARGS(&pID3D12Debug)));
		pID3D12Debug->EnableDebugLayer();
		nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG
	}

	CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&this->m_IDXGIFactory));

	//TODO : Just One Adapter
	{
		ComPtr<IDXGIAdapter2> pAdapter;
		D3D12_CHECK(this->m_IDXGIFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapter)));
		this->m_Devices.emplace_back(MakeShared<Device>(pAdapter));
	}

}

//TODO Mybe thsi can use to Compile Time
void DynamicRHID3D12::TranslatePixelFormat(void){
	using enum RHIPixelFormat;
	this->m_PixelFormatTranslate.AddTransFormat(R8G8B8A8_UNORM,				DXGI_FORMAT_R8G8B8A8_UNORM);
	this->m_PixelFormatTranslate.AddTransFormat(R8G8B8A8_UNORM_SRGB,		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	this->m_PixelFormatTranslate.AddTransFormat(R8G8B8A8_UINT,				DXGI_FORMAT_R8G8B8A8_UINT);
	this->m_PixelFormatTranslate.AddTransFormat(R8G8B8A8_SNORM,				DXGI_FORMAT_R8G8B8A8_SNORM);
}

void DynamicRHID3D12::TranslateSamplerFilter(void){
	using enum RHIFilter;
	this->m_SamplerFilterTranslate.AddTransFilter(Point,					D3D12_FILTER_MIN_MAG_MIP_POINT);
	this->m_SamplerFilterTranslate.AddTransFilter(ComparsionPoint,			D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT);//TODO  :
	this->m_SamplerFilterTranslate.AddTransFilter(Linear, 					D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	this->m_SamplerFilterTranslate.AddTransFilter(Bilinear,					D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	this->m_SamplerFilterTranslate.AddTransFilter(Trilinear,				D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	this->m_SamplerFilterTranslate.AddTransFilter(Anisotropic,				D3D12_FILTER_ANISOTROPIC);
}

void DynamicRHID3D12::TranslateSamplerCompareFunc(void){
	using enum RHISamplerCompareFunc;
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(NEVER,			D3D12_COMPARISON_FUNC_NEVER);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(LESS,			D3D12_COMPARISON_FUNC_LESS);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(EQUAL,			D3D12_COMPARISON_FUNC_EQUAL);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(LESS_EQUAL,		D3D12_COMPARISON_FUNC_LESS_EQUAL);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(GREATER,		D3D12_COMPARISON_FUNC_GREATER);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(NOT_EQUAL,		D3D12_COMPARISON_FUNC_NOT_EQUAL);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(GREATER_EQUAL,	D3D12_COMPARISON_FUNC_GREATER_EQUAL);
	this->m_SamplerCompareFuncTranslate.AddTransCompareFunc(ALWAYS,			D3D12_COMPARISON_FUNC_ALWAYS);
}

void DynamicRHID3D12::TranslateSamplerAddressMode(void){
	using enum RHIAddressMode;
	this->m_SamplerAddressModeTranslate.AddTransAddressMode(Wrap,			D3D12_TEXTURE_ADDRESS_MODE_WRAP);
	this->m_SamplerAddressModeTranslate.AddTransAddressMode(Mirror,			D3D12_TEXTURE_ADDRESS_MODE_MIRROR);
	this->m_SamplerAddressModeTranslate.AddTransAddressMode(Clamp,			D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	this->m_SamplerAddressModeTranslate.AddTransAddressMode(Border,			D3D12_TEXTURE_ADDRESS_MODE_BORDER);
	this->m_SamplerAddressModeTranslate.AddTransAddressMode(MirrorOnce,		D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE);
}

void DynamicRHID3D12::Imp_RHIPerInitialize(void)
{
}

void DynamicRHID3D12::Imp_RHIPostInitialize(void)
{
}

void DynamicRHID3D12::Imp_RHICreateStaticSampler(const RHISamplerDesc& StateSampler){
	this->m_Devices.front()->CreateStaticSampler(StateSampler);
}

void DynamicRHID3D12::Imp_RHICreateTextur2D(const RHITextrure2DDesc& Desc, Uint8* pdata){
	//this->m_Devices.front()-
}

