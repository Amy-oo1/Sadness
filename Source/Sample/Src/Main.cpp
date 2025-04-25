
#include "Runtime/Core/Platform/Include/PlatformWindowsWrapper.h"

import Launch;


int WINAPI wWinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ wchar_t* pCmdLine, _In_ int nCmdShow)
{
	int Result{ LaunchWindows(hInInstance, nCmdShow) };

	return Result;
}