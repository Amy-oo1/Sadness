module;
#include "Runtime/Core/Platform/Include/PlatformWindowsWrapper.h"
#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/String/Include/StringMacros.h"

export module Window:Windows;

import Platform;

import :Base;

export class WindowWin final :public WindowBase<WindowWin>{
	friend class WindowBase<WindowWin>;
private:
	WindowWin(void):
		WindowBase<WindowWin>{} {

		WNDCLASSEXW wcex{
			.cbSize = sizeof(WNDCLASSEXW),
			.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = WindowWin::WindowProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = g_hInstance,
			.hIcon = ::LoadIconW(g_hInstance, IDI_APPLICATION),
			.hCursor = ::LoadCursorW(g_hInstance, IDC_ARROW),
			.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1),
			.lpszMenuName = nullptr,
			.lpszClassName = _W("Sadness"),
			.hIconSm = ::LoadIconW(g_hInstance, IDI_APPLICATION)
		};

		ASSERT(0 != RegisterClassExW(&wcex));

		RECT Rect{ 0, 0, static_cast<LONG>(this->m_Width), static_cast<LONG>(this->m_Height) };

		ASSERT(::AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, FALSE));

		this->m_Hand = ::CreateWindowExW(
			0,
			_W("Sadness"),
			this->m_Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			Rect.right - Rect.left, Rect.bottom - Rect.top,
			nullptr,
			nullptr,
			g_hInstance,
			nullptr
		);

	}

public:
	~WindowWin(void) = default;

	static WindowWin* Get(void) {
		static WindowWin instance{};

		return &instance;
	}


private:
	HWND m_Hand{ nullptr };

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
};