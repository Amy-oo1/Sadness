module;
#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Memory :Alloctor;

import std;

//NOTE : Do NOT Use shadcred or uniqe ptr to conl memory,because these ptr use it

export using MemopyResource = std::pmr::memory_resource;

export [[nodiscard]] FORCINLINE MemopyResource* NewDeleteResource(void) {
	return std::pmr::new_delete_resource();
}

export [[nodiscard]] FORCINLINE MemopyResource* NullMemoryResource(void) {
	return std::pmr::null_memory_resource();
}

export [[nodiscard]] FORCINLINE MemopyResource* Get_DefaultResource(void) {
	return std::pmr::get_default_resource();
}

//NOTE : DO NOT Set Becase has memory class use new/delete to do sync in ThreeadSfte 
export FORCINLINE void Set_DefaultResource(MemopyResource* resource) {
	std::pmr::set_default_resource(resource);
}

export using PoolOptions = std::pmr::pool_options;

export using SynchronizedPoolResource = std::pmr::synchronized_pool_resource;

export using UnsynchronizedPoolResource=std::pmr::unsynchronized_pool_resource;

export using MonotonicBufferResource=std::pmr::monotonic_buffer_resource;

export template<typename Type=std::byte>
using PolymorphicAllocator = std::pmr::polymorphic_allocator<Type>;

export using std::pmr::operator==;