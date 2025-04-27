module;
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHI;

import Platform;
import Utility;
import Concurrent;
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
		this->Get_Derived()->Imp_RHIPerInitialize();
	}

	void RHIPostInitialize(void) {
		this->Get_Derived()->Imp_RHIPostInitialize();
	}

	void RHICreateStaticSampler(const RHISamplerDesc& StateSampler){
		this->Get_Derived()->Imp_RHICreateStaticSampler(StateSampler);
	}

	void RHICreateTextur2D(const RHITextrure2DDesc& Desc ,Uint8* pdata) {
		this->Get_Derived()->Imp_RHICreateTextur2D(Desc, pdata);
	}

private:
	Derived* Get_Derived(void) {
		return static_cast<Derived*>(this);
	}

private:
	//NOTE: this func should be implemented in the derived class
	void Imp_RHIPerInitialize(void) {
		LOG_INFO("DynamicRHI::Impl_RHIPerInitialize()");
	}

	void Imp_RHIPostInitialize(void) {
		LOG_INFO("DynamicRHI::Impl_RHIPostInitialize()");
	}

	void Imp_RHICreateStaticSampler(const Vector<RHISamplerDesc>& StateSamplers) {
		LOG_INFO("DynamicRHI::Impl_RHICreateStateSampler()");
	}

	void Imp_RHICreateTextur2D(const RHITextrure2DDesc& Desc, Uint8* pdata) {
		LOG_INFO("DynamicRHI::Impl_RHICreateTextur2D()");
	}


};