export module Concurrent :Mutex;

import std;

//NOTE : thread manager is less ,this for me is enougth


export using Mutex = std::mutex;

export using SharedMutex = std::shared_mutex;

export template<typename  _Mutex>
using LockGuard = std::lock_guard<_Mutex>;

export template<typename _Mutex>
using UniqueLock = std::unique_lock<_Mutex>;

export template<typename _Mutex>
using SharedLock = std::shared_lock<_Mutex>;