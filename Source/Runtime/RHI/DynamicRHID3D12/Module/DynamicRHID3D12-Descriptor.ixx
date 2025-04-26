module;

#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :Descriptor;

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

export template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
class DescriptorAllocator final : public MoveAbleOnly, public DeviceChild, public SingleNodeGPUObject {
	static_assert(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES != _Type, "Invalid Descriptor Heap Type");	
public:
	DescriptorAllocator(Device* Parent, RHIGPUMask InGPUMask) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUMask }{
	
		{
			m_HeapDesc.Type = _Type;
			m_HeapDesc.NumDescriptors = s_DescriptorCount;
			m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			m_HeapDesc.NodeMask = InGPUMask.Get_Native();
		}
	}
	
	~DescriptorAllocator(void) = default;
	
public:
	D3D12_CPU_DESCRIPTOR_HANDLE Allocate(Uint32 Count) {
		if (nullptr == m_CurrentHeap || m_RemainingDescriptors < Count) {
			this->m_CurrentHeap=this->AllocateNewHeap();
			this->m_CurrentHeapCPUHandle = m_CurrentHeap->GetCPUDescriptorHandleForHeapStart();
			this->m_RemainingDescriptors = s_DescriptorCount;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE ret { this->m_CurrentHeapCPUHandle };
		this->m_CurrentHeapCPUHandle.ptr += Count * this->m_Device->Get_DescriptorSize<_Type>();
		this->m_RemainingDescriptors -= Count;

		return ret;
	}
private:
	ID3D12DescriptorHeap* AllocateNewHeap(void) {
		ComPtr<ID3D12DescriptorHeap> NewHeap;
		{
			LockGuard<Mutex> Lock(this->m_Mutex);
			D3D12_CHECK(this->m_Device->Get_Device()->CreateDescriptorHeap(&m_HeapDesc, IID_PPV_ARGS(&NewHeap)));
			this->m_DescriptorHeaps.emplace_back(NewHeap);
		}

		return NewHeap.Get();
	}

private:
	static constexpr Uint32 s_DescriptorCount = g_DescriptorCountPerHeap;
	D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
	
	Mutex m_Mutex {};
	Vector<ComPtr<ID3D12DescriptorHeap>> m_DescriptorHeaps {};
	
	ID3D12DescriptorHeap* m_CurrentHeap { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE m_CurrentHeapCPUHandle {};
	Uint32 m_RemainingDescriptors { s_DescriptorCount };
};

//NOTE :Strut to use Handle
export template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
class DescriptorHandle final {
	static_assert(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES != _Type, "Invalid Descriptor Heap Type");
	TRIVIAL_FUNCTION(DescriptorHandle<_Type>)
public:
	DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle) :
		m_CPUDescriptorHandle { CPUHandle },
		m_GPUDescriptorHandle { GPUHandle } 
	{}

public:
	const D3D12_CPU_DESCRIPTOR_HANDLE* operator&(void)const { return &this->m_CPUDescriptorHandle; }

	operator D3D12_CPU_DESCRIPTOR_HANDLE(void)const { return this->m_CPUDescriptorHandle; }
	operator D3D12_GPU_DESCRIPTOR_HANDLE(void)const { return this->m_GPUDescriptorHandle; }

	DescriptorHandle operator+=(Uint32 Count) {
		this->m_CPUDescriptorHandle.ptr += Count * this->m_Device->Get_DescriptorSize<_Type>();
		this->m_GPUDescriptorHandle.ptr += Count * this->m_Device->Get_DescriptorSize<_Type>();
		return *this;
	}

	DescriptorHandle operator+(Uint32 Count) const {
		DescriptorHandle Temp { *this };
		Temp += Count;
		return Temp;
	}

public:
	Uint64 Get_CPUPtr(void)const { return this->m_CPUDescriptorHandle.ptr; }
	Uint64 Get_GPUPtr(void)const { return this->m_GPUDescriptorHandle.ptr; }

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUDescriptorHandle {};
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUDescriptorHandle {};

};

export template<typename Derived>
struct DescriptorHeapTraits;

template<>
struct DescriptorHeapTraits<class GlobalSamplerHeap> {
	static constexpr auto DescriptorHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
};

