module;

#include "Runtime/Core/Platform/Include/PlatformMacros.h"

export module Container :SmartPointer;

import std;

//NOTE : UniquePtr
export template<typename Type,typename Deleter=std::default_delete<Type>>
using UniquePtr = std::unique_ptr<Type, Deleter>;

export template <class _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
[[nodiscard]] FORCINLINE UniquePtr<_Ty> MakeUnique(_Types&&... _Args) {
	return std::make_unique<_Ty>(std::forward<_Types>(_Args)...);
}

//NOTE : SharedPtr
export template<typename Type>
using SharedPtr = std::shared_ptr<Type>;

export template<typename Type>
using WeakPtr = std::weak_ptr<Type>;

export template<typename Type>
using EnableSharedFormThis = std::enable_shared_from_this<Type>;


export template<std::_Bounded_builtin_array _Ty>
[[nodiscard]] FORCINLINE SharedPtr<_Ty> MakeShared(void) {
	return std::make_shared<_Ty>();
}

export template<std::_Bounded_builtin_array _Ty>
[[nodiscard]] FORCINLINE SharedPtr<_Ty> MakeShared(const std::remove_extent_t<_Ty>& _Val) {
	return std::make_shared<_Ty>(_Val);
}

export template<std::_Not_builtin_array _Ty, class... _Types>
[[nodiscard]] FORCINLINE SharedPtr<_Ty> MakeShared(_Types&&... _Args) {
	return std::make_shared<_Ty>(std::forward<_Types>(_Args)...);
}

export template <std::_Unbounded_builtin_array _Ty>
[[nodiscard]] FORCINLINE SharedPtr<_Ty> MakeShared(const size_t _Count) {
	return std::make_shared<_Ty>(_Count);
}

export template <std::_Unbounded_builtin_array _Ty>
[[nodiscard]] FORCINLINE SharedPtr<_Ty> MakeShared(const size_t _Count, const std::remove_extent_t<_Ty>& _Val) {
	return std::make_shared<_Ty>(_Count, _Val);
}



export template<std::_Not_builtin_array Type,typename Alloc,typename...Args>
[[nodiscard]] FORCINLINE SharedPtr<Type> AllocateShared(const Alloc& alloc, Args&&... args) {
	return std::allocate_shared<Type>(alloc, std::forward<Args>(args)...);
}

export template<std::_Bounded_builtin_array Type, typename Alloc>
[[nodiscard]] FORCINLINE SharedPtr<Type> AllocateShared(const Alloc& alloc) {
	return std::allocate_shared<Type>(alloc);
}

export template<std::_Bounded_builtin_array Type, typename Alloc>
[[nodiscard]] FORCINLINE SharedPtr<Type> AllocateShared(const Alloc& alloc, const std::remove_extent_t<Type>& _Val) {
	return std::allocate_shared<Type>(alloc, _Val);
}

export template<std::_Unbounded_builtin_array Type, typename Alloc>
[[nodiscard]] FORCINLINE SharedPtr<Type> AllocateShared(const Alloc& alloc, const size_t _Count) {
	return std::allocate_shared<Type>(alloc, _Count);
}

export template<std::_Unbounded_builtin_array Type, typename Alloc>
[[nodiscard]] FORCINLINE SharedPtr<Type> AllocateShared(const Alloc& alloc, const size_t _Count, const std::remove_extent_t<Type>& _Val) {
	return std::allocate_shared<Type>(alloc, _Count, _Val);
}

//NOTE : Pointer_Cast
export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> StaticPointerCast(const SharedPtr<SrcType>& ptr) {
	return std::static_pointer_cast<SrcType>(ptr);
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> StaticPointerCast(SharedPtr<SrcType>&& ptr) {
	return std::static_pointer_cast<SrcType>(std::move(ptr));
}


export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> DynamicPointetCast(const SharedPtr<SrcType>& ptr) {
	return std::dynamic_pointer_cast<SrcType>(ptr);
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> DynamicPointetCast(SharedPtr<SrcType>&& ptr) {
	return std::dynamic_pointer_cast<SrcType>(std::move(ptr));
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> ConstPointerCast(const SharedPtr<SrcType>& ptr) {
	return std::const_pointer_cast<SrcType>(ptr);
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> ConstPointerCast(SharedPtr<SrcType>&& ptr) {
	return std::const_pointer_cast<SrcType>(std::move(ptr));
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> ReinterpretPointerCast(const SharedPtr<SrcType>& ptr) {
	return std::reinterpret_pointer_cast<SrcType>(ptr);
}

export template<typename DstType, typename SrcType>
[[nodiscard]] FORCINLINE SharedPtr<DstType> ReinterpretPointerCast(SharedPtr<SrcType>&& ptr) {
	return std::reinterpret_pointer_cast<SrcType>(std::move(ptr));
}

//NOTE: SharedPtr Must Have a delete in constructor
export template<typename Deleter,typename Type>
[[nodiscard]] FORCINLINE Deleter* GetDeleter(const SharedPtr<Type>& ptr) {
	return std::get_deleter<Deleter>(ptr);
}