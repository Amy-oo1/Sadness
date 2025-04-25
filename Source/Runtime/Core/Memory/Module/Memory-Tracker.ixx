module;
#include "Runtime/Core/String/Include/StringMacros.h"

export module Memory:Tracker;

import :Alloctor;

import Platform;
import Utility;
import String;
import SyncIO;

export class Tracker final :public MoveAbleOnly, public MemopyResource {
public:
	explicit Tracker(MemopyResource* us = Get_DefaultResource()) :
		MoveAbleOnly{},
		MemopyResource{},
		m_UpStream(us){}

	explicit Tracker(const u8String& prefix, MemopyResource* us = Get_DefaultResource()) :
		MoveAbleOnly{},
		MemopyResource{},
		m_UpStream(us),
		m_Prefix(prefix) {
	}

private:
	MemopyResource* m_UpStream;
	u8String m_Prefix{};

private:
	void* do_allocate(Size bytes,Size alignment) override {
		cout <<reinterpret_cast<const char*>(this->m_Prefix.c_str()) << ("Allocating ") << bytes << " bytes of memory with alignment " << alignment << endl;
	
		return m_UpStream->allocate(bytes, alignment);
	}

	void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
		cout << reinterpret_cast<const char*>(this->m_Prefix.c_str()) << ("Deallocating ") << ptr << endl;

		m_UpStream->deallocate(ptr, bytes, alignment);
	}

	bool do_is_equal(const MemopyResource& other) const noexcept override {
		if (this == &other)
			return true;
		
		auto op{ dynamic_cast<const Tracker*>(&other) };
		return op != nullptr && op->m_Prefix == this->m_Prefix && this->m_UpStream->is_equal(other);
	}

};