module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHID3D12 :StateCache;

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



export class StateCache final {
public:
	
	bool m_NeedScissorRects = true;
	Uint32 m_ScissorCount = 0;

	D3D12_RECT m_Scissor {};



	void Set_Scissor(const D3D12_RECT& InScissor) {

		//Add Check;
		

		this->m_Scissor = InScissor;
		this->m_NeedScissorRects = true;
		this->m_ScissorCount = 1;
	}

	

};