module;
#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "Runtime/Core/Utility/Include/UtilityMacros.h"
#include "Runtime/Core/String/Include/StringMacros.h"

export module DynamicRHID3D12 :TextureManager;

import Platform;
import Utility;
import Container;
import Concurrent;
import String;
import FileSystemLayer;

import RHI;
import DynamicRHI;

import D3D12Definition;

import :Forward;
import :Common;
import :Device;
import :Descriptor;
import :GPUNode;
import :Fence;

export class TextureState final : public MoveAbleOnly, public DeviceChild {
public:
	TextureState(Device * Parent):
		MoveAbleOnly {},
		DeviceChild { Parent }
	{}

	~TextureState() { this->Resouce.m_Native->Release(); }

	RHITexture Resouce;
	TString FileName;
	bool Is_Vaild;
	bool m_Is_Loading;

	void WaitForLoad()  {
		while(!static_cast<volatile bool&>(this->m_Is_Loading))
			ThisThrad::yield();	
	}

	
};

export class TextureManager final :public MoveAbleOnly, public DeviceChild {
private:
	TextureManager(Device* parent) :
		DeviceChild { parent }
	{}

public:
	~TextureManager(void) = default;

public:
	TextureState* LoadTexture(TString FileName, const D3D12_RESOURCE_DESC& Desc,RHIGPUMask InGPUIndex) {
		TextureState* Re { nullptr };

		{
			LockGuard<Mutex> lock { m_Mutex };

			if (auto It { this->m_TextureMap.find(FileName) };It!=this->m_TextureMap.end()) {
				auto Tex { It->second };
				Tex->WaitForLoad();

				return Tex.get();
			}
			else {
				this->m_TextureMap[FileName] = MakeShared<TextureState>(this->m_Device);

				Re = this->m_TextureMap[FileName].get();
			}
		}

		Path file = { this->m_RootPath / FileName / _W(".DDS") };
		auto [Data,DataSize] { FileSystem::Get().RenderFileSync(file) };

		this->CreateGPUResource(Data, DataSize, Desc, InGPUIndex);

		//TODO Set Texture Data 

	}


private:
	ID3D12Resource* CreateGPUResource(const Uint8* Data, Size size,const D3D12_RESOURCE_DESC& Desc, RHIGPUMask InGPUIndex);


private:

	Path m_RootPath { _W("") };

	Mutex m_Mutex {};
	UnorderedMap<TString, SharedPtr<TextureState>> m_TextureMap {};

};