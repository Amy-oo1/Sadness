export module ApplicationContext:ApplicationContextBase;

import Utility;

import Root;


template<typename Derived>
struct RootTraits;

template<>
struct RootTraits<class ApplicationWin> {
	using RootTye = RootWin;
};


export template<typename Derived>
class ApplicationContextBase :Singleton {
	using RootTye = typename RootTraits<Derived>::RootTye;
protected://NOTE: Init 
	ApplicationContextBase(void) = default;

public:
	virtual ~ApplicationContextBase(void) = default;

protected:
	RootBase<RootTye>* m_Root{ nullptr };
};
