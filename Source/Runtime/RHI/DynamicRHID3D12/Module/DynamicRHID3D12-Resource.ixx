module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :Resource;

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


class Resource :public MoveAbleOnly, public DeviceChild, public SingleNodeGPUObject {
public:
	
};



export class ResourceBarriers final : public MoveAbleOnly {
public:
	ResourceBarriers(void) = default;

	~ResourceBarriers(void) = default;



private:
	Vector<D3D12_RESOURCE_BARRIER> m_Barriers;
};