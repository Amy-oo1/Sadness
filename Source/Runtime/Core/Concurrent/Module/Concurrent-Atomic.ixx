export module Concurrent :Atomic;

import std;

export template<typename Type>
using Atomic = std::atomic<Type>;

export template<typename Type>
using AtomicRef = std::atomic_ref<Type>;

export using MemoryOrder = std::memory_order;

export constexpr auto MemoryOrderRelaxd = MemoryOrder::relaxed;
export constexpr auto MemoryOrderConsume = MemoryOrder::consume;//NOTE :Not Ues
export constexpr auto MemoryOrderAcquire = MemoryOrder::acquire;
export constexpr auto MemoryOrderRelease = MemoryOrder::release;
export constexpr auto MemoryOrderAcqRel = MemoryOrder::acq_rel;
export constexpr auto MemoryOrderSeqCst = MemoryOrder::seq_cst;//NOTE :Not Ues