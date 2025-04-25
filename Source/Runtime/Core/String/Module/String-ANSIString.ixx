export module String:ANSIString;

import std;

export using ANSIString = std::string;
export using MANSIString = std::pmr::string;
export using ANSIStringView = std::string_view;