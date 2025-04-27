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

//import :Forward;
import :Common;
import :Device;
import :Descriptor;
//import :Resource;
import :Fence;
import :GPUNode;
import :RootSignatureManager;
import :PipelineState;//TODO Cache?
import :CommandList;
import :CommandListManager;
import :SamplerManager;
import :TextureManager;



void GPUNode::InitializatiePartGPUNodeCommandListManager(void){
	this->m_GraphicsCommandListManager = new CommandListManager { this->m_Device, this->Get_GPUMask(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT };
	this->m_ComputeCommandListManager = new CommandListManager { this->m_Device, this->Get_GPUMask(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE };
	this->m_CopyCommandListManager = new CommandListManager { this->m_Device, this->Get_GPUMask(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY };
}

void GPUNode::DestroyPartGPUNodeCommandListManager(void){
	if (this->m_GraphicsCommandListManager) {
		delete this->m_GraphicsCommandListManager;
		this->m_GraphicsCommandListManager = nullptr;
	}
	if (this->m_ComputeCommandListManager) {
		delete this->m_ComputeCommandListManager;
		this->m_ComputeCommandListManager = nullptr;
	}
	if (this->m_CopyCommandListManager) {
		delete this->m_CopyCommandListManager;
		this->m_CopyCommandListManager = nullptr;
	}
}




void Device::InitializatiePartFence(void){
	this->m_FenceCorePool = new FenceCorePool { this };
	this->m_FrameFence = new FenceManual { this };
	this->m_StagingFence = new FenceIncrement { this };
}

void Device::DestroyPartFence(void){
	if (this->m_FrameFence) {
		delete this->m_FrameFence;
		this->m_FrameFence = nullptr;
	}

	if (this->m_StagingFence) {
		delete this->m_StagingFence;
		this->m_StagingFence = nullptr;
	}

	if (this->m_FenceCorePool) {
		delete this->m_FenceCorePool;
		this->m_FenceCorePool = nullptr;
	}
}

void Device::InitializatiePartCommandListManager(void){
	for(auto& GPUNode :this->m_GPUNodes)
		GPUNode->InitializatiePartGPUNodeCommandListManager();
}

void Device::DestroyPartCommandListManager(void){
	for (auto& GPUNode : this->m_GPUNodes)
		GPUNode->DestroyPartGPUNodeCommandListManager();
}

void Device::InitializatiePartSamplerManager(void){
	this->m_SamplerManager = new SamplerManager { this };
}

void Device::DestroyPartSamplerManager(void){
	delete this->m_SamplerManager;
}

void Device::CreateStaticSampler(const RHISamplerDesc& Desc){
	auto& State { this->m_SamplerManager->Create_Sampler(Desc, this->Get_ALLNode()) };//TODO :

	State.Is_Static = true;
}

void Device::CreateTexture2D(const RHITextrure2DDesc& Desc, Uint8* data){

}

void Device::InitializatiePartGPUNode(void){
	for (Uint32 Index = 0; auto & EachGPUNode : this->m_GPUNodes)
		EachGPUNode = new  GPUNode { this, RHIGPUMask { 1u << (Index++)} };
}

void Device::DestroyPartGPUNode(void){
	for (auto& EachGPUNode : this->m_GPUNodes)
		delete EachGPUNode;
}


Device::Device(ComPtr<IDXGIAdapter2> adapter):
	MoveAbleOnly {},
	m_Adapter2 { adapter } {

	D3D12_CHECK(D3D12CreateDevice(this->m_Adapter2.Get(), g_MinFeatureLevelSupport, IID_PPV_ARGS(&this->m_Device4)));

	this->m_Device4->SetStablePowerState(true);

	D3D12_COMMAND_SIGNATURE_DESC commandSignatureDesc = {};
	commandSignatureDesc.NumArgumentDescs = 1;
	commandSignatureDesc.ByteStride = 20;
	commandSignatureDesc.NodeMask = this->Get_ALLNode().Get_Native();

	D3D12_INDIRECT_ARGUMENT_DESC indirectParameterDesc[1] = {};
	commandSignatureDesc.pArgumentDescs = indirectParameterDesc;

	indirectParameterDesc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
	commandSignatureDesc.ByteStride = sizeof(D3D12_DRAW_ARGUMENTS);

	D3D12_CHECK(this->m_Device4->CreateCommandSignature(
		&commandSignatureDesc,
		nullptr,
		IID_PPV_ARGS(&this->m_DrawIndirectCommandSignature)));

	indirectParameterDesc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
	commandSignatureDesc.ByteStride = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);

	D3D12_CHECK(this->m_Device4->CreateCommandSignature(
		&commandSignatureDesc,
		nullptr,
		IID_PPV_ARGS(&this->m_DrawIndexedIndirectCommandSignature)));

	indirectParameterDesc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
	commandSignatureDesc.ByteStride = sizeof(D3D12_DISPATCH_ARGUMENTS);

	D3D12_CHECK(this->m_Device4->CreateCommandSignature(
		&commandSignatureDesc,
		nullptr,
		IID_PPV_ARGS(&this->m_DispatchIndirectCommandSignature)));

	this->InitializatiePartGPUNode();
	this->InitializatiePartFence();
	this->InitializatiePartCommandListManager();
	this->InitializatiePartSamplerManager();


	this->m_ConstantBufferPageProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
}


//const SamplerState& SamplerManager::Create_Sampler(const RHISamplerDesc& Desc, RHIGPUMask InGPUIIndex){
//	auto  D3D12SamplerDesc { TranslateToD3D12SamplrDesc(Desc) };
//
//	{
//		LockGuard<Mutex> Lock { this->m_SamplerCacheMutex };
//
//		if (auto Iter { this->m_SamplerMap.find(D3D12SamplerDesc) }; Iter != this->m_SamplerMap.end())
//			return Iter->second;
//
//		else {
//			auto Handle { this->m_Device->Get_GPUNode(InGPUIIndex)->m_SamplerAllocator.Allocate(1) };
//			this->m_Device->Get_Device()->CreateSampler(&D3D12SamplerDesc, Handle);
//
//			SamplerState NewSamplerState {};
//			{
//				NewSamplerState.m_ID = this->IDAllocator++;
//				NewSamplerState.m_Desc = D3D12SamplerDesc;
//				NewSamplerState.m_CPUHandle = Handle;
//				NewSamplerState.m_InGPUMask = InGPUIIndex;
//			}
//
//			this->m_SamplerMap.emplace(D3D12SamplerDesc, MoveTemp(NewSamplerState));
//			return this->m_SamplerMap[D3D12SamplerDesc];
//		}
//	}
//}

ID3D12Resource* TextureManager::CreateGPUResource(const Uint8* Data,Size size, const D3D12_RESOURCE_DESC& Desc, RHIGPUMask InGPUIndex){
	auto Handle { this->m_Device->Get_GPUNode(InGPUIndex)->m_CBVSRVUAVAllocator.Allocate(1) };

	//TODO :Subresource ?
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	ID3D12Resource* Re;
	auto Result { DirectX::LoadDDSTextureFromMemory(this->m_Device->Get_Device(), Data, size,&Re, subresources) };

	return Re;
}

Device::~Device(void) {
	this->DestroyPartSamplerManager();
	this->DestroyPartCommandListManager();
	this->DestroyPartFence();
	this->DestroyPartGPUNode();
	

	//TODO : Remove
	this->m_Adapter2.Reset();
	this->m_Device4.Reset();
}