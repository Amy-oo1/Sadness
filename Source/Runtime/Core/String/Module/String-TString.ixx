export module String:TString;

import std;

import Platform;

export using TString = std::basic_string<TChar>;
export using MTString = std::pmr::basic_string<TChar>;
export using TStringView = std::basic_string_view<TChar>;
