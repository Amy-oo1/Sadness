module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12:Fence;

import Platform;
import Utility;
import Concurrent;
import Math;
import String;
import Container;
import Logger;

import RHI;

import D3D12Definition;

import :Forward;
import :Common;
import :Device;
import :Descriptor;

//NOTE :D3d12 Fence can be use any GPU Node
//NOTE  Fence Count in Api has Limitations So i use a fence pool
//TODO :this class not should export
export class FenceCore final : public MoveAbleOnly, public DeviceChild {
	friend class FenceCorePool;
	friend class FenceIncrement;
	friend class FenceManual;
	friend class FenceCommandList;
public:
	FenceCore(Device* Parent, Uint64 InitalValue) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		m_FenceValueAvailableAt { InitalValue },
		m_FenceCompleteEvent { CreateEventW(nullptr, false, false, nullptr) } {

		D3D12_CHECK(Parent->Get_Device()->CreateFence(InitalValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_Fence)));
	}

	~FenceCore(void) {
		if (g_InvalidHandleValue!=this->m_FenceCompleteEvent ) {
			ASSERT(CloseHandle(this->m_FenceCompleteEvent));
			this->m_FenceCompleteEvent = g_InvalidHandleValue;
		}
	}

public:
	[[nodiscard]] ID3D12Fence* Get_Fence(void) const {
		return this->m_Fence.Get();
	}

	[[nodiscard]] HANDLE Get_FenceCompleteEvent(void) const {
		return this->m_FenceCompleteEvent;
	}

	[[nodiscard]] bool Is_FenceValueAvailable(void) const {
		return this->m_FenceValueAvailableAt <= this->m_Fence->GetCompletedValue();
	}

private:
	Uint64 m_FenceValueAvailableAt;
	HANDLE m_FenceCompleteEvent{ g_InvalidHandleValue };//NOTE : this just a tops

	ComPtr<ID3D12Fence> m_Fence{ nullptr };
};

//NOTE : Fence Resource cand use newdelete
export class FenceCorePool final : public MoveAbleOnly, public DeviceChild {
public:
	FenceCorePool(Device* Parent) :
		MoveAbleOnly {},
		DeviceChild { Parent }
	{}

	~FenceCorePool(void) {
			while (!this->m_AvailableFences.Is_Empty())
				delete this->m_AvailableFences.DeQueue().value();
	}

public:
	FenceCore* Get_FenceCore(void) {
		if (!this->m_AvailableFences.Is_Empty() && this->m_AvailableFences.Peek().value()->Is_FenceValueAvailable())
			return this->m_AvailableFences.DeQueue().value();

		return new FenceCore { this->m_Device, 0};
	}

	void ReleaseFenceCore(FenceCore* InFenceCore,Uint64 CurrentFenceValue) {

		InFenceCore->m_FenceValueAvailableAt= CurrentFenceValue;
		this->m_AvailableFences.EnQueue(InFenceCore);
	}

private:
	//NOTE : Use Defual Allocator
	Queue_SPMC<FenceCore*> m_AvailableFences {};

};

// TODO : base fun is empty ,mybe can has shared part
export template<typename Derived>
class FenceBase : public MoveAbleOnly, public DeviceChild {
public:
	FenceBase(Device* Parent):
		MoveAbleOnly {},
		DeviceChild { Parent }
	{}

	//TODO : =0 Or Base 
	void DeferredInitializate(void) {
		this->Get_Derived()->Imp_DeferredInitializate();
	}

	[[nodiscard]] Uint64 Get_CurrentFence(void) {
		this->Get_Derived()->Imp_Get_CurrentFence();
	}

	[[nodiscard]] Uint64 Signal(D3D12_COMMAND_LIST_TYPE InQueueType) {
		this->Get_Derived()->Imp_Signal();
	}

	[[nodiscard]] bool Is_FenceComplete(Uint64 FenceValue) {
		if (FenceValue <= this->m_LastCompletedFenceValue)
			return true;

		return FenceValue <= this->UpdateCompletedFenceValue();
	}

	virtual ~FenceBase(void) = default;//TODO :

	void WaitForFence(Uint64 FenceValue);

	void InternalSignal(D3D12_COMMAND_LIST_TYPE InQueueType, Uint64 FenceToSignal) {
		this->m_Device->Get_CommandQueue(InQueueType)->Signal(this->m_FenceCore->Get_Fence(), FenceToSignal);

		this->m_LastSignaledFenceVaule = FenceToSignal;
	}

	Uint64 UpdateCompletedFenceValue(void) {
		return this->m_LastCompletedFenceValue = Math::Max(
			this->m_LastCompletedFenceValue,
			this->m_FenceCore->Get_Fence()->GetCompletedValue()
		);
	}

protected:


protected:

