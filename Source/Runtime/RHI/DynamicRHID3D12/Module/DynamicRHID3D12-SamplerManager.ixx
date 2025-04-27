module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :SamplerManager;

import Platform;
import Utility;
import Concurrent;
import Container;
import Logger;

import RHI;

import D3D12Definition;

import :Forward;
import :Common;
import :Device;
import :Descriptor;
import :GPUNode;

export Function<Uint64(const D3D12_SAMPLER_DESC&)> SamplerDescHash = [](const D3D12_SAMPLER_DESC& desc) {
	Uint64 hash { 0 };
	{
		HashCombine(hash, desc.Filter);
		HashCombine(hash, desc.AddressU);
		HashCombine(hash, desc.AddressV);
		HashCombine(hash, desc.AddressW);
		HashCombine(hash, static_cast<Uint64>(desc.MipLODBias));
		HashCombine(hash, static_cast<Uint64>(desc.MaxAnisotropy));
		HashCombine(hash, desc.ComparisonFunc);
		for (const auto& color : desc.BorderColor)
			HashCombine(hash, static_cast<Uint64>((color)));
		HashCombine(hash, static_cast<Uint64>((desc.MinLOD)));
		HashCombine(hash, static_cast<Uint64>((desc.MaxLOD)));
	}
	return hash;
	};

export Function<bool(const D3D12_SAMPLER_DESC&, const D3D12_SAMPLER_DESC&)>	SamplerDescEqual = [](const D3D12_SAMPLER_DESC& lhs, const D3D12_SAMPLER_DESC& rhs) {
	return //TODO :Mybew You kan Swap to Compare Faster
		(lhs.Filter == rhs.Filter) &&
		(lhs.AddressU == rhs.AddressU) &&
		(lhs.AddressV == rhs.AddressV) &&
		(lhs.AddressW == rhs.AddressW) &&
		(lhs.MipLODBias == rhs.MipLODBias) &&
		(lhs.MaxAnisotropy == rhs.MaxAnisotropy) &&
		(lhs.ComparisonFunc == rhs.ComparisonFunc) &&
		(lhs.MinLOD == rhs.MinLOD) &&
		(lhs.MaxLOD == rhs.MaxLOD) &&
		(lhs.BorderColor[0] == rhs.BorderColor[0]) &&
		(lhs.BorderColor[1] == rhs.BorderColor[1]) &&
		(lhs.BorderColor[2] == rhs.BorderColor[2]) &&
		(lhs.BorderColor[3] == rhs.BorderColor[3]);
	};

export struct SamplerState final {
	Uint32 m_ID;
	D3D12_SAMPLER_DESC m_Desc {};
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle {};
	RHIGPUMask m_InGPUMask {};
	bool Is_Static = false;
};

export class SamplerManager final : public MoveAbleOnly, public DeviceChild {
public:
	SamplerManager(Device* Parent) :
		MoveAbleOnly {},
		DeviceChild { Parent }{
	}

public:
	~SamplerManager(void) = default;

public:
	[[nodiscard]] SamplerState& Create_Sampler(const RHISamplerDesc& Desc, RHIGPUMask InGPUIIndex);

private:
	Mutex m_SamplerCacheMutex {};
	UnorderedMap<D3D12_SAMPLER_DESC, SamplerState, decltype(SamplerDescHash), decltype(SamplerDescEqual)>
		m_SamplerMap { 0, SamplerDescHash, SamplerDescEqual };

	Uint32 IDAllocator { 0 };
};

SamplerState& SamplerManager::Create_Sampler(const RHISamplerDesc& Desc, RHIGPUMask InGPUIIndex){
	auto  D3D12SamplerDesc { TranslateToD3D12SamplrDesc(Desc) };
	{
		LockGuard<Mutex> Lock { this->m_SamplerCacheMutex };

		if (auto Iter { this->m_SamplerMap.find(D3D12SamplerDesc) }; Iter != this->m_SamplerMap.end())
			return Iter->second;

		else {
			auto Handle { this->m_Device->Get_GPUNode(InGPUIIndex)->m_SamplerAllocator.Allocate(1) };
			this->m_Device->Get_Device()->CreateSampler(&D3D12SamplerDesc, Handle);

			SamplerState NewSamplerState {};
			{
				NewSamplerState.m_ID = this->IDAllocator++;
				NewSamplerState.m_Desc = D3D12SamplerDesc;
				NewSamplerState.m_CPUHandle = Handle;
				NewSamplerState.m_InGPUMask = InGPUIIndex;
			}

			this->m_SamplerMap.emplace(D3D12SamplerDesc, MoveTemp(NewSamplerState));
			return this->m_SamplerMap[D3D12SamplerDesc];
		}
	}
}