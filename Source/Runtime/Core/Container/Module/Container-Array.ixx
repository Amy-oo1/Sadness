export module Container:Array;

import std;

import Platform;

//NOTEL :Size==0 ,std has a specialization
export template<typename Type, Size _Size>
using Array = std::array<Type, _Size>;