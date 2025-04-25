module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :PipelineState;

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
import :GPUNode;
import :RootSignatureManager;


export template<typename Derived>
class PipelineStateBase : public MoveAbleOnly, public DeviceChild {
public:
	PipelineStateBase(Device* Parent, const TString& name):
		MoveAbleOnly {},
		DeviceChild { Parent }, 
		m_Name { name }
	{}

	virtual ~PipelineStateBase(void) = default;

public:
	void WatirForLoad(void) {
		while (false == static_cast<volatile bool>(this->m_Is_Loaded))
			ThisThrad::yield();
	}


	//TODO :
	
protected:
	TString m_Name;

	ID3D12PipelineState* m_PipelineState { nullptr };

	bool m_Is_Loaded { false };

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:
	
};


//NOTE :NOt Use StreamOut
//NOTE : You Shoudle use ShredPtr to make sure the memory is not free
export class GraphicsPipelineState final : public PipelineStateBase<GraphicsPipelineState> ,public EnableSharedFormThis<GraphicsPipelineState> {
	friend class PipelineStateBase<GraphicsPipelineState>;
public:
	GraphicsPipelineState(Device* Parent,const TString& name):
		PipelineStateBase<GraphicsPipelineState> { Parent,name },
		EnableSharedFormThis<GraphicsPipelineState>{} {
	}

	~GraphicsPipelineState(void) = default;

	GraphicsPipelineState& Set_RootSignature(ID3D12RootSignature* InRootSignature) {
		//NOTE : Root Signature int Cache ,i make sure it is not null
		this->m_Desc.pRootSignature = InRootSignature;

		return *this;
	}

	GraphicsPipelineState& Set_VS(const D3D12_SHADER_BYTECODE& InVS) {
		this->m_Desc.VS = InVS;
		
		return *this;
	}

	GraphicsPipelineState& Set_PS(const D3D12_SHADER_BYTECODE& InPS) {
		this->m_Desc.PS = InPS;

		return *this;
	}

	GraphicsPipelineState& Set_DS(const D3D12_SHADER_BYTECODE& InDS) {
		this->m_Desc.DS = InDS;
		
		return *this;
	}

	GraphicsPipelineState& Set_HS(const D3D12_SHADER_BYTECODE& InHS) {
		this->m_Desc.HS = InHS;

		return *this;
	}

	GraphicsPipelineState& Set_GS(const D3D12_SHADER_BYTECODE& InGS) {
		this->m_Desc.GS = InGS;
		
		return *this;
	}

	GraphicsPipelineState& Set_BlendState(const D3D12_BLEND_DESC& InBlendState) {
		this->m_Desc.BlendState = InBlendState;
		
		return *this;
	}

	GraphicsPipelineState& Set_SampleMask(UINT InSampleMask) {
		this->m_Desc.SampleMask = InSampleMask;

		return *this;
	}

	GraphicsPipelineState& Set_RenderTargetFormats(const Vector<DXGI_FORMAT>& InRTVFormats){
		this->m_Desc.NumRenderTargets = static_cast<Uint32>(InRTVFormats.size());
		for (Uint32 Index = 0; const auto& RTVFormat:InRTVFormats) 
			this->m_Desc.RTVFormats[Index++] = RTVFormat;

		return *this;
	}

	GraphicsPipelineState& Set_RenderTargetFormat(DXGI_FORMAT InRTVFormat) {
		this->m_Desc.NumRenderTargets = 1;
		this->m_Desc.RTVFormats[0] = InRTVFormat;

		return *this;
	}
	
	GraphicsPipelineState& Set_DepthStencilFormat(DXGI_FORMAT InDSVFormat) {
		this->m_Desc.DSVFormat = InDSVFormat;

		return *this;
	}

	GraphicsPipelineState& Set_SampleDesc(const DXGI_SAMPLE_DESC& InSampleDesc) {
		this->m_Desc.SampleDesc = InSampleDesc;

		return *this;
	}
	
	GraphicsPipelineState& Set_RasterizerState(const D3D12_RASTERIZER_DESC& InRasterizerState) {
		this->m_Desc.RasterizerState = InRasterizerState;

		return *this;
	}

	GraphicsPipelineState& Set_DepthStencilState(const D3D12_DEPTH_STENCIL_DESC& InDepthStencilState) {
		this->m_Desc.DepthStencilState = InDepthStencilState;
		
		return *this;
	}

	GraphicsPipelineState& Set_InputLayout(const SharedPtr<D3D12_INPUT_ELEMENT_DESC[]> pInputElementDescs, UINT NumElements) {
		this->m_InputElementDescs ={ pInputElementDescs, NumElements };
		this->m_Desc.InputLayout = { pInputElementDescs.get(), NumElements };

		return *this;
	}

