export module Concurrent :Container;

import std;

using namespace std;

export template<typename _Ty>
class Queue_ThreadSafe {
public:

	void EnQueue(_Ty&& InValue) {
		lock_guard<mutex> Lock { this->m_Mutex };

		this->m_Queue.push(move(InValue));
	}

	void EnQueue(const _Ty& InValue) {
		lock_guard<mutex> Lock { this->m_Mutex };

		this->m_Queue.push(InValue);
	}

	_Ty DeQueue(void) {
		lock_guard<mutex> Lock { this->m_Mutex };
		
		auto Re { this->m_Queue.front() };
		this->m_Queue.pop();
		return Re;
	}

	size_t Size(void)  const {
		lock_guard<mutex> Lock { this->m_Mutex };

		return this->m_Queue.size();
	}

	bool Is_Empty(void) const{
		lock_guard<mutex> Lock { this->m_Mutex };

		return this->m_Queue.empty();
	}


private:
	queue<_Ty> m_Queue {};
	mutable mutex m_Mutex {};

};