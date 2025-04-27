module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :CommandContext;

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
import :CommandListManager;
import :SamplerManager;
import :TextureManager;
//import :StateCache;

//TODO :do not use this class  has too many base class ........
export template <typename Derived>
class CommandContext :public DeviceChild ,public SingleNodeGPUObject{
protected:
	CommandContext(Device* Parent, RHIGPUMask InGPUIndex):
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUIndex } {
	}
public:
	virtual ~CommandContext(void) = default;


	void Reset(void){}

	void InitializeResource(ID3D12Resource* Dest, UINT64 SubResourceCount, D3D12_SUBRESOURCE_DATA* pSrcData) {
		
		Uint64 UploadBufferSize { GetRequiredIntermediateSize(Dest,0,SubResourceCount) };


	}




private:
	

public:

private:
	CommandAllocator* m_CommandAllocator { nullptr };
	CommandList* m_CommandList { nullptr };


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:
	
	
};

export class ComputeContext final :
	public RHIComputeContext<ComputeContext>,
	public CommandContext<ComputeContext> {

	friend class RHIComputeContext<ComputeContext>;
	friend class CommandContext<ComputeContext>;

public:
	ComputeContext(Device* Parent,RHIGPUMask InGPUIndex) :
		RHIComputeContext<ComputeContext>{},
		CommandContext<ComputeContext>{ Parent, InGPUIndex } {
	}

	~ComputeContext(void) = default;

public:


private:

	void Imp_Set_Scissor(const RHIRect2D& InScissor) {
		const D3D12_RECT Scissor {
			.left{static_cast<LONG>(InScissor.Offset.X)},
			.top{static_cast<LONG>(InScissor.Offset.Y)} ,
			.right{static_cast<LONG>(InScissor.Offset.X + InScissor.Extent.Width)},
			.bottom{static_cast<LONG>(InScissor.Offset.Y + InScissor.Extent.Height)}
		};



	}



private:

};

export class GraphicsContext final :
	public RHIGraphicsContext<GraphicsContext>,
	public CommandContext<GraphicsContext> {

	friend class RHIGraphicsContext<GraphicsContext>;
	friend class CommandContext<GraphicsContext>;

public:
	GraphicsContext(Device* Parent, RHIGPUMask InGPUIndex) :
		RHIGraphicsContext<GraphicsContext>{},
		CommandContext<GraphicsContext>{ Parent, InGPUIndex } {
	}

	~GraphicsContext(void) = default;

public:


private:



private:


};

export enum class CommandContextType :Uint8 {
	Compute,
	Graphics,
	Count
};

export template<CommandContextType _ContextType>
struct CommandContextTraits;

export template<>
struct CommandContextTraits<CommandContextType::Compute> final{
	using ContextType = ComputeContext;
};

export template<>
struct CommandContextTraits<CommandContextType::Graphics> final{
	using ContextType = GraphicsContext;
};


export template<CommandContextType _ContextType>
class CommandContextManager final :public DeviceChild, SingleNodeGPUObject{
public:
	using ContextType = typename CommandContextTraits<_ContextType>::ContextType;

	CommandContextManager(Device* Parent, RHIGPUMask InGPUIndex) :
		DeviceChild { Parent },
		SingleNodeGPUObject { InGPUIndex } {
	}
	~CommandContextManager(void) = default;

public:
	ContextType* Allocate(void) {
		{
			LockGuard<Mutex> Lock { this->m_Mutex };
			if (!this->m_AvailableContexts.empty()) {
				auto Context { this->m_AvailableContexts.front() };
				this->m_AvailableContexts.pop();

				Context->Reset();
				return Context;
			}
			else {
				this->m_ContextPool.emplace_back(MakeUnique<ContextType>(this->GetDevice(), this->Get_GPUMask()));

				return this->m_ContextPool.back().get();
			}
		}
	}


private:
	Mutex m_Mutex {};

	Vector<UniquePtr<ContextType>> m_ContextPool {};
	Queue<ContextType*> m_AvailableContexts {};
};