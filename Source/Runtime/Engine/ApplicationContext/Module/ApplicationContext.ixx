export module ApplicationContext;

export import :ApplicationContextBase;
export import :Windows;


export using CurrentApp = ApplicationWin;//TODO

export ApplicationContextBase<CurrentApp>* g_APP{ nullptr };