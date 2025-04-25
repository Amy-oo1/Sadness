module;
#include "d3d12.h"

export module RHI :SamplerDesc;

import Platform;
import Utility;
import Container;

import :Platform;

//NOTE :Although Vulkan Set More(DX 12) Sampler Filter, But We Only Use 3 Type
//Ref UE4


export enum class RHIFilter :Uint32 {
	Point,
	Bilinear,
	Trilinear,
	Anisotropic_MipPoint,
	Anisotropic_MipLinear,
	//NOTE :You kan Add ,but must Trans to Api
	Count
};

export template<typename APITag = g_CurrentAPI>
class RHISamplerFilterManager final :public Singleton {
	static_assert(requires { typename APITag::Filter; },
		"APITag must contain Filter type");
private:
	using PlatformFilter = typename RHITypeTraits<APITag>::Filter;
	struct RHISamplerFilterInfo final {
		RHIFilter Filter;
		PlatformFilter Platform;
	};

private:
	RHISamplerFilterManager(void) = default;
public:
	~RHISamplerFilterManager(void) = default;

public:
	static RHISamplerFilterManager& Get(void) {
		static RHISamplerFilterManager instance {};

		return instance;
	}

	void AddTransFilter(RHIFilter filter, PlatformFilter platformFilter) {
		this->m_FilterInfos[Tounderlying(filter)].Filter = filter;
		this->m_FilterInfos[Tounderlying(filter)].Platform = platformFilter;
	}

	PlatformFilter operator[](RHIFilter filter) const {
		return this->m_FilterInfos[Tounderlying(filter)].Platform;
	}

private:
	Array<RHISamplerFilterInfo, static_cast<Uint32>(RHIFilter::Count)> m_FilterInfos;

};

export auto& g_FilterTranslate { RHISamplerFilterManager<g_CurrentAPI>::Get() };


export enum class RHIAddressMode :Uint32 {
	Wrap,
	Mirror,
	Clamp,
	Border,
	MirrorOnce,

	Count
};

export template<typename APITag = g_CurrentAPI>
class RHISamplerAddressModeManager final :public Singleton {
	static_assert(requires { typename APITag::AddressMode; },
		"APITag must contain AddressMode type");
private:
	using PlatformAddressMode = typename RHITypeTraits<APITag>::AddressMode;
	struct RHISamplerAddressModeInfo final {
		RHIAddressMode AddressMode;
		PlatformAddressMode Platform;
	};

private:
	RHISamplerAddressModeManager(void) = default;

public:
	~RHISamplerAddressModeManager(void) = default;

public:
	static RHISamplerAddressModeManager& Get(void) {
		static RHISamplerAddressModeManager instance {};

		return instance;
	}

	void AddTransAddressMode(RHIAddressMode addressMode, PlatformAddressMode platformAddressMode) {
		this->m_AddressModeInfos[Tounderlying(addressMode)].AddressMode = addressMode;
		this->m_AddressModeInfos[Tounderlying(addressMode)].Platform = platformAddressMode;
	}
	PlatformAddressMode operator[](RHIAddressMode addressMode) const {
		return this->m_AddressModeInfos[Tounderlying(addressMode)].Platform;
	}

private:
	Array<RHISamplerAddressModeInfo, static_cast<Uint32>(RHIAddressMode::Count)> m_AddressModeInfos;

};

export auto& g_AddressModeTranslate { RHISamplerAddressModeManager<g_CurrentAPI>::Get() };


export enum class RHISamplerCompareFunc :Uint32 {
	NEVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS,
	//NOTE :this func Vulkan And DirecX is Same
	Count
};

export template<typename APITag = g_CurrentAPI>
class RHISamplerCompareFuncManager final :public Singleton {
	static_assert(requires { typename APITag::CompareFunc; },
		"APITag must contain CompareFunc type");
private:
	using PlatformCompareFunc = typename RHITypeTraits<APITag>::CompareFunc;
	struct RHISamplerCompareFuncInfo final {
		RHISamplerCompareFunc CompareFunc;
		PlatformCompareFunc PlatForm;
	};

private:
	RHISamplerCompareFuncManager(void) = default;

public:
	~RHISamplerCompareFuncManager(void) = default;

public:
	static RHISamplerCompareFuncManager& Get(void) {
		static RHISamplerCompareFuncManager instance {};

		return instance;
	}

	void AddTransCompareFunc(RHISamplerCompareFunc compareFunc, PlatformCompareFunc platformCompareFunc) {
		this->m_CompareFuncInfos[Tounderlying(compareFunc)].CompareFunc = compareFunc;
		this->m_CompareFuncInfos[Tounderlying(compareFunc)].PlatForm = platformCompareFunc;
	}
	PlatformCompareFunc operator[](RHISamplerCompareFunc compareFunc) const {
		return this->m_CompareFuncInfos[Tounderlying(compareFunc)].PlatForm;
	}
private:
	Array<RHISamplerCompareFuncInfo, static_cast<Uint32>(RHISamplerCompareFunc::Count)> m_CompareFuncInfos;

};

export auto& g_CompareFuncTranslate { RHISamplerCompareFuncManager<g_CurrentAPI>::Get() };


