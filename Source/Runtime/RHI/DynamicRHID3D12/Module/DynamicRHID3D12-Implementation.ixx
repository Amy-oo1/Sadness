module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"


#include "ThirdParty/Windows/DDS/DDSTextureLoader12.h"

export module DynamicRHID3D12 :Implementation;

import Platform;
import Utility;
import Concurrent;
import Memory;
import Container;
import Logger;

import RHI;
import DynamicRHI;

import D3D12Definition;

import :Forward;
import :Common;
import :Device;
import :Descriptor;
//import :Resource;
import :Fence;
import :GPUNode;
import :SamplerManager;
import :TextureManager;


Device::Device(ComPtr<IDXGIAdapter2> adapter):
	MoveAbleOnly {},
	m_Adapter2 { adapter } {

	D3D12_CHECK(D3D12CreateDevice(this->m_Adapter2.Get(), g_MinFeatureLevelSupport, IID_PPV_ARGS(&this->m_Device4)));

	for (Uint32 Index = 0; auto & EachGPUNode : this->m_GPUNodes)
		EachGPUNode = new  GPUNode { this, RHIGPUMask { Index++ } };
}


const SamplerState& SamplerManager::Create_Sampler(const RHISamplerDesc& Desc, RHIGPUMask InGPUIIndex){
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

ID3D12Resource* TextureManager::CreateGPUResource(const Uint8* Data,Size size, const D3D12_RESOURCE_DESC& Desc, RHIGPUMask InGPUIndex){
	auto Handle { this->m_Device->Get_GPUNode(InGPUIndex)->m_CBVSRVUAVAllocator.Allocate(1) };

	//TODO :Subresource ?
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	ID3D12Resource* Re;
	auto Result { DirectX::LoadDDSTextureFromMemory(this->m_Device->Get_Device(), Data, size,&Re, subresources) };

	return Re;
}

Device::~Device(void){
	for (auto& EachGPUNode : this->m_GPUNodes) {
		delete EachGPUNode;
	}

	//TODO : Remove
	this->m_Adapter2.Reset();
	this->m_Device4.Reset();
}
