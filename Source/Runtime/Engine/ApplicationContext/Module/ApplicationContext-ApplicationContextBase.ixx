module;
#include "Runtime/Core/Logger/Include/LogMacros.h"

export module ApplicationContext:ApplicationContextBase;

import Utility;

import Logger;

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


	void StartUp(void) {
		this->Get_Derived()->Imp_StartUp();
	}

	void CleanUp(void) {
		this->Get_Derived()->Imp_CleanUp();
	}

	bool Is_Done(void) {
		return this->Get_Derived()->Imp_Is_Done();
	}

	void Update(float deltaT) {
		this->Get_Derived()->Imp_Update(deltaT);
	}

	void RenderScene(void) {
		this->Get_Derived()->Imp_RenderScene();
	}

protected:
	RootBase<RootTye>* m_Root{ nullptr };

private:
	Derived* Get_Derived(void) { return static_cast<Derived*>(this); }

private:
	void Imp_StartUp(void) {
		LOG_ERROR("ApplicationContextBase::Imp_StartUp()");
	}

	void Imp_CleanUp(void) {
		LOG_ERROR("ApplicationContextBase::Imp_CleanUp()");
	}

	bool Imp_Is_Done(void) {
		LOG_ERROR("ApplicationContextBase::Imp_Is_Done()");

		return false;
	}

	void Imp_Update(float deltaT) {
		LOG_ERROR("ApplicationContextBase::Imp_Update()");
	}

	void Imp_RenderScene(void) {
		LOG_ERROR("ApplicationContextBase::Imp_RenderScene()");
	}

};
