module;
#include "d3d12.h"

export module ApplicationContext:Windows;

import Render;

import Root;

import Camera;
import Input;
import CameraController;

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

	void Initialize_Render(void);


private:
	void Imp_StartUp(void);


private:
	Camera m_Camera {};
	ShadowCamera m_ShadowCamera {};
	FlyingCameraController m_CameraController { this->m_Camera };
	//OrbitCameraController m_OrbitCameraController { this->m_Camera };

	D3D12_VIEWPORT m_MainViewport;
	D3D12_RECT m_MainScissor;

};


void ApplicationWin::Initialize_Render(void){
	RenderSystem::CreateStaticSamplers();

	auto* RefRHI { this->m_Root->m_DynamicRHI };

	RefRHI->RHICreateStaticSampler(RenderSystem::g_LinearWrapDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_AnisoWrapDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_ShadowDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_LinearClampDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_VolumeWrapDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_PointClampDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_LinearBorderDesc);
	RefRHI->RHICreateStaticSampler(RenderSystem::g_PointBorderDesc);
	

	


	


	/*this->m_Root->m_DynamicRHI->RHICreateStaticSampler()*/
}

void ApplicationWin::Imp_StartUp(void){
	this->Initialize_Render();
}

