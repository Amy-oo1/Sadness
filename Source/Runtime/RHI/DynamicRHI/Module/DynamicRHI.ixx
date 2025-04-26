module;
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHI;

import Platform;
import Utility;
import Logger;

import RHI;

export import :Resource;
export import :CommandContext;

export template<typename Derived>
class DynamicRHI :public Singleton {
protected:
	DynamicRHI(void) = default;
public:
	virtual ~DynamicRHI(void) = default;

	//NOTE : this func should be implemented in the derived class
	/*static DynamicRHI& Get(void) {
		LOG_ERROR("DynamicRHI::Get() is not implemented.");
	}*/

public:
	void RHIPerInitialize(void) {
		this->Get_Derived()->Impl_RHIPerInitialize();
	}

	void RHIPostInitialize(void) {
		this->Get_Derived()->Impl_RHIPostInitialize();
	}

	void RHICreateSamplerState(const RHISamplerDesc& Desc) {
		this->Get_Derived()->Impl_RHICreateSamplerState(Desc);
	}


private:
	Derived* Get_Derived(void) {
		return static_cast<Derived*>(this);
	}

private:
	//NOTE: this func should be implemented in the derived class
	void Impl_RHIPerInitialize(void) {
		LOG_INFO("DynamicRHI::Impl_RHIPerInitialize()");
	}

	void Impl_RHIPostInitialize(void) {
		LOG_INFO("DynamicRHI::Impl_RHIPostInitialize()");
	}

	void Impl_RHICreateSamplerState(const RHISamplerDesc& Desc) {
		LOG_INFO("DynamicRHI::Impl_RHICreateSamplerState()");
	}

};