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
		SingleNodeGPUObject { InGPUMask }
	{}

public:
	~GPUNode(void) {
		//TODO : this class is controlled by Device
	}

	void InitializatiePartGPUNodeCommandListManager(void);

	void DestroyPartGPUNodeCommandListManager(void);

public:
	//TODO Remove Defautl Init
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> m_SamplerAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV> m_CBVSRVUAVAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_RTV> m_RTVAllocator { this->m_Device, this->Get_GPUMask() };
	DescriptorAllocator<D3D12_DESCRIPTOR_HEAP_TYPE_DSV> m_DSVAllocator { this->m_Device, this->Get_GPUMask() };

	GlobalSamplerHeap m_GlobalSamerHeap { this->m_Device, this->Get_GPUMask() };
	GlobalViewHeap m_GlobalViewHeap { this->m_Device, this->Get_GPUMask() };

	CommandListManager* m_GraphicsCommandListManager { nullptr };
	CommandListManager* m_ComputeCommandListManager { nullptr };
	CommandListManager* m_CopyCommandListManager { nullptr };

};