	void Get_FenceCore(void) {
		if (this->m_FenceCore)
			return;

		this->m_FenceCore = this->m_Device->Get_FenceCorePool()->Get_FenceCore();
	}


protected:
	Uint64 m_CurrentFenceValue { 0 };
	Uint64 m_LastSignaledFenceVaule { 0 };
	Uint64 m_LastCompletedFenceValue { 0 };

	FenceCore* m_FenceCore { nullptr };

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }




private:
	//NOTE :this Func To Debug

	void Imp_DeferredInitializate(void) {
		LOG_ERROR("FenceBase::Imp_DeferredInitializate() not implemented");
	}

	Uint64 Imp_Get_CurrentFence(void) {
		LOG_ERROR("FenceBase::Imp_GetCurrentFence() not implemented");

		return 0;
	}

	Uint64 Imp_Signal(void) {
		LOG_ERROR("FenceBase::Imp_Signal() not implemented");
		return 0;
	}

};


export class FenceIncrement final : public FenceBase<FenceIncrement> {
	friend class FenceBase<FenceIncrement>;
public:
	FenceIncrement(Device* Parent) :
		FenceBase<FenceIncrement> { Parent }
	{}


private:
	void Imp_DeferredInitializate(void) {
		this->Get_FenceCore();

		this->m_LastSignaledFenceVaule = 0;
		this->m_LastCompletedFenceValue = this->m_FenceCore->m_FenceValueAvailableAt;
		this->m_CurrentFenceValue = this->m_LastCompletedFenceValue + 1;
	
	}

	Uint64 Imp_Get_CurrentFence(void) { return this->m_CurrentFenceValue; }

	Uint64 Imp_Signal(D3D12_COMMAND_LIST_TYPE InQueueType) {
		this->InternalSignal(InQueueType, this->m_CurrentFenceValue);
	
	
	}
};

export class FenceManual final : public FenceBase<FenceManual> {
	friend class FenceBase<FenceManual>;
public:
	FenceManual(Device* Parent) :
		FenceBase<FenceManual> { Parent }
	{}


private:
	//TODO :IMPI
	void Imp_DeferredInitializate(void) {
		this->Get_FenceCore();

		this->m_LastSignaledFenceVaule = 0;
		this->m_LastCompletedFenceValue = this->m_FenceCore->m_FenceValueAvailableAt;
		this->m_CurrentFenceValue = this->m_LastCompletedFenceValue + 1;
	}

	Uint64 Imp_Get_CurrentFence(void) { return this->m_CurrentFenceValue; }

	Uint64 Imp_Signal(D3D12_COMMAND_LIST_TYPE InQueueType) {
		this->InternalSignal(InQueueType, this->m_CurrentFenceValue);
	}


};

export class FenceCommandList final : public FenceBase<FenceCommandList> {
	friend class FenceBase<FenceCommandList>;
public:
	FenceCommandList(Device* Parent) :
		FenceBase { Parent }
	{}

private:
	//TDO :IMPI
	void Imp_DeferredInitializate(void) {
		this->m_FenceCore = this->m_Device->Get_FenceCorePool()->Get_FenceCore();
		this->m_LastSignaledFenceVaule = 0;

		this->m_LastCompletedFenceValue = this->m_FenceCore->m_FenceValueAvailableAt;
		this->m_CurrentFenceValue = this->m_LastCompletedFenceValue + 1;
	}
	Uint64 Imp_Get_CurrentFence(void) { return this->m_CurrentFenceValue; }

	Uint64 Imp_Signal(D3D12_COMMAND_LIST_TYPE InCommandType) {
		this->InternalSignal(InCommandType, this->m_CurrentFenceValue);

	}


};

export template<typename FenceType>
class SyncPoint final {
	TRIVIAL_FUNCTION(SyncPoint<FenceType>)
public:
	/*explicit*/ SyncPoint(FenceBase<FenceType>* Fence):
		m_Fence { Fence },
		m_Value { Fence->UpdateCompletedFenceValue()}
	{}

public:
	[[nodiscard]] bool Is_Compelete(void) const {
		return this->m_Fence->Is_FenceComplete(this->m_Value);
	}

	void WaitForCompletion(void) {
		this->m_Fence->WaitForFence(this->m_Value);
	}

private:
	FenceBase<FenceType>* m_Fence { nullptr };//TODO Remove It
	Uint64 m_Value;
};


template<typename Derived>
void FenceBase<Derived>::WaitForFence(Uint64 FenceValue) {
	if (true == this->Is_FenceComplete(FenceValue))
		return;

	static Mutex s_WaitMutex {};

	{
		LockGuard<Mutex> Lock { s_WaitMutex };

		this->m_FenceCore->Get_Fence()->SetEventOnCompletion(FenceValue, this->m_FenceCore->Get_FenceCompleteEvent());

		WaitForSingleObject(this->m_FenceCore->Get_FenceCompleteEvent(), INFINITE);
	}

	this->UpdateCompletedFenceValue();
}