export struct RHISamplerDesc final{
	RHIFilter Filter { RHIFilter::Point };

	RHIAddressMode AddressU { RHIAddressMode::Wrap };
	RHIAddressMode AddressV { RHIAddressMode::Wrap };
	RHIAddressMode AddressW { RHIAddressMode::Wrap};
	
	RHISamplerCompareFunc CompareFunc { RHISamplerCompareFunc::NEVER };
	float MipBias { 0.f };

	float MinMipLevel { 0.f };
	float MaxMipLevel { 0.f };
	
	Int32 MaxAnisotropy { 0 };
	
	Array<float, 4> BorderColors { 0.f,0.f,0.f,0.f };

	//NOTE : Vulkan Can Unorderstand UV,but must be set
};


class RHISamplerDescBuilder final {
public:
	RHISamplerDescBuilder(void) = default;
public:
	RHISamplerDescBuilder& SetFilter(RHIFilter filter) {
		this->desc.Filter = filter;
		return *this;
	}
	RHISamplerDescBuilder& SetAddressU(RHIAddressMode addressU) {
		this->desc.AddressU = addressU;
		return *this;
	}
	RHISamplerDescBuilder& SetAddressV(RHIAddressMode addressV) {
		this->desc.AddressV = addressV;
		return *this;
	}
	RHISamplerDescBuilder& SetAddressW(RHIAddressMode addressW) {
		this->desc.AddressW = addressW;
		return *this;
	}
	RHISamplerDescBuilder& SetCompareFunc(RHISamplerCompareFunc compareFunc) {
		this->desc.CompareFunc = compareFunc;
		return *this;
	}
	RHISamplerDescBuilder& SetMipBias(float mipBias) {
		this->desc.MipBias = mipBias;
		return *this;
	}
	RHISamplerDescBuilder& SetMinMipLevel(float minMipLevel) {
		this->desc.MinMipLevel = minMipLevel;
		return *this;
	}
	RHISamplerDescBuilder& SetMaxMipLevel(float maxMipLevel) {
		this->desc.MaxMipLevel = maxMipLevel;
		return *this;
	}
	RHISamplerDescBuilder& SetMaxAnisotropy(Int32 maxAnisotropy) {
		this->desc.MaxAnisotropy = maxAnisotropy;
		return *this;
	}
	RHISamplerDescBuilder& SetBorderColors(const Array<float, 4>& borderColors) {
		this->desc.BorderColors = borderColors;
		return *this;
	}

	RHISamplerDescBuilder& SetAddressUVW(RHIAddressMode Address) {
		this->desc.AddressU = Address;
		this->desc.AddressV = Address;
		this->desc.AddressW = Address;
		return *this;
	}
	RHISamplerDescBuilder& SetMipLevel(float MinMipLevel, float MaxMipLevel) {
		this->desc.MinMipLevel = MinMipLevel;
		this->desc.MaxMipLevel = MaxMipLevel;
		return *this;
	}
	RHISamplerDescBuilder& SetBorderColors(float R, float G, float B, float A) {
		this->desc.BorderColors[0] = R;
		this->desc.BorderColors[1] = G;
		this->desc.BorderColors[2] = B;
		this->desc.BorderColors[3] = A;
		return *this;
	}
	RHISamplerDescBuilder& SetBorderColor(float BorderColor) {
		this->desc.BorderColors[0] = BorderColor;
		this->desc.BorderColors[1] = BorderColor;
		this->desc.BorderColors[2] = BorderColor;
		this->desc.BorderColors[3] = BorderColor;
		return *this;
	}

	[[nodiscard]] RHISamplerDesc Build(void) const {
		return this->desc;
	}

	RHISamplerDescBuilder& Reset(void) {
		this->desc = RHISamplerDesc {};
		return *this;
	}

private:
	RHISamplerDesc desc {};
};


export [[nodiscard]] inline auto TranslateToD3D12SamplrDesc(const RHISamplerDesc& Desc) {
	D3D12_SAMPLER_DESC D3D12SamplerDesc {};
	{
		D3D12SamplerDesc.Filter = g_FilterTranslate[Desc.Filter];

		D3D12SamplerDesc.AddressU = g_AddressModeTranslate[Desc.AddressU];
		D3D12SamplerDesc.AddressV = g_AddressModeTranslate[Desc.AddressV];
		D3D12SamplerDesc.AddressW = g_AddressModeTranslate[Desc.AddressW];

		D3D12SamplerDesc.MipLODBias = Desc.MipBias;
		D3D12SamplerDesc.MaxAnisotropy = Desc.MaxAnisotropy;

		D3D12SamplerDesc.ComparisonFunc = g_CompareFuncTranslate[Desc.CompareFunc];

		D3D12SamplerDesc.BorderColor[0] = Desc.BorderColors[0];
		D3D12SamplerDesc.BorderColor[1] = Desc.BorderColors[1];
		D3D12SamplerDesc.BorderColor[2] = Desc.BorderColors[2];
		D3D12SamplerDesc.BorderColor[3] = Desc.BorderColors[3];

		D3D12SamplerDesc.MinLOD = Desc.MinMipLevel;
		D3D12SamplerDesc.MaxLOD = Desc.MaxMipLevel;
	}

	return D3D12SamplerDesc;
}