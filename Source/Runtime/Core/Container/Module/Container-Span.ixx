export module Container :Span;

import std;

export template <class _Ty, size_t _Extent = std::dynamic_extent>
using Span = std::span<_Ty>;