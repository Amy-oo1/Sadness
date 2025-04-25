export module Root :Windows;

import Container;

import DynamicRHID3D12;

import Window;

import :Base;

export class RootWin final :public RootBase<RootWin> {
	friend class RootBase<RootWin>;
private:
	RootWin(void):
		RootBase<RootWin> {}{
		
		this->m_Window =WindowWin::Get();
		this->m_DynamicRHI = DynamicRHID3D12::Get();
	}

public:
	~RootWin(void) = default;

public:
	static RootWin* Get(void) {
		static RootWin instance{};

		return &instance;
	}

};