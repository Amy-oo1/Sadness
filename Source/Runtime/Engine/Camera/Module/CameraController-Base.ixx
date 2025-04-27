module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

#include "Runtime/Core/Logger/Include/LogMacros.h"

export module CameraController :Base;

import Platform;
import Utility;
import Math;
import Logger;

import Camera;

using namespace Math;

export template<typename Derived>
class CameraControllerBase :public MoveAbleOnly {
protected:
	CameraControllerBase(Camera& targetCamera)
		:m_TargetCamera { targetCamera } {
	}

	virtual ~CameraControllerBase(void) = default;

public:
	void Updata(float dt) {
		this->Get_Derived()->Imp_Update(dt);
	}

	static void ApplyMomentum(float& oldValue, float& newValue, float deltaTime);

protected:
	Camera& m_TargetCamera;

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:
	void Imp_Update(float dt) {
		LOG_ERROR("CameraControllerBase::Imp_Update() Not Implemented");
	}


};

template<typename Derived>
void CameraControllerBase<Derived>::ApplyMomentum(float& oldValue, float& newValue, float deltaTime){
	float blendedValue {};

	if (Abs(newValue) > Abs(oldValue))
		blendedValue = Lerp(newValue, oldValue, Pow(0.6f, deltaTime * 60.0f));
	else
		blendedValue = Lerp(newValue, oldValue, Pow(0.8f, deltaTime * 60.0f));
	
	oldValue = blendedValue;
	newValue = blendedValue;

}