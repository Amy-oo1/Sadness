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

//TODO :do not use this class  has too many base class ........
export template <typename Derived>
class CommandContext {
protected:
	CommandContext(void) = default;
public:
	virtual ~CommandContext(void) = default;
};

export class ComputeContext final :
	public DeviceChild,
	public RHIComputeContext<ComputeContext>,
	public CommandContext<ComputeContext> {

	friend class RHIComputeContext<ComputeContext>;
	friend class CommandContext<ComputeContext>;

public:
	ComputeContext(Device* Parent) :
		DeviceChild { Parent },
		RHIComputeContext<ComputeContext>{},
		CommandContext<ComputeContext>{} {
	}

	~ComputeContext(void) = default;

public:


private:



private:


};

export class GraphicsContext final :
	public DeviceChild,
	public RHIGraphicsContext<GraphicsContext>,
	public CommandContext<GraphicsContext> {

	friend class RHIGraphicsContext<GraphicsContext>;
	friend class CommandContext<GraphicsContext>;

public:
	GraphicsContext(Device* Parent) :
		DeviceChild { Parent },
		RHIGraphicsContext<GraphicsContext>{},
		CommandContext<GraphicsContext>{} {
	}

	~GraphicsContext(void) = default;

public:


private:



private:


};