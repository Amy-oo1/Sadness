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