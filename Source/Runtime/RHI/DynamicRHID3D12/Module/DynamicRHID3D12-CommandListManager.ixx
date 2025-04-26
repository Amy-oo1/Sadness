module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :CommandListManager;

import Platform;
import Utility;
import Concurrent;
import String;
import Container;
import Logger;

import RHI;
import DynamicRHI;

import D3D12Definition;

import :Forward;
import :Common;
import :Device;
import :Descriptor;
import :Fence;
import :GPUNode;
import :RootSignatureManager;

import :CommandList;


export class CommandListManager final : public MoveAbleOnly, public DeviceChild ,public SingleNodeGPUObject {
public:
	CommandListManager(Device* Parent, RHIGPUMask GPUIndex, D3D12_COMMAND_LIST_TYPE CommandType) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		SingleNodeGPUObject { GPUIndex },
		m_Fence { Parent } {

		this->m_Fence.DeferredInitializate();

		D3D12_COMMAND_QUEUE_DESC QueueDesc {
			.Type = CommandType,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = GPUIndex.Get_Native(),
		};

		this->m_Device->Get_Device()->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&this->m_Queue));


	}

	~CommandListManager(void) = default;//TODO :

	void Create(Uint32 NUm = 1);//Default Create

	SharedPtr<CommandList> Get_CommanhdList(CommandAllocator* InCommandAllocator) {
		if (this->m_ReadyLists.Is_Empty())
			return MakeShared<CommandList>(this->m_Device, this->Get_GPUMask(), this->m_Type, InCommandAllocator, this);
		else {
			auto Re { this->m_ReadyLists.DeQueue() };

			return Re;
		}
	}



public:

private:



private:

	D3D12_COMMAND_LIST_TYPE m_Type;
	ComPtr<ID3D12CommandQueue> m_Queue { nullptr };

	FenceCommandList m_Fence;

	Queue_ThreadSafe<SharedPtr<CommandList>> m_ReadyLists {};

};


void CommandListManager::Create(Uint32 NUm){
	CommandAllocator TempCommandAllocator { this->m_Device ,this->m_Type };
	for (Uint32 Index = 0; Index < NUm; ++Index)
		this->m_ReadyLists.EnQueue(MakeShared<CommandList>(this->m_Device, this->Get_GPUMask(), this->m_Type, &TempCommandAllocator, this));	
}