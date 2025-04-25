module;
#include "Runtime/Core/Platform/Include/PlatformWindowsWrapper.h"

#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Platform:Windows;

export HINSTANCE g_hInstance{ nullptr };

//NOTO :dont tans any platform api in here ,plase include include werper