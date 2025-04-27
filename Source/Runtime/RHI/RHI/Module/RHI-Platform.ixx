module;
//#include "Runtime/RHI/DynamicRHID3D12/Include/D3D12APIWrapper.h"

#include "d3d12.h"
#include "ThirdParty/Vulkan/Include/vulkan/vulkan.h"

export module RHI:Platform;

import Platform;
import Utility;
import Container;


//NOTE : Here just Traits some enum but struct in func to change

export struct D3D12Tag final{
    using PixelFormat = DXGI_FORMAT;
	using TextureFlags = D3D12_RESOURCE_FLAGS;

    using Filter = D3D12_FILTER;
	using AddressMode = D3D12_TEXTURE_ADDRESS_MODE;
	using CompareFunc = D3D12_COMPARISON_FUNC;

	using ResourcePtr = ID3D12Resource*;
	using ResourceStates = D3D12_RESOURCE_STATES;

    using BlendFactor = D3D12_BLEND;

};

export struct WarperVkFilter final {
    VkFilter MinFilter;
    VkFilter MagFilter;
    VkSamplerMipmapMode MipFilter;
};

export struct VulkanTag final{
	using PixelFormat = VkFormat;
	using TextrueFlags = VkImageUsageFlags;

    using Filter = WarperVkFilter;
	using AddressMode = VkSamplerAddressMode;
	using CompareFunc = VkCompareOp;

	using BufferPtr = VkBuffer;
	using BufferStates = VkBufferUsageFlags;

	using TexturePtr = VkImage;
	using TextureStates = VkImageUsageFlags;

    using BlendFactor = VkBlendFactor;

};

export template<typename APITag>
struct RHITypeTraits;

export template<>
struct RHITypeTraits<D3D12Tag> {
	using PixelFormat = D3D12Tag::PixelFormat;
	using TextureFlags = D3D12Tag::TextureFlags;

    using Filter = D3D12Tag::Filter;
	using AddressMode = D3D12Tag::AddressMode;
	using CompareFunc = D3D12Tag::CompareFunc;

	using BufferResourcePtr = D3D12Tag::ResourcePtr;//In Vulkan Buffer and Texture is not same
	using BufferStates = D3D12Tag::ResourceStates;

	using TextureResourcePtr = D3D12Tag::ResourcePtr;
	using TextureStates = D3D12Tag::ResourceStates;

    using BlendFactor = D3D12Tag::BlendFactor;
};

export template<>
struct RHITypeTraits<VulkanTag> {
	using PixelFormat = VulkanTag::PixelFormat;
	using TextureFlags = VulkanTag::TextrueFlags;

    using Filter = VulkanTag::Filter;
	using AddressMode = VulkanTag::AddressMode;
	using CompareFunc = VulkanTag::CompareFunc;

	using BufferResourcePtr = VulkanTag::BufferPtr;
	using BufferStates = VulkanTag::BufferStates;

	using TextureResourcePtr = VulkanTag::TexturePtr;
	using TextureStates = VulkanTag::TextureStates;

    using BlendFactor = VulkanTag::BlendFactor;
};

//NOTE : Runtime to change API no support trans is under in compler
export using g_CurrentAPI = D3D12Tag;


//NOTE :Although Vulkan not finish ,but i also Tag Same Node
// Vulkane Use VK_EXT_custom_border_color 