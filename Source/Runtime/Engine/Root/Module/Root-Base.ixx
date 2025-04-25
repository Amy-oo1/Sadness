export module Root :Base;

import Utility;
import Logger;
import Configer;
import FileSystemLayer;

import DynamicRHI;

import DynamicRHID3D12;//TODO Use Hong 

import Window;

//import :Forward;

//TODO :Module Window With Root has a circular dependency
//But it butter than include hong or add Get func
//So i has i idea  def in main module

export template<typename Derived>
struct WindowTraits;

template<>
struct WindowTraits<class RootWin> {
	using WindowType = WindowWin;
	using DynamicRHIType = DynamicRHID3D12;
};



export template<typename Derived>
class RootBase :public Singleton {
	using WindowType = typename WindowTraits<Derived>::WindowType;
	using DynamicRHIType = typename WindowTraits<Derived>::DynamicRHIType;
protected:
	RootBase(void) :
		Singleton{},
		m_Configer{ Configer::Get() },
		m_Logger{ SystemLogger::Get() },
		m_FileSystem{ FileSystem::Get() }
	{}

public:
	virtual ~RootBase(void) = default;

protected:
	Configer& m_Configer;
	SystemLogger& m_Logger;
	FileSystem& m_FileSystem;

	WindowBase<WindowType>* m_Window{ nullptr };

	DynamicRHI<DynamicRHIType>* m_DynamicRHI { nullptr };

};