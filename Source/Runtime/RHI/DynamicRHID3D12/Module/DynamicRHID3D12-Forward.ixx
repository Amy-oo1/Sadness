module;
#include "d3d12.h"

export module DynamicRHID3D12 :Forward;

export class DeviceChild;

class GPUObject;
export class SingleNodeGPUObject;
export class MultiNodeGPUObject;

export class Device;

export template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
class DescriptorAllocator;

export template<D3D12_DESCRIPTOR_HEAP_TYPE _Type>
class DescriptorHandle;

export template<typename Derived>
class DescriptorHeapBase;
export class GlobalSamplerHeap;
export class GlobalViewHeap;
export class LocalSamplerHeap;
export class LocalViewHeap;

export class DescriptorCache;

export class GPUNode;

export class SamplerManager;


export class CommandListManager;






export class FenceCore;
export class FenceCorePool;
export template<typename Derived>
class FenceBase;
export class FenceIncrement;
export class FenceManual;
export class FenceCommandList;
export template<typename FenceType>
class SyncPoint;

export class Resource;

export class Heap;

export class DynamicRHID3D12;
