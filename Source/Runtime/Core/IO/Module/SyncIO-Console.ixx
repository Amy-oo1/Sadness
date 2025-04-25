export module SyncIO:Console;

import std;

export using std::cout;
export using std::cin;

export using std::operator<<;//NOTE :STL Module is specific,it kan link global operator func by ADL
export using std::endl;