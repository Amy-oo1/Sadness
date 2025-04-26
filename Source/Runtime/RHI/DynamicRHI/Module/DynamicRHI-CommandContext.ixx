module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module DynamicRHI :CommandContext;

import Platform;
import Utility;
import Logger;



import RHI;


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
	RHIComputeContext() = default;

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
	RHIGraphicsContext() = default;

public:
	virtual ~RHIGraphicsContext(void) = default;


public:


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:


};