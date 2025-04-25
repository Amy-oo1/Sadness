export module Container :Map;

import std;

import :Hash;//Just Test

export template <class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>,class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using UnorderedMap = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;

export template <class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using UnorderedMultimap = std::unordered_multimap<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;

export template <class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using UnorderedSet = std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>;

export template <class _Kty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = std::allocator<_Kty>>
using UnorderedMultiset = std::unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>;

export template <class _Kty, class _Ty, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using Map = std::map<_Kty, _Ty, std::less<_Kty>, _Alloc>;

export template <class _Kty, class _Ty, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using Multimap = std::multimap<_Kty, _Ty, std::less<_Kty>, _Alloc>;

export template <class _Kty, class _Ty, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using Multiset = std::multiset<_Kty, std::less<_Kty>, _Alloc>;

export template <class _Kty, class _Ty, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
using Set = std::set<_Kty, std::less<_Kty>, _Alloc>;
