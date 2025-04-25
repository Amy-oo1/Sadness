module;
#include "Runtime/Core/String/Include/StringMacros.h"

export module Window :Base;

//TODO UPlatform Window ,this is a Window In Windows Platform

import Utility;
import Platform;
import String;

export template<typename Derived>
class WindowBase :public Singleton {
protected:
	WindowBase(void) = default;

public:
	//TODO Move It
	virtual ~WindowBase(void) = default;


protected:
	Uint32 m_Width;
	Uint32 m_Height;

	//TODO Use UPlatform Str
	WString m_Title{ _W("Sadness") };

};