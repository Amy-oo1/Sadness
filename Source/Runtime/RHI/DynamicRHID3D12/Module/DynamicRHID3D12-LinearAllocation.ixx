module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :LinearAllocation;

import Platform;
import Utility;
import Container;
import Memory;
import Concurrent;
import Logger;

import RHI;

import :Forward;
import :Common;
import :Device;

//
//export struct DynamicAllocation final{
//	DynamicAllocation()
//
//	ComPtr<ID3D12Resource> Resource;
//	UINT64 Offset;
//	Uint64 Size;
//	static const Uint64 s_Alignment { 256 };
//	void* Data { nullptr };
//	D3D12_GPU_VIRTUAL_ADDRESS GPUAddress { 0 };
//};


export class LinearAllocationPage final {
public:
	LinearAllocationPage(ComPtr<ID3D12Resource> resource) :m_Resource { resource } {}
	
	~LinearAllocationPage(void) {
		this->Unmap();
	}


public:
	void Map(void) {
		if (nullptr==m_CPUVirtualAddress)
			D3D12_CHECK(this->m_Resource->Map(0, nullptr, &this->m_CPUVirtualAddress));
	}

	void Unmap(void) {
		if (nullptr != m_CPUVirtualAddress) {
			this->m_Resource->Unmap(0, nullptr);
		
			this->m_CPUVirtualAddress = nullptr;
		}
	}

private:
	ComPtr<ID3D12Resource> m_Resource;
	void* m_CPUVirtualAddress { nullptr };
	D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress;

};

export enum class LinearAllocatorType :Int8 {
	Invalid = -1,

	GpuExclusive = 0,		// DEFAULT   GPU-writeable (via UAV)
	CpuWritable = 1,		// UPLOAD CPU-writeable (but write combined)

	Count
};

//NOTE :Remove To D3D12
constexpr Uint64 g_GPUAllocatorPageSize = 0x010000;	// 64K
constexpr Uint64 g_CPUAllocatorPageSize = 0x200000;	// 2M

export template<LinearAllocatorType _Type>
struct HeapTypetraits;

export template<>
struct HeapTypetraits<LinearAllocatorType::GpuExclusive> {
	static constexpr auto Type = D3D12_HEAP_TYPE_DEFAULT;
	static constexpr auto State = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	static constexpr auto PageSize = g_GPUAllocatorPageSize;
	static constexpr auto Flag = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
};

export template<>
struct HeapTypetraits<LinearAllocatorType::CpuWritable> {
	static constexpr auto Type = D3D12_HEAP_TYPE_UPLOAD;
	static constexpr auto State = D3D12_RESOURCE_STATE_GENERIC_READ;
	static constexpr auto PageSize = g_CPUAllocatorPageSize;
	static constexpr auto Flag = D3D12_RESOURCE_FLAG_NONE;
};

export template<LinearAllocatorType _Type>
class LinearAllocatorPageManager final  :public DeviceChild {
public:
	LinearAllocatorPageManager(Device* Parent)
		:DeviceChild { Parent } 
	{}

	~LinearAllocatorPageManager(void) = default;

public:
	LinearAllocationPage* RequestPage(void);

	void DiscardPages(Uint64 FenceValue, Span<LinearAllocationPage*> UsedPages);

	void FreeLargePages(Uint64 FenceValue, Span<LinearAllocationPage*> UsedPages);


public://NOTE : not In Pool
	UniquePtr<LinearAllocationPage> CreateNewPage(Uint64 PageSize = HeapTypetraits<_Type>::PageSize);

private:
	LinearAllocatorType m_AllocatorType { _Type };

	Mutex m_Mutex {};
	Vector<UniquePtr<LinearAllocationPage>> m_PagePool;
	Queue<Pair<Uint64, LinearAllocationPage*>> m_RetiredPages;
	Queue<Pair<Uint64, LinearAllocationPage*>> m_DeletionQueue;
	Queue<LinearAllocationPage*> m_AvailablePages;
};

template<LinearAllocatorType _Type>
LinearAllocationPage* LinearAllocatorPageManager<_Type>::RequestPage(void){
	{
		LockGuard<Mutex> LockGuard { this->m_Mutex };
		//TODO
		while (!this->m_RetiredPages.empty()) {

			this->m_AvailablePages.push(this->m_RetiredPages.front().second);
			this->m_RetiredPages.pop();
		}

		if (!this->m_AvailablePages.empty()) {
			auto Re = this->m_AvailablePages.front();
			this->m_AvailablePages.pop();
			
			return Re;
		}
		else {
			this->m_PagePool.emplace_back(this->CreateNewPage());

			return this->m_PagePool.back().get();
		}
	}
}

template<LinearAllocatorType _Type>
void LinearAllocatorPageManager<_Type>::DiscardPages(Uint64 FenceValue, Span<LinearAllocationPage*> UsedPages){
	{
		LockGuard<Mutex> Lock { this->m_Mutex };

		for (const auto& Page : UsedPages)
			this->m_RetiredPages.push(MakePair(FenceValue, Page));
	}
}

template<LinearAllocatorType _Type>
void LinearAllocatorPageManager<_Type>::FreeLargePages(Uint64 FenceValue, Span<LinearAllocationPage*> UsedPages){
	{
		LockGuard<Mutex> Lock { this->m_Mutex };

		for (const auto& Page : UsedPages)
			this->m_RetiredPages.push(MakePair(FenceValue, Page));
	}
}

template<LinearAllocatorType _Type>
UniquePtr<LinearAllocationPage> LinearAllocatorPageManager<_Type>::CreateNewPage(Uint64 PageSize ){
	//TODO :
	static constexpr D3D12_HEAP_PROPERTIES HeapProperties {
		.Type { HeapTypetraits<_Type>::Type },
		.CPUPageProperty { D3D12_CPU_PAGE_PROPERTY_UNKNOWN },
		.MemoryPoolPreference { D3D12_MEMORY_POOL_UNKNOWN },
		.CreationNodeMask { 1 },//TODO
		.VisibleNodeMask  { 1 }//TODO
	};

	const D3D12_RESOURCE_DESC Desc {
		.Dimension { D3D12_RESOURCE_DIMENSION_BUFFER },
		.Alignment { 0 },
		.Width { PageSize },
		.Height { 1 },
		.DepthOrArraySize { 1 },
		.MipLevels { 1 },
		.Format{ DXGI_FORMAT_UNKNOWN },
		.SampleDesc {.Count{ 1 }, .Quality { 0 } },
		.Layout { D3D12_TEXTURE_LAYOUT_ROW_MAJOR },
		.Flag  { HeapTypetraits<_Type>::Flag }
	};

	ComPtr<ID3D12Resource> pBuffer;
	this->m_Device->Get_Device()->CreateCommittedResource(
		&HeapProperties, 
		D3D12_HEAP_FLAG_NONE,
		&Desc,
		HeapTypetraits<_Type>::State, 
		nullptr, 
		IID_PPV_ARGS(&pBuffer)
	);

	return MakeUnique<LinearAllocationPage>(pBuffer);
}
