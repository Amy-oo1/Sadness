module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

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
class DescriptorAllocator final :public MoveAbleOnly, public DeviceChild , public SingleNodeGPUObject{
	static_assert(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES != _Type, "Invalid Descriptor Heap Type");

public:
	DescriptorAllocator(Device* Parent,RHIGPUMask InGPUMask) :
		MoveAbleOnly {},
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUMask },
		m_RemainingDescriptors{ s_DescriptorCount }{

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
			D3D12_CHECK(this->m_Device->Get_Device()->CreateDescriptorHeap(&m_HeapDesc,IID_PPV_ARGS(&NewHeap)));
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
	Uint32 m_RemainingDescriptors;

};

export template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
class DescriptorHandle final :public NonDefaultConstructible, public DeviceChild {
	static_assert(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES != _Type, "Invalid Descriptor Heap Type");
public:
	DescriptorHandle(Device* Parent, D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle) :
		NonDefaultConstructible {},
		DeviceChild { Parent },
		m_CPUDescriptorHandle { CPUHandle },
		m_GPUDescriptorHandle {GPUHandle } 
	{}

public:
	~DescriptorHandle(void) = default;

public:
	const D3D12_CPU_DESCRIPTOR_HANDLE* operator&(void)const { return &this->m_CPUDescriptorHandle; }

	operator D3D12_CPU_DESCRIPTOR_HANDLE(void)const { return this->m_CPUDescriptorHandle; }
	operator D3D12_GPU_DESCRIPTOR_HANDLE(void)const { return this->m_GPUDescriptorHandle; }

	DescriptorHandle operator+=(Uint32 Count) {
		this->m_CPUDescriptorHandle.ptr += Count * this->m_Device->Get_DescriptorSize<_Type>();
		this->m_GPUDescriptorHandle.ptr += Count * this->m_Device->Get_DescriptorSize<_Type>();
		return *this;
	}

	DescriptorHandle operator+(Uint32 Count) {
		DescriptorHandle Temp { *this };
		Temp += Count;
		return Temp;
	}

public:
	Uint64 Get_CPUHandlePtr(void)const {return this->m_CPUDescriptorHandle.ptr;}
	Uint64 Get_GPUHandlePtr(void)const { return this->m_GPUDescriptorHandle.ptr; }

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUDescriptorHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUDescriptorHandle;

};
