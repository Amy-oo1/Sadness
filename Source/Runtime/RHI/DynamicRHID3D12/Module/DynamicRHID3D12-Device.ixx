module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12:Device;

import Platform;
import Utility;
import Concurrent;
import Memory;
import Container;
import Logger;

import RHI;

import D3D12Definition;

import :Forward;
import :Common;

export class Device final : public MoveAbleOnly{
	//friend class DynamicRHID3D12;
public:
	Device(ComPtr<IDXGIAdapter2> adapter);

public:
	~Device(void);

public:
	ID3D12Device4* Get_Device(void) {
		return this->m_Device4.Get();
	}

private:


private:


public:
	template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
	Uint32 Get_DescriptorSize(void) {//TODO : Set a cache is best
		return this->m_Device4->GetDescriptorHandleIncrementSize(_Type);
	}

	ID3D12CommandQueue* Get_CommandQueue(D3D12_COMMAND_LIST_TYPE Type) {
		//TODO
		return nullptr;
	}



	GPUNode* Get_GPUNode(RHIGPUMask InGPUMask) {
		ASSERT(InGPUMask.Get_Index() < g_MaxGPUCount);

		return this->m_GPUNodes[InGPUMask.Get_Index()];
	}

	FenceCorePool* Get_FenceCorePool(void) { return this->m_FenceCorePool; }

	FenceManual* Get_FrameFence(void) {
		return this->m_FrameFence;
	}

	const RHIGPUMask Get_ALLNode()const { return RHIGPUMask::All(); }

	const D3D_ROOT_SIGNATURE_VERSION Get_RootSignatureVersion(void) { return g_RootSignatureVersion; }//TODO

	D3D12_RESOURCE_BINDING_TIER Get_ResourceBindingTier(void) { return D3D12_RESOURCE_BINDING_TIER_1; }//TODO :



public:
	void CreateStaticSampler(const RHISamplerDesc& Desc);

	void CreateTexture2D(const RHITextrure2DDesc& Desc, Uint8* data);


private:
	//TODO :not all type can not in auto ptr ,unless you make a delete ,typpe is too much,use new delete
	void InitializatiePartGPUNode(void);

	void DestroyPartGPUNode(void);

	void InitializatiePartFence(void);

	void DestroyPartFence(void);

	void InitializatiePartCommandListManager(void);

	void DestroyPartCommandListManager(void);

	void InitializatiePartSamplerManager(void);

	void DestroyPartSamplerManager(void);

private:
	ComPtr<IDXGIAdapter2> m_Adapter2;
	ComPtr<ID3D12Device4> m_Device4 { nullptr };

	ComPtr<ID3D12CommandSignature> m_DrawIndirectCommandSignature { nullptr };
	ComPtr<ID3D12CommandSignature> m_DrawIndexedIndirectCommandSignature { nullptr };
	ComPtr<ID3D12CommandSignature> m_DispatchIndirectCommandSignature { nullptr };

private:
	Array<GPUNode*, g_MaxGPUCount> m_GPUNodes {};

	FenceCorePool* m_FenceCorePool { nullptr };
	FenceIncrement* m_StagingFence { nullptr };
	FenceManual* m_FrameFence { nullptr };

	SamplerManager* m_SamplerManager { nullptr };

	D3D12_HEAP_PROPERTIES m_ConstantBufferPageProperties {};//TODO ?

private:
	

};