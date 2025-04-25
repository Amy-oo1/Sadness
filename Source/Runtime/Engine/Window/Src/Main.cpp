
#include "Runtime/Core/Platform/Include/PlatformWindowsWrapper.h"

import Input_Module;

import Launch;


int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* pCmdLine, _In_ int nCmdShow)
{
	LaunchWindows(hInInstance, nCmdShow);

	// Your application code here
	return 0;
}