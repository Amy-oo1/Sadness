export module Camera :Base;

import Platform;
import Utility;
import Math;

export template<typename Derived>
class CameraBase :MoveAbleOnly {
protected:
	CameraBase(void) :
		MoveAbleOnly {} 
	{}

public:
	virtual ~CameraBase(void) = default;

public:


private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:


};