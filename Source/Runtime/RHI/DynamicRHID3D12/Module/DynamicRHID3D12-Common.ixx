module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

export module DynamicRHID3D12 :Common;

import Platform;
import Utility;
import Container;

import RHI;

import :Forward;

//NOTE : these class not base Utility ,so you kan use it to cor
//So you can not use virtual ~classname

//NOTE :Here has a bug ,con func must be public not prot
//TODO :Move not One Device is not a good idea
export class DeviceChild {
protected:
	DeviceChild(Device* Parent) :
		m_Device(Parent)
	{}

	Device* GetDevice(void) const {
		return this->m_Device;
	}

	void SetDevice(Device* Parent) {
		this->m_Device = Parent;
	}
protected:
	Device* m_Device { nullptr };

};

class GPUObject {
protected:
	GPUObject(RHIGPUMask InGPUMask,RHIGPUMask VisibiltyMask):
		m_GPUMask(InGPUMask),
		m_VisibiltyMask(VisibiltyMask)
	{}
	
protected:
	[[nodiscard]] const RHIGPUMask& Get_GPUMask(void) const {
		return this->m_GPUMask;
	}

	[[nodiscard]] const RHIGPUMask& Get_VisibiltyMask(void) const {
		return this->m_VisibiltyMask;
	}

private:
	const RHIGPUMask m_GPUMask;
	const RHIGPUMask m_VisibiltyMask;

};

export class SingleNodeGPUObject : public GPUObject {
protected:
	SingleNodeGPUObject(RHIGPUMask InGPUMask) :
		GPUObject(InGPUMask, InGPUMask)
	{}
};

export class MultiNodeGPUObject : public GPUObject {
protected:
	MultiNodeGPUObject(RHIGPUMask InGPUMask, RHIGPUMask VisibiltyMask) :
		GPUObject(InGPUMask, VisibiltyMask)
	{}
};

export class D3D12ResourceDescBuilder final {
public:
	D3D12ResourceDescBuilder(void) = default;

public:
	D3D12ResourceDescBuilder& SetDimension(D3D12_RESOURCE_DIMENSION dimension) {
		this->m_Desc.Dimension = dimension;
		return *this;
	}
	D3D12ResourceDescBuilder& SetWidth(Uint64 width) {
		this->m_Desc.Width = width;
		return *this;
	}
	D3D12ResourceDescBuilder& SetHeight(Uint32 height) {
		this->m_Desc.Height = height;
		return *this;
	}
	D3D12ResourceDescBuilder& SetDepthOrArraySize(Uint16 depthOrArraySize) {
		this->m_Desc.DepthOrArraySize = depthOrArraySize;
		return *this;
	}
	D3D12ResourceDescBuilder& SetMipLevels(Uint16 mipLevels) {
		this->m_Desc.MipLevels = mipLevels;
		return *this;
	}
	D3D12ResourceDescBuilder& SetFormat(DXGI_FORMAT format) {
		this->m_Desc.Format = format;
		return *this;
	}
	D3D12ResourceDescBuilder& SetSampleCount(Uint32 sampleCount) {
		this->m_Desc.SampleDesc.Count = sampleCount;
		return *this;
	}
	D3D12ResourceDescBuilder& SetLayout(D3D12_TEXTURE_LAYOUT layout) {
		this->m_Desc.Layout = layout;
		return *this;
	}
	D3D12ResourceDescBuilder& SetFlags(D3D12_RESOURCE_FLAGS flags) {
		this->m_Desc.Flags = flags;
		return *this;
	}

	[[nodiscard]] D3D12_RESOURCE_DESC Build(void) const {
		return this->m_Desc;
	}
private:
	D3D12_RESOURCE_DESC m_Desc {
		.Dimension { D3D12_RESOURCE_DIMENSION_TEXTURE2D },
		.Alignment { 0 },//NOTE :System will set this
		.Width { 0 },
		.Height { 0 },
		.DepthOrArraySize { 1 },
		.MipLevels { 1 },
		.Format { DXGI_FORMAT_UNKNOWN },
		.SampleDesc {.Count{ 1 }, .Quality{ 0 } },
		.Layout { D3D12_TEXTURE_LAYOUT_UNKNOWN },
		.Flags { D3D12_RESOURCE_FLAG_NONE },
	};
};