template<>
struct DescriptorHeapTraits<class GlobalViewHeap> {
	static constexpr auto DescriptorHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
};


export template<typename Derived>
class DescriptorHeapBase :public MoveAbleOnly, public DeviceChild, public SingleNodeGPUObject {
	static constexpr auto DescriptorHeapType = DescriptorHeapTraits<Derived>::DescriptorHeapType;
protected:
	DescriptorHeapBase(Device* Parent, RHIGPUMask InGPUMask) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUMask },
		m_DescriptorSize { Parent->Get_DescriptorSize<DescriptorHeapType>() } {
	}

	void DeferredInitializate(Uint32 MaxCount) {
		this->m_DescriptorCount = MaxCount;
		this->m_FreeDescriptorCount = this->m_DescriptorCount;

		{
			this->m_HeapDesc.Type = DescriptorHeapType;
			this->m_HeapDesc.NumDescriptors = this->m_DescriptorCount;
			this->m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			this->m_HeapDesc.NodeMask = this->Get_GPUMask().Get_Native();
		}
		D3D12_CHECK(this->m_Device->Get_Device()->CreateDescriptorHeap(&this->m_HeapDesc, IID_PPV_ARGS(&this->m_Heap)));

		this->m_FirstHandle = DescriptorHandle<DescriptorHeapType> {
			this->m_Heap->GetCPUDescriptorHandleForHeapStart(),
			this->m_Heap->GetGPUDescriptorHandleForHeapStart()
		};
		this->m_NextFreeHandle = this->m_FirstHandle;
	}

public:
	virtual ~DescriptorHeapBase(void) = default;

protected:
	bool HasAvailableSpace(Uint32 Count) { return this->m_FreeDescriptorCount <= Count; }

	bool Is_ValidateHandle(const DescriptorHandle<DescriptorHeapType>& DHandle) {
		if (DHandle.Get_CPUPtr < this->m_FirstHandle.Get_CPUPtr() ||
			DHandle.Get_CPUPtr >= this->m_FirstHandle.Get_CPUPtr())
			return false;

		 if(DHandle.Get_GPUPtr() - this->m_FirstHandle.Get_GPUPtr()!=
		    DHandle.Get_CPUPtr() - this->m_FirstHandle.Get_CPUPtr())
			return false;

		 return true;
	}

	//TODO Add Check to Allocate
	DescriptorHandle<DescriptorHeapType> Allocate(Uint32 Count) {
		ASSERT(this->HasAvailableSpace(Count));

		auto Re { this->m_NextFreeHandle };

		this->m_NextFreeHandle += Count * this->m_DescriptorSize;
		this->m_FreeDescriptorCount -= Count;

		return Re;
	}

private:
	Uint32 m_DescriptorSize;

	Uint32 m_DescriptorCount {};
	Uint32 m_FreeDescriptorCount {};

	D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc {};
	ComPtr<ID3D12DescriptorHeap> m_Heap { nullptr };

	DescriptorHandle<DescriptorHeapType> m_FirstHandle {};
	DescriptorHandle<DescriptorHeapType> m_NextFreeHandle {};

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

};


export class GlobalSamplerHeap final :public DescriptorHeapBase<GlobalSamplerHeap> {
	friend class DescriptorHeapBase<GlobalSamplerHeap>;
public:
	GlobalSamplerHeap(Device* Parent, RHIGPUMask InGPUMask ,Uint32 GlobalSamerDescriptorCount=20)
		:DescriptorHeapBase<GlobalSamplerHeap> { Parent, InGPUMask } {

		this->DeferredInitializate(GlobalSamerDescriptorCount);
	}

	~GlobalSamplerHeap(void) = default;

};

export class GlobalViewHeap final :public DescriptorHeapBase<GlobalViewHeap> {
	friend class DescriptorHeapBase<GlobalViewHeap>;
public:
	GlobalViewHeap(Device* Parent, RHIGPUMask InGPUMask, Uint32 GlobalViewDescriptorCount = 100)
		:DescriptorHeapBase<GlobalViewHeap> { Parent, InGPUMask } {

		this->DeferredInitializate(GlobalViewDescriptorCount);
	}

	~GlobalViewHeap(void) = default;

};
