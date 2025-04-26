module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :GPUNode;

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

export class GPUNode final : public MoveAbleOnly,public DeviceChild, public SingleNodeGPUObject {
	friend class Device;
public:
	GPUNode(Device* Patrent,RHIGPUMask InGPUMask):
		MoveAbleOnly {},
		DeviceChild { Patrent },
		SingleNodeGPUObject { InGPUMask },
		m_GlobalSamerHeap { new GlobalSamplerHeap{this->m_Device, this->Get_GPUMask()} },
		m_GlobalViewHeap { new GlobalViewHeap{this->m_Device, this->Get_GPUMask()} }
	{}

public:
	~GPUNode(void) {
		delete this->m_GlobalSamerHeap;
		delete this->m_GlobalViewHeap;
	}

	void InitializatiePartGPUNodeCommandListManager(void);

	void DestroyPartGPUNodeCommandListManager(void);

public:
	//TODO Remove Defautl Init
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> m_SamplerAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV> m_CBVSRVUAVAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_RTV> m_RTVAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_DSV> m_DSVAllocator { this->m_Device, this->Get_GPUMask() };

	DescriptorHeapBase<GlobalSamplerHeap>* m_GlobalSamerHeap;
	DescriptorHeapBase<GlobalViewHeap>* m_GlobalViewHeap;

	CommandListManager* m_GraphicsCommandListManager { nullptr };
	CommandListManager* m_ComputeCommandListManager { nullptr };
	CommandListManager* m_CopyCommandListManager { nullptr };

};