	GraphicsPipelineState& Set_PrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps) {
		this->m_Desc.IBStripCutValue = IBProps;
		
		return *this;
	}

	GraphicsPipelineState& Set_PrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE InPrimitiveTopologyType) {
		//TODO
		
		this->m_Desc.PrimitiveTopologyType = InPrimitiveTopologyType;

		return *this;
	}

	void DeferredInitializate(void);

public:

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_Desc {
		.pRootSignature{nullptr},
		.VS{nullptr},
		.PS{nullptr},
		.DS{nullptr},
		.HS{nullptr},
		.GS{nullptr},
		.StreamOutput{nullptr},
		.BlendState{CD3DX12_BLEND_DESC{D3D12_DEFAULT}},
		.SampleMask{UINT_MAX},
		.RasterizerState{CD3DX12_RASTERIZER_DESC{D3D12_DEFAULT}},
		.DepthStencilState{CD3DX12_DEPTH_STENCIL_DESC{D3D12_DEFAULT}},
		.InputLayout{nullptr, 0},
		.IBStripCutValue{D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED},
		.PrimitiveTopologyType{D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE},
		.NumRenderTargets{1},
		.RTVFormats{DXGI_FORMAT_R8G8B8A8_UNORM},
		.DSVFormat{DXGI_FORMAT_D24_UNORM_S8_UINT},
		.SampleDesc{1, 0},
		.NodeMask{0},
		.CachedPSO{nullptr,0},
		.Flags{D3D12_PIPELINE_STATE_FLAG_NONE}
	};
	//TODO :this Desc can attend to a cache
	Pair<SharedPtr<D3D12_INPUT_ELEMENT_DESC[]>,Uint32> m_InputElementDescs {};
	
};


Mutex g_GraphicsPipelineStateCacheMutex {};
UnorderedMap<TString, SharedPtr<GraphicsPipelineState>> g_GraphicsPipelineStateCache {};//TODO Remove Name

void GraphicsPipelineState::DeferredInitializate(void){
	{
		LockGuard<Mutex> Lock(g_GraphicsPipelineStateCacheMutex);
		if (auto It = g_GraphicsPipelineStateCache.find(this->m_Name); It != g_GraphicsPipelineStateCache.end()) {
			It->second->WatirForLoad();

			this->m_PipelineState = It->second->m_PipelineState;
		}
		else
			g_GraphicsPipelineStateCache[this->m_Name] = EnableSharedFormThis<GraphicsPipelineState>::shared_from_this();
	}

	this->m_Device->Get_Device()->CreateGraphicsPipelineState(
		&this->m_Desc,
		IID_PPV_ARGS(&this->m_PipelineState)
	);

	this->m_Is_Loaded = true;

}


export class ComputePipelineState final : public PipelineStateBase<ComputePipelineState>, public EnableSharedFormThis<ComputePipelineState> {
	friend class PipelineStateBase<ComputePipelineState>;
public:
	ComputePipelineState(Device* Parent, const TString& name) :
		PipelineStateBase<ComputePipelineState> { Parent,name },
		EnableSharedFormThis<ComputePipelineState>{} {
	}
	~ComputePipelineState(void) = default;
	ComputePipelineState& Set_RootSignature(ID3D12RootSignature* InRootSignature) {
		this->m_Desc.pRootSignature = InRootSignature;
		
		return *this;
	}
	ComputePipelineState& Set_CS(const D3D12_SHADER_BYTECODE& InCS) {
		this->m_Desc.CS = InCS;
		
		return *this;
	}
	void DeferredInitializate(void);
public:
private:
	D3D12_COMPUTE_PIPELINE_STATE_DESC m_Desc {
		.pRootSignature{nullptr},
		.CS{nullptr},
		.NodeMask{0},
		.CachedPSO{nullptr,0},
		.Flags{D3D12_PIPELINE_STATE_FLAG_NONE}
	};
};
//TODO : this Desc can attend to a cache

Mutex g_ComputePipelineStateCacheMutex {};
UnorderedMap<TString, SharedPtr<ComputePipelineState>> g_ComputePipelineStateCache {};//TODO Remove Name

void ComputePipelineState::DeferredInitializate(void) {
	{
		LockGuard<Mutex> Lock(g_ComputePipelineStateCacheMutex);
		if (auto It = g_ComputePipelineStateCache.find(this->m_Name); It != g_ComputePipelineStateCache.end()) {
			It->second->WatirForLoad();

			this->m_PipelineState = It->second->m_PipelineState;
		}
		else
			g_ComputePipelineStateCache[this->m_Name] = EnableSharedFormThis<ComputePipelineState>::shared_from_this();
	}
	this->m_Device->Get_Device()->CreateComputePipelineState(
		&this->m_Desc,
		IID_PPV_ARGS(&this->m_PipelineState)
	);

	this->m_Is_Loaded = true;
}