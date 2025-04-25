module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :RootSignatureManager;

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


export struct QuantizedBoundShaderDesc final {
	struct ShaderRegisterCounts final {
		Uint8 ShaderResourceCount;
		Uint8 UnorderedAccessCount;
		Uint8 ConstantBufferCount;
		Uint8 SamplerCount;
	};

	Array<ShaderRegisterCounts, Tounderlying(ShaderVisibility::Count)> RegisterCounts;
	bool Is_AllowIAInputLayout;
};

Function<Uint64(const QuantizedBoundShaderDesc&)> QuantizedBoundShaderDescHash = [](const QuantizedBoundShaderDesc& desc) {
	Uint64 hash { 0 };

	for (const auto& ShaderRegisterCount : desc.RegisterCounts) {
		HashCombine(hash, ShaderRegisterCount.ShaderResourceCount);
		HashCombine(hash, ShaderRegisterCount.UnorderedAccessCount);
		HashCombine(hash, ShaderRegisterCount.ConstantBufferCount);
		HashCombine(hash, ShaderRegisterCount.SamplerCount);
	}
	HashCombine(hash, desc.Is_AllowIAInputLayout);

	return hash;
	};

Function<bool(const QuantizedBoundShaderDesc&, const QuantizedBoundShaderDesc&)> QuantizedBoundShaderDescEqual = [](const QuantizedBoundShaderDesc& lhs, const QuantizedBoundShaderDesc& rhs) {
	for (Uint32 Index = 0; Index < Tounderlying(ShaderVisibility::Count); ++Index) {
		if (lhs.RegisterCounts[Index].ShaderResourceCount != rhs.RegisterCounts[Index].ShaderResourceCount)
			return false;
		if (lhs.RegisterCounts[Index].UnorderedAccessCount != rhs.RegisterCounts[Index].UnorderedAccessCount)
			return false;
		if (lhs.RegisterCounts[Index].ConstantBufferCount != rhs.RegisterCounts[Index].ConstantBufferCount)
			return false;
		if (lhs.RegisterCounts[Index].SamplerCount != rhs.RegisterCounts[Index].SamplerCount)
			return false;
	}
	return lhs.Is_AllowIAInputLayout == rhs.Is_AllowIAInputLayout;
	};

export struct RootSignatureDesc final {
	Uint32 RootParameterCount { 0 };//this fild up 
	Array<CD3DX12_ROOT_PARAMETER1, g_MaxRootParameterCast> TableSlots;//NOTE :each Cast greater 1
	Array<CD3DX12_DESCRIPTOR_RANGE1,g_MaxRootParameterCast> DescriptorRanges;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootDesc {};

	//NOTE : to Unlock ,Defend Init
	void Build(const QuantizedBoundShaderDesc& bounddesc, D3D12_RESOURCE_BINDING_TIER tiler);
};

export class RootSignatureState final : public MoveAbleOnly, public DeviceChild {
public:
	//NOTE :Plase Call DeferredInitializate
	RootSignatureState(Device* parent):
		MoveAbleOnly {},
		DeviceChild { parent } 
	{}
public:
	~RootSignatureState(void) = default;
	
	void DeferredInitializate(const QuantizedBoundShaderDesc& bounddesc);

	void WatiForLoading(void) {
		while (!static_cast<volatile bool&>(this->m_Is_Loading)) {
			ThisThrad::yield();
		}
	}

private:
	RootSignatureDesc m_RootSignatureDesc {};

	ComPtr<ID3DBlob> m_RootSignatureBlob { nullptr };
	ComPtr<ID3D12RootSignature> m_RootSignature { nullptr };

	bool m_Is_Loading { false };

};

export class RootSignatureManager final : public MoveAbleOnly, public DeviceChild {
public:
	
	RootSignatureManager(Device* parent) :
		MoveAbleOnly {},
		DeviceChild { parent }
	{}

	~RootSignatureManager(void) = default;

	RootSignatureState* CreateRootSignature(const QuantizedBoundShaderDesc& bounddesc) {
		
		RootSignatureState* NewRootSignatureState { nullptr };
		{
			LockGuard<Mutex> Lock(this->m_Mutex);

			if (auto it { this->m_RootSignatureMap.find(bounddesc) }; it != this->m_RootSignatureMap.end()) {
				it->second->WatiForLoading();

				return it->second.get();
			}
			else {
				this->m_RootSignatureMap[bounddesc] =MakeShared<RootSignatureState>(this->m_Device);

				NewRootSignatureState = this->m_RootSignatureMap[bounddesc].get();
			}
		}

		NewRootSignatureState->DeferredInitializate(bounddesc);

		return NewRootSignatureState;
	}


public:
	Mutex m_Mutex {};
	UnorderedMap<QuantizedBoundShaderDesc, SharedPtr<RootSignatureState>, decltype(QuantizedBoundShaderDescHash), decltype(QuantizedBoundShaderDescEqual)>
		m_RootSignatureMap { 0,QuantizedBoundShaderDescHash, QuantizedBoundShaderDescEqual };


};




