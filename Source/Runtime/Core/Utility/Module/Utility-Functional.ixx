export module Utility :Functional;

import std;

export template<typename Type>
using Function = std::function<Type>;

export template<typename Type>
using Is_Function = std::is_function<Type>;
