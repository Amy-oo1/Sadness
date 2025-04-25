export module Container:Vector;

import std;

export template<typename Type>
using MVector = std::vector<Type, std::pmr::polymorphic_allocator<Type>>;

export template<typename Type,typename Alloc=std::allocator<Type>>
using Vector = std::vector<Type, Alloc>;

export using BitVector = std::vector<bool>;