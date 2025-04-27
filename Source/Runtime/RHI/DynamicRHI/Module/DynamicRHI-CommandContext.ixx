module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHI :CommandContext;

import Platform;
import Utility;
import Logger;



import RHI;


//NOTE :Some Platform is not FSL
export template <typename Derived>
class RHICommandContext :public MoveAbleOnly{
protected:
	RHICommandContext(void) = default;

public:
	virtual ~RHICommandContext(void) = default;

public:


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:


};

export template <typename Derived>
class RHIComputeContext : public RHICommandContext<Derived> {
    friend class RHICommandContext<Derived>;
protected:
	RHIComputeContext(void) = default;

public:
	virtual ~RHIComputeContext(void) = default;


public:


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:


};

export template <typename Derived>
class RHIGraphicsContext : public RHICommandContext<Derived> {
	friend class RHICommandContext<Derived>;
protected:
	RHIGraphicsContext(void) = default;

public:
	virtual ~RHIGraphicsContext(void) = default;


public:

	void Set_Scissor(const RHIRect2D& InScissor) {
		Get_Derived()->Imp_Set_Scissor(InScissor);
	}


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:

	void Imp_Set_Scissor(const RHIRect2D& InScissor) {
		LOG_INFO("Imp_Set_Scissor::RHIRect2D");
	}


};