void RootSignatureDesc::Build(const QuantizedBoundShaderDesc& QBSS, D3D12_RESOURCE_BINDING_TIER ResourceBindingTier) {
	using enum ShaderVisibility;

	static constexpr ShaderVisibility ShaderVisibilityPriorityOrder[] {
		Pixel,
		Vertex,
		Geometry,
		Hull,
		Domain,
		All
	};

	static constexpr D3D12_ROOT_PARAMETER_TYPE RootParameterTypePriorityOrder[] {
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
		D3D12_ROOT_PARAMETER_TYPE_CBV
	};

	//Ref UE4
	// Determine if our descriptors or their data is static based on the resource binding tier.
		// We do this because sometimes (based on binding tier) our descriptor tables are bigger than the # of descriptors we copy. See FD3D12QuantizedBoundShaderState::InitShaderRegisterCounts().
	const auto SRVDescriptorRangeFlags { (ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE };

	const auto CBVDescriptorRangeFlags { (ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE };

	const auto UAVDescriptorRangeFlags { (ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE };

	const auto SamplerDescriptorRangeFlags { (ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_NONE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE };

	const auto CBVRootDescriptorFlags { D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC };	// We always set the data in an upload heap before calling Set*RootConstantBufferView.

	Uint32 CurrentCast { 0 };

	for (const auto& RootParameterType : RootParameterTypePriorityOrder)
		for (const auto& ShaderVisibility : ShaderVisibilityPriorityOrder) {
			const QuantizedBoundShaderDesc::ShaderRegisterCounts Shader { QBSS.RegisterCounts[Tounderlying(ShaderVisibility)] };

			switch (RootParameterType) {
				case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: {
					if (Shader.ShaderResourceCount > 0) {
						ASSERT(CurrentCast < g_MaxRootParameterCast);

						this->DescriptorRanges[this->RootParameterCount].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
							Shader.ShaderResourceCount,
							0,
							g_DefualtBindingSpace,
							SRVDescriptorRangeFlags
						);
						this->TableSlots[this->RootParameterCount].InitAsDescriptorTable(
							1,
							&this->DescriptorRanges[this->RootParameterCount],
							T0_D3D12ShaderVisibility(ShaderVisibility)
						);

						++this->RootParameterCount;
						CurrentCast += g_PerRootDescriptorTableCost;
					}

					if (Shader.ConstantBufferCount > g_MaxRootCBVCount) {
						ASSERT(CurrentCast < g_MaxRootParameterCast);

						this->DescriptorRanges[this->RootParameterCount].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
							Shader.ConstantBufferCount - g_MaxRootCBVCount,
							g_MaxRootCBVCount,
							g_DefualtBindingSpace,
							CBVDescriptorRangeFlags
						);
						this->TableSlots[this->RootParameterCount].InitAsDescriptorTable(
							1,
							&this->DescriptorRanges[this->RootParameterCount],
							T0_D3D12ShaderVisibility(ShaderVisibility)
						);

						++this->RootParameterCount;
						CurrentCast += g_PerRootDescriptorTableCost;
					}

					if (Shader.UnorderedAccessCount > 0) {
						ASSERT(CurrentCast < g_MaxRootParameterCast);
						this->DescriptorRanges[this->RootParameterCount].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
							Shader.UnorderedAccessCount,
							0,
							g_DefualtBindingSpace,
							UAVDescriptorRangeFlags
						);
						this->TableSlots[this->RootParameterCount].InitAsDescriptorTable(
							1,
							&this->DescriptorRanges[this->RootParameterCount],
							T0_D3D12ShaderVisibility(ShaderVisibility)
						);

						++this->RootParameterCount;
						CurrentCast += g_PerRootDescriptorTableCost;
					}

					if (Shader.SamplerCount > 0) {
						ASSERT(CurrentCast < g_MaxRootParameterCast);
						this->DescriptorRanges[this->RootParameterCount].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
							Shader.SamplerCount,
							0,
							g_DefualtBindingSpace,
							SamplerDescriptorRangeFlags
						);
						this->TableSlots[this->RootParameterCount].InitAsDescriptorTable(
							1,
							&this->DescriptorRanges[this->RootParameterCount],
							T0_D3D12ShaderVisibility(ShaderVisibility)
						);

						++this->RootParameterCount;
						CurrentCast += g_PerRootDescriptorTableCost;
					}

					break;
				}

				case D3D12_ROOT_PARAMETER_TYPE_CBV:{
					for (Uint32 Rehister = 0; (Rehister < Shader.ConstantBufferCount) && (Rehister < g_MaxRootCBVCount); ++Rehister) {
						ASSERT(CurrentCast < g_MaxRootParameterCast);

						this->TableSlots[this->RootParameterCount].InitAsConstantBufferView(
							Rehister,
							g_DefualtBindingSpace,
							CBVRootDescriptorFlags,
							T0_D3D12ShaderVisibility(ShaderVisibility)
						);

						++this->RootParameterCount;
						CurrentCast += g_PerRootDescriptorCost;
					}

					break;
				}

				default:
					ASSERT(false);
					break;
			}
		}

	D3D12_ROOT_SIGNATURE_FLAGS Flags { QBSS.Is_AllowIAInputLayout ? D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT : D3D12_ROOT_SIGNATURE_FLAG_NONE };

	//TODO : Add Static Sampler
	this->RootDesc.Init_1_1(
		this->RootParameterCount,
		this->TableSlots.data(),
		0,
		nullptr,
		Flags
	);
}

void RootSignatureState::DeferredInitializate(const QuantizedBoundShaderDesc& bounddesc){
	this->m_RootSignatureDesc.Build(bounddesc, this->m_Device->Get_ResourceBindingTier());

	ID3DBlob* Error { nullptr };

	D3DX12SerializeVersionedRootSignature(
		&this->m_RootSignatureDesc.RootDesc,
		this->m_Device->Get_RootSignatureVersion(),
		&Error,
		&this->m_RootSignatureBlob
	);

	if (nullptr != Error) {
		LOG_ERROR("RootSignatureStateError:");

		Error->Release();
	}

	this->m_Device->Get_Device()->CreateRootSignature(
		this->m_Device->Get_ALLNode().Get_Native(),
		this->m_RootSignatureBlob->GetBufferPointer(),
		this->m_RootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&this->m_RootSignature)
	);

	this->m_Is_Loading = true;
}

