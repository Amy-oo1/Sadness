module;
//TODO : Put Platform Type
export module Platform : Type;

import std;

//NOTE : Base Types
export using Byte = std::byte;
export using Int8 = std::int8_t;
export using Uint8 = std::uint8_t;
export using Int16 = std::int16_t;
export using Uint16 = std::uint16_t;
export using Int32 = std::int32_t;
export using Uint32 = std::uint32_t;
export using Int64 = std::int64_t;
export using Uint64 = std::uint64_t;


export constexpr auto Min_Byte { std::numeric_limits<Byte>::min() };
export constexpr auto Min_Int8 { std::numeric_limits<Int8>::min() };
export constexpr auto Min_Uint8 { std::numeric_limits<Uint8>::min() };
export constexpr auto Min_Int16 { std::numeric_limits<Int16>::min() };
export constexpr auto Min_Uint16 { std::numeric_limits<Uint16>::min() };
export constexpr auto Min_Int32 { std::numeric_limits<Int32>::min() };
export constexpr auto Min_Uint32 { std::numeric_limits<Uint32>::min() };
export constexpr auto Min_Int64 { std::numeric_limits<Int64>::min() };
export constexpr auto Min_Uint64 { std::numeric_limits<Uint64>::min() };

export constexpr auto Max_Byte { std::numeric_limits<Byte>::max() };
export constexpr auto Max_Int8 { std::numeric_limits<Int8>::max() };
export constexpr auto Max_Uint8 { std::numeric_limits<Uint8>::max() };
export constexpr auto Max_Int16 { std::numeric_limits<Int16>::max() };
export constexpr auto Max_Uint16 { std::numeric_limits<Uint16>::max() };
export constexpr auto Max_Int32 { std::numeric_limits<Int32>::max() };
export constexpr auto Max_Uint32 { std::numeric_limits<Uint32>::max() };
export constexpr auto Max_Int64 { std::numeric_limits<Int64>::max() };
export constexpr auto Max_Uint64 { std::numeric_limits<Uint64>::max() };

export constexpr auto Min_Float { std::numeric_limits<float>::min() };
export constexpr auto Max_Float { std::numeric_limits<float>::max() };
export constexpr auto Min_Double { std::numeric_limits<double>::min() };
export constexpr auto Max_Double { std::numeric_limits<double>::max() };

export using Size = std::size_t;//NOTE : DO NOT USE IT !!!
export using TChar = wchar_t;
//TODO Because i use Windws so I write wchar_t in here

export using Intptr = std::intptr_t;
export using Uintptr = std::uintptr_t;
export using Ptrdiff = std::ptrdiff_t;