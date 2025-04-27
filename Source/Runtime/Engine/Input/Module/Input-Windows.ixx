module;
#include "Runtime/Engine/Input/Include/XinputWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module Input :Windows;



import Platform;
import Utility;
import Container;
import Math;
import Logger;

import :Base;

namespace Input {

	inline float FilterAnalogInput(int val, int deadZone){
		if (val < 0)
		{
			if (val > -deadZone)
				return 0.0f;
			else
				return (val + deadZone) / (32768.0f - deadZone);
		}
		else
		{
			if (val < deadZone)
				return 0.0f;
			else
				return (val - deadZone) / (32767.0f - deadZone);
		}
	}


	IDirectInput8A* s_DI;
	IDirectInputDevice8A* s_Keyboard;
	IDirectInputDevice8A* s_Mouse;

	DIMOUSESTATE2 s_MouseState;

	Array<Uint8, 256> s_Keybuffer;
	Array<Uint8, Tounderlying(DigitalInput::kNumKeys)> s_DXKeyMapping;
	
	HWND s_hWnd;


	void KbmBuildKeyMapping(void) {
		using enum DigitalInput;

		s_DXKeyMapping[Tounderlying(kKey_escape)] = DIK_ESCAPE;
		s_DXKeyMapping[Tounderlying(kKey_1)] = DIK_1;
		s_DXKeyMapping[Tounderlying(kKey_2)] = DIK_2;
		s_DXKeyMapping[Tounderlying(kKey_3)] = DIK_3;
		s_DXKeyMapping[Tounderlying(kKey_4)] = DIK_4;
		s_DXKeyMapping[Tounderlying(kKey_5)] = DIK_5;
		s_DXKeyMapping[Tounderlying(kKey_6)] = DIK_6;
		s_DXKeyMapping[Tounderlying(kKey_7)] = DIK_7;
		s_DXKeyMapping[Tounderlying(kKey_8)] = DIK_8;
		s_DXKeyMapping[Tounderlying(kKey_9)] = DIK_9;
		s_DXKeyMapping[Tounderlying(kKey_0)] = DIK_0;
		s_DXKeyMapping[Tounderlying(kKey_minus)] = DIK_MINUS;
		s_DXKeyMapping[Tounderlying(kKey_equals)] = DIK_EQUALS;
		s_DXKeyMapping[Tounderlying(kKey_back)] = DIK_BACK;
		s_DXKeyMapping[Tounderlying(kKey_tab)] = DIK_TAB;
		s_DXKeyMapping[Tounderlying(kKey_q)] = DIK_Q;
		s_DXKeyMapping[Tounderlying(kKey_w)] = DIK_W;
		s_DXKeyMapping[Tounderlying(kKey_e)] = DIK_E;
		s_DXKeyMapping[Tounderlying(kKey_r)] = DIK_R;
		s_DXKeyMapping[Tounderlying(kKey_t)] = DIK_T;
		s_DXKeyMapping[Tounderlying(kKey_y)] = DIK_Y;
		s_DXKeyMapping[Tounderlying(kKey_u)] = DIK_U;
		s_DXKeyMapping[Tounderlying(kKey_i)] = DIK_I;
		s_DXKeyMapping[Tounderlying(kKey_o)] = DIK_O;
		s_DXKeyMapping[Tounderlying(kKey_p)] = DIK_P;
		s_DXKeyMapping[Tounderlying(kKey_lbracket)] = DIK_LBRACKET;
		s_DXKeyMapping[Tounderlying(kKey_rbracket)] = DIK_RBRACKET;
		s_DXKeyMapping[Tounderlying(kKey_return)] = DIK_RETURN;
		s_DXKeyMapping[Tounderlying(kKey_lcontrol)] = DIK_LCONTROL;
		s_DXKeyMapping[Tounderlying(kKey_a)] = DIK_A;
		s_DXKeyMapping[Tounderlying(kKey_s)] = DIK_S;
		s_DXKeyMapping[Tounderlying(kKey_d)] = DIK_D;
		s_DXKeyMapping[Tounderlying(kKey_f)] = DIK_F;
		s_DXKeyMapping[Tounderlying(kKey_g)] = DIK_G;
		s_DXKeyMapping[Tounderlying(kKey_h)] = DIK_H;
		s_DXKeyMapping[Tounderlying(kKey_j)] = DIK_J;
		s_DXKeyMapping[Tounderlying(kKey_k)] = DIK_K;
		s_DXKeyMapping[Tounderlying(kKey_l)] = DIK_L;
		s_DXKeyMapping[Tounderlying(kKey_semicolon)] = DIK_SEMICOLON;
		s_DXKeyMapping[Tounderlying(kKey_apostrophe)] = DIK_APOSTROPHE;
		s_DXKeyMapping[Tounderlying(kKey_grave)] = DIK_GRAVE;
		s_DXKeyMapping[Tounderlying(kKey_lshift)] = DIK_LSHIFT;
		s_DXKeyMapping[Tounderlying(kKey_backslash)] = DIK_BACKSLASH;
		s_DXKeyMapping[Tounderlying(kKey_z)] = DIK_Z;
		s_DXKeyMapping[Tounderlying(kKey_x)] = DIK_X;
		s_DXKeyMapping[Tounderlying(kKey_c)] = DIK_C;
		s_DXKeyMapping[Tounderlying(kKey_v)] = DIK_V;
		s_DXKeyMapping[Tounderlying(kKey_b)] = DIK_B;
		s_DXKeyMapping[Tounderlying(kKey_n)] = DIK_N;
		s_DXKeyMapping[Tounderlying(kKey_m)] = DIK_M;
		s_DXKeyMapping[Tounderlying(kKey_comma)] = DIK_COMMA;
		s_DXKeyMapping[Tounderlying(kKey_period)] = DIK_PERIOD;
		s_DXKeyMapping[Tounderlying(kKey_slash)] = DIK_SLASH;
		s_DXKeyMapping[Tounderlying(kKey_rshift)] = DIK_RSHIFT;
		s_DXKeyMapping[Tounderlying(kKey_multiply)] = DIK_MULTIPLY;
		s_DXKeyMapping[Tounderlying(kKey_lalt)] = DIK_LALT;
		s_DXKeyMapping[Tounderlying(kKey_space)] = DIK_SPACE;
		s_DXKeyMapping[Tounderlying(kKey_capital)] = DIK_CAPITAL;
		s_DXKeyMapping[Tounderlying(kKey_f1)] = DIK_F1;
		s_DXKeyMapping[Tounderlying(kKey_f2)] = DIK_F2;
		s_DXKeyMapping[Tounderlying(kKey_f3)] = DIK_F3;
		s_DXKeyMapping[Tounderlying(kKey_f4)] = DIK_F4;
		s_DXKeyMapping[Tounderlying(kKey_f5)] = DIK_F5;
		s_DXKeyMapping[Tounderlying(kKey_f6)] = DIK_F6;
		s_DXKeyMapping[Tounderlying(kKey_f7)] = DIK_F7;
		s_DXKeyMapping[Tounderlying(kKey_f8)] = DIK_F8;
		s_DXKeyMapping[Tounderlying(kKey_f9)] = DIK_F9;
		s_DXKeyMapping[Tounderlying(kKey_f10)] = DIK_F10;
		s_DXKeyMapping[Tounderlying(kKey_numlock)] = DIK_NUMLOCK;
		s_DXKeyMapping[Tounderlying(kKey_scroll)] = DIK_SCROLL;
		s_DXKeyMapping[Tounderlying(kKey_numpad7)] = DIK_NUMPAD7;
		s_DXKeyMapping[Tounderlying(kKey_numpad8)] = DIK_NUMPAD8;
		s_DXKeyMapping[Tounderlying(kKey_numpad9)] = DIK_NUMPAD9;
		s_DXKeyMapping[Tounderlying(kKey_subtract)] = DIK_SUBTRACT;
		s_DXKeyMapping[Tounderlying(kKey_numpad4)] = DIK_NUMPAD4;
		s_DXKeyMapping[Tounderlying(kKey_numpad5)] = DIK_NUMPAD5;
		s_DXKeyMapping[Tounderlying(kKey_numpad6)] = DIK_NUMPAD6;
		s_DXKeyMapping[Tounderlying(kKey_add)] = DIK_ADD;
		s_DXKeyMapping[Tounderlying(kKey_numpad1)] = DIK_NUMPAD1;
		s_DXKeyMapping[Tounderlying(kKey_numpad2)] = DIK_NUMPAD2;
		s_DXKeyMapping[Tounderlying(kKey_numpad3)] = DIK_NUMPAD3;
		s_DXKeyMapping[Tounderlying(kKey_numpad0)] = DIK_NUMPAD0;
		s_DXKeyMapping[Tounderlying(kKey_decimal)] = DIK_DECIMAL;
		s_DXKeyMapping[Tounderlying(kKey_f11)] = DIK_F11;
		s_DXKeyMapping[Tounderlying(kKey_f12)] = DIK_F12;
		s_DXKeyMapping[Tounderlying(kKey_numpadenter)] = DIK_NUMPADENTER;
		s_DXKeyMapping[Tounderlying(kKey_rcontrol)] = DIK_RCONTROL;
		s_DXKeyMapping[Tounderlying(kKey_divide)] = DIK_DIVIDE;
		s_DXKeyMapping[Tounderlying(kKey_sysrq)] = DIK_SYSRQ;
		s_DXKeyMapping[Tounderlying(kKey_ralt)] = DIK_RALT;
		s_DXKeyMapping[Tounderlying(kKey_pause)] = DIK_PAUSE;
		s_DXKeyMapping[Tounderlying(kKey_home)] = DIK_HOME;
		s_DXKeyMapping[Tounderlying(kKey_up)] = DIK_UP;
		s_DXKeyMapping[Tounderlying(kKey_pgup)] = DIK_PRIOR;
		s_DXKeyMapping[Tounderlying(kKey_left)] = DIK_LEFT;
		s_DXKeyMapping[Tounderlying(kKey_right)] = DIK_RIGHT;
		s_DXKeyMapping[Tounderlying(kKey_end)] = DIK_END;
		s_DXKeyMapping[Tounderlying(kKey_down)] = DIK_DOWN;
		s_DXKeyMapping[Tounderlying(kKey_pgdn)] = DIK_NEXT;
		s_DXKeyMapping[Tounderlying(kKey_insert)] = DIK_INSERT;
		s_DXKeyMapping[Tounderlying(kKey_delete)] = DIK_DELETE;
		s_DXKeyMapping[Tounderlying(kKey_lwin)] = DIK_LWIN;
		s_DXKeyMapping[Tounderlying(kKey_rwin)] = DIK_RWIN;
		s_DXKeyMapping[Tounderlying(kKey_apps)] = DIK_APPS;

	}

