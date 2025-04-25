export module ApplicationContext:Windows;

import Root;

import :ApplicationContextBase;

export class ApplicationWin final :public ApplicationContextBase<ApplicationWin> {
	friend class ApplicationContextBase<ApplicationWin>;
private:
	ApplicationWin(void) :
		ApplicationContextBase<ApplicationWin> {} {

		this->m_Root = RootWin::Get();
	}

public:
	~ApplicationWin(void) = default;

public:
	static ApplicationWin* Get(void) {
		static ApplicationWin instance{};

		return &instance;
	}

public:

};

