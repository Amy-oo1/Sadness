export module Container :Hash;


import std;

import Platform;
import String;

//TODO : base type will use default hash
//my class will define when i need 
//i just use sign mothed, so here is empty

export using HashBool = std::hash<bool>;
export using HashByte = std::hash<Byte>;
export using HashUint8 = std::hash<Uint8>;
export using HashUint16 = std::hash<Uint16>;
export using HashUint32 = std::hash<Uint32>;
export using HashUint64 = std::hash<Uint64>;

export using HashInt8 = std::hash<Int8>;
export using HashInt16 = std::hash<Int16>;
export using HashInt32 = std::hash<Int32>;
export using HashInt64 = std::hash<Int64>;

export using HashChar = std::hash<char>;
export using HashChar8 = std::hash<char8_t>;
export using HashWChar = std::hash<wchar_t>;

export using HashFloat = std::hash<float>;
export using HashDouble = std::hash<double>;

export using HashVoidPtr = std::hash<void*>;
export using HashNullPtr = std::hash<std::nullptr_t>;

export using HashAnsiString = std::hash<ANSIString>;
export using HashU8String = std::hash<u8String>;
export using HashU16String = std::hash<u16String>;
export using HashU32String = std::hash<u32String>;
export using HashWString = std::hash<WString>;

export using HashAnsiStringView = std::hash<ANSIStringView>;
export using HashU8StringView = std::hash<u8StringView>;
export using HashU16StringView = std::hash<u16StringView>;
export using HashU32StringView = std::hash<u32StringView>;
export using HashWStringView = std::hash<WStringView>;

//TODO Set More

export constexpr Uint64 HashCombine(Uint64 seed, Uint64 value) { //TODO good ?
    return seed ^ (value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}