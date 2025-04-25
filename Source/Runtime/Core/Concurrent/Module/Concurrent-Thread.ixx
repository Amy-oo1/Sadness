export module Concurrent :Thread;

import std;

export using Thread = std::jthread;


export namespace ThisThrad {
	export using  std::this_thread::yield;
	export using  std::this_thread::sleep_for;
	export using  std::this_thread::sleep_until;
	export using  std::this_thread::get_id;
}