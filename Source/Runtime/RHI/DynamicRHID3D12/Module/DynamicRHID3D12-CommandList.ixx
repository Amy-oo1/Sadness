module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :CommandList;

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


export class CommandAllocator final : public MoveAbleOnly, public DeviceChild {
public:
	CommandAllocator(Device* Parent, D3D12_COMMAND_LIST_TYPE CommandType, Optional<SyncPoint<FenceManual>> Sync = NullOpt) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		m_SyncPoint { Sync } {
		this->m_Device->Get_Device()->CreateCommandAllocator(CommandType, IID_PPV_ARGS(&this->m_CommandAllocator));
	}

	ID3D12CommandAllocator* Get_CommandAllocator(void) { return this->m_CommandAllocator.Get(); }

	~CommandAllocator(void) = default;//TODO

public:


	bool Is_Ready(void) { //TODO :add Check
		return
			(0 == this->m_PendingCommandListCount) &&
			this->m_SyncPoint->Is_Compelete();
	}

	void Reset(void) { this->m_CommandAllocator->Reset(); }

	void IncrementPendingCommandLists(void) {
		++this->m_PendingCommandListCount;
	}

private:
	ComPtr<ID3D12CommandAllocator> m_CommandAllocator { nullptr };
	Atomic<Uint32> m_PendingCommandListCount { 0 };
	Optional<SyncPoint<FenceManual>> m_SyncPoint;

};

export class CommandAllocatorManager final : public MoveAbleOnly, public DeviceChild {
public:
	CommandAllocatorManager(Device* Parent, D3D12_COMMAND_LIST_TYPE CommandType) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		m_CommandType { CommandType } {
	}
	~CommandAllocatorManager(void) = default;

public:
	CommandAllocator* Get_CommandAllocator(void) {
		{
			LockGuard<Mutex> Lock { this->m_Mutex };

			if ((!this->m_ReadyAllocators.empty()) && (this->m_ReadyAllocators.front()->Is_Ready())) {
				auto Re { this->m_ReadyAllocators.front() };
				this->m_ReadyAllocators.pop();

				Re->Reset();
				return Re;
			}
			else {
				m_Allocators.emplace_back(new CommandAllocator
					{ this->m_Device, this->m_CommandType,MakeOptional<SyncPoint<FenceManual>>(SyncPoint<FenceManual>{this->m_Device->Get_FrameFence()}) });

				return m_Allocators.back();
			}
		}
	}

	void ReleaseCommandAllocator(CommandAllocator* InCommandAllocator) {
		{
			LockGuard<Mutex> Lock { this->m_Mutex };

			this->m_ReadyAllocators.push(InCommandAllocator);
		}
	}

private:
	const D3D12_COMMAND_LIST_TYPE m_CommandType;

	Mutex m_Mutex {};
	Vector<CommandAllocator*> m_Allocators {};
	Queue<CommandAllocator*> m_ReadyAllocators {};
};


//NOTE :ID Zero Is  Fild
Atomic<Uint64> g_CommandIDGenerator { 0 };

export class CommandList final : public MoveAbleOnly, public DeviceChild ,SingleNodeGPUObject{
	using GenerationSyncPointPair = Pair<Uint64, SyncPoint<FenceCommandList>>;

public:
public:
	CommandList(Device* Parent, RHIGPUMask InGPUIndex, D3D12_COMMAND_LIST_TYPE CommandType, CommandAllocator* InCommandAllocator, CommandListManager* InManager) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUIndex },
		m_ID { ++g_CommandIDGenerator },
		m_InManager { InManager } {

		this->m_Device->Get_Device()->CreateCommandList(
			this->Get_GPUMask().Get_Native(),
			CommandType,
			InCommandAllocator->Get_CommandAllocator(),
			nullptr,//TODO ?
			IID_PPV_ARGS(&this->m_CommandList));

		this->Close();
	}

	~CommandList(void) = default;

public:
	void Close(void) {
		this->m_CommandList->Close();
		//TODO ?
	}

	void Reset(CommandAllocator* NewAllocator) {
		this->m_CommandList->Reset(NewAllocator->Get_CommandAllocator(), nullptr);

		this->m_InCommandAllocator = NewAllocator;
		this->m_Is_Closed = false;
		
		this->CleanupActiveGenerations();
		

	}

private:

	void CleanupActiveGenerations(void) {
		{
			LockGuard<Mutex> Lock { this->m_Mutex };
			while ((!this->m_ActiveGenerations.empty())&&this->m_ActiveGenerations.front().second.Is_Compelete()) {
				auto& [Generation, SyncPoint] { this->m_ActiveGenerations.front() };
				this->m_ActiveGenerations.pop();

				this->m_LastCompleteGeneration = Generation;
			}
		}
	}



private:

	ComPtr<ID3D12GraphicsCommandList> m_CommandList { nullptr };
	const Uint64 m_ID;


	CommandListManager* m_InManager { nullptr };

	CommandAllocator* m_InCommandAllocator { nullptr };

	bool m_Is_Closed { true };

	Uint64 m_LastCompleteGeneration { 0 };

	Mutex m_Mutex {};
	Queue<GenerationSyncPointPair> m_ActiveGenerations {};

	
};