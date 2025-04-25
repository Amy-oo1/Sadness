module;
#include "Runtime/Core/Platform/Include/PlatformWindowsWrapper.h"

#include "Runtime/Core/String/Include/StringMacros.h"

export module Launch :Windows;

import Platform;
import Utility;

import ApplicationContext;

import :Universal;

export int LaunchWindows(HINSTANCE hInInstance, int nCmdShow) {
	if (false == DirectX::XMVerifyCPUSupport()) {

		//altoough Logger no dependency is available, but i do not want to do yet
		// we can use the Windows API to log the error
		::MessageBoxW(nullptr, _W("CPU does not support SSE2"), _W("Error"), MB_OK | MB_ICONERROR);
		
		return -1;
	}

	int argc{ 0 };
	LPWSTR* argv{ CommandLineToArgvW(GetCommandLineW(), &argc) };
	//TODO Use Command 
	if(argc>1)
		::MessageBoxW(nullptr, _W("Command is TODO"), _W("Error"), MB_OK | MB_ICONERROR);

	
	//Mybe This Is Guard Main
	g_hInstance = hInInstance;
	
	g_APP = ApplicationWin::Get();


	//Becase Static ,So not has Work in Here
	//TODO my ErrCode Shoulde Change to Platform Error Code
	return Tounderlying(Launch());
}