	void KbmZeroInputs(){
		memset(&s_MouseState, 0, sizeof(DIMOUSESTATE2));
		memset(s_Keybuffer.data(), 0, sizeof(s_Keybuffer));
	}

	void KbmInitialize(HWND hWnd) {
		KbmBuildKeyMapping();

		s_hWnd = hWnd;

		if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&s_DI, nullptr)))
			ASSERT(false/*, "DirectInput8 initialization failed."*/);

		if (FAILED(s_DI->CreateDevice(GUID_SysKeyboard, &s_Keyboard, nullptr)))
			ASSERT(false /*"Keyboard CreateDevice failed."*/);
		if (FAILED(s_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
			ASSERT(false /*"Keyboard SetDataFormat failed."*/);
		if (FAILED(s_Keyboard->SetCooperativeLevel(s_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			ASSERT(false/* "Keyboard SetCooperativeLevel failed."*/);

	
		DIPROPDWORD dipdw {};
		{
			dipdw.diph.dwSize = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj = 0;
			dipdw.diph.dwHow = DIPH_DEVICE;
			dipdw.dwData = 10;
		}
		if (FAILED(s_Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
			ASSERT(false/* "Keyboard set buffer size failed."*/);

		if (FAILED(s_DI->CreateDevice(GUID_SysMouse, &s_Mouse, nullptr)))
			ASSERT(false/* "Mouse CreateDevice failed."*/);
		if (FAILED(s_Mouse->SetDataFormat(&c_dfDIMouse2)))
			ASSERT(false/* "Mouse SetDataFormat failed."*/);
		if (FAILED(s_Mouse->SetCooperativeLevel(s_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			ASSERT(false/* "Mouse SetCooperativeLevel failed."*/);

		KbmZeroInputs();
	}

	void KbmUpdate(void) {
		HWND foreground = GetForegroundWindow();
		bool visible = IsWindowVisible(foreground) != 0;

		if (foreground != s_hWnd // wouldn't be able to acquire
			|| !visible)
		{
			KbmZeroInputs();
		}
		else
		{
			s_Mouse->Acquire();
			s_Mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &s_MouseState);
			s_Keyboard->Acquire();
			s_Keyboard->GetDeviceState(sizeof(s_Keybuffer), s_Keybuffer.data());
		}

	}

	void KbmShutdown(void) {
		if (s_Keyboard) {
			s_Keyboard->Unacquire();
			s_Keyboard->Release();
			s_Keyboard = nullptr;
		}
		if (s_Mouse) {
			s_Mouse->Unacquire();
			s_Mouse->Release();
			s_Mouse = nullptr;
		}
		if (s_DI) {
			s_DI->Release();
			s_DI = nullptr;
		}
	}


	export void Initialize(HWND hWnd) {
		memset(s_Keybuffer.data(), 0, sizeof(s_Keybuffer._Elems));
		memset(s_Analogs.data(), 0, sizeof(s_Analogs._Elems));

		KbmInitialize(hWnd);
	}

	export void Shutdown(void) {
		KbmShutdown();
	}


	export void Update(float frameDelta){
		memcpy(s_Buttons[1].data(), s_Buttons[0].data(), sizeof(s_Buttons[0]));
		memset(s_Buttons[0].data(), 0, sizeof(s_Buttons[0]));
		memset(s_Analogs.data(), 0, sizeof(s_Analogs));
		
		using enum DigitalInput;

		XINPUT_STATE newInputState;
		if (ERROR_SUCCESS == XInputGetState(0, &newInputState)) {

#define SET_BUTTON_VALUE(InputEnum, GameInputMask) \
        s_Buttons[0][Tounderlying(InputEnum)] = !!(newInputState.Gamepad.wButtons & GameInputMask);
		

			SET_BUTTON_VALUE(kDPadUp, XINPUT_GAMEPAD_DPAD_UP);
			SET_BUTTON_VALUE(kDPadDown, XINPUT_GAMEPAD_DPAD_DOWN);
			SET_BUTTON_VALUE(kDPadLeft, XINPUT_GAMEPAD_DPAD_LEFT);
			SET_BUTTON_VALUE(kDPadRight, XINPUT_GAMEPAD_DPAD_RIGHT);
			SET_BUTTON_VALUE(kStartButton, XINPUT_GAMEPAD_START);
			SET_BUTTON_VALUE(kBackButton, XINPUT_GAMEPAD_BACK);
			SET_BUTTON_VALUE(kLThumbClick, XINPUT_GAMEPAD_LEFT_THUMB);
			SET_BUTTON_VALUE(kRThumbClick, XINPUT_GAMEPAD_RIGHT_THUMB);
			SET_BUTTON_VALUE(kLShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER);
			SET_BUTTON_VALUE(kRShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER);
			SET_BUTTON_VALUE(kAButton, XINPUT_GAMEPAD_A);
			SET_BUTTON_VALUE(kBButton, XINPUT_GAMEPAD_B);
			SET_BUTTON_VALUE(kXButton, XINPUT_GAMEPAD_X);
			SET_BUTTON_VALUE(kYButton, XINPUT_GAMEPAD_Y);

#undef SET_BUTTON_VALUE

			s_Analogs[kAnalogLeftTrigger] = newInputState.Gamepad.bLeftTrigger / 255.0f;
			s_Analogs[kAnalogRightTrigger] = newInputState.Gamepad.bRightTrigger / 255.0f;
			s_Analogs[kAnalogLeftStickX] = FilterAnalogInput(newInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			s_Analogs[kAnalogLeftStickY] = FilterAnalogInput(newInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			s_Analogs[kAnalogRightStickX] = FilterAnalogInput(newInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			s_Analogs[kAnalogRightStickY] = FilterAnalogInput(newInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}

		KbmUpdate();

		for (Uint32 i = 0; i < Tounderlying(kNumKeys); ++i)
		{
			s_Buttons[0][i] = (s_Keybuffer[s_DXKeyMapping[i]] & 0x80) != 0;
		}

		for (Uint32 i = 0; i < 8; ++i)
		{
			if (s_MouseState.rgbButtons[i] > 0) s_Buttons[0][Tounderlying(kMouse0 + i)] = true;
		}

		s_Analogs[kAnalogMouseX] = (float)s_MouseState.lX * .0018f;
		s_Analogs[kAnalogMouseY] = (float)s_MouseState.lY * -.0018f;

		if (s_MouseState.lZ > 0)
			s_Analogs[kAnalogMouseScroll] = 1.0f;
		else if (s_MouseState.lZ < 0)
			s_Analogs[kAnalogMouseScroll] = -1.0f;

		// Update time duration for buttons pressed
		for (Uint32 i = 0; i < Tounderlying(kNumDigitalInputs); ++i)
		{
			if (s_Buttons[0][i])
			{
				if (!s_Buttons[1][i])
					s_HoldDuration[i] = 0.0f;
				else
					s_HoldDuration[i] += frameDelta;
			}
		}

		for (Uint32 i = 0; i < kNumAnalogInputs; ++i)
		{
			s_AnalogsTC[i] = s_Analogs[i] * frameDelta;
		}

	}

	export bool IsAnyPressed(void)
	{
		
		for(const auto&flag : s_Buttons[0]){
			if (flag)
				return true; 
		}

		return false;
	}

	export bool IsPressed(DigitalInput di)
	{
		return s_Buttons[0][Tounderlying(di)];
	}

	export bool IsFirstPressed(DigitalInput di)
	{
		return s_Buttons[0][Tounderlying(di)] && !s_Buttons[1][Tounderlying(di)];
	}

	export bool IsReleased(DigitalInput di)
	{
		return !s_Buttons[0][Tounderlying(di)];
	}

	export bool IsFirstReleased(DigitalInput di)
	{
		return !s_Buttons[0][Tounderlying(di)] && s_Buttons[1][Tounderlying(di)];
	}

	export float GetDurationPressed(DigitalInput di)
	{
		return s_HoldDuration[Tounderlying(di)];
	}

	export float GetAnalogInput(AnalogInput ai)
	{
		return s_Analogs[ai];
	}

	export float GetTimeCorrectedAnalogInput(AnalogInput ai)
	{
		return s_AnalogsTC[ai];
	}
}