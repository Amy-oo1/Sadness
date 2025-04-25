#pragma once

#include<cassert>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<time.h>
#include<dxgi1_6.h>
#ifdef _DEBUG
#include<dxgidebug.h>
#endif // _DEBUG

#include<d3d12.h>
#include<d3dcompiler.h>
#include<wrl.h>

#include "ThirdParty/Windows/DirectX/Include/d3dx12.h"

using Microsoft::WRL::ComPtr;


#define D3D12_CHECK(x) { HRESULT hr = (x); if (FAILED(hr)) assert(false); };


