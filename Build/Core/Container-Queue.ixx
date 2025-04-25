export module Container:Queue;

import Utility;
import Concurrent;
import Memory;
import :Optional;

import std;

//TODO Queue ? Meybe you just nned a vector

constexpr int Alignment{ 64 };//TODO :

//TODO :Remove ti form here
export template<typename Type>
class Queue_SPMC final :public MoveAbleOnly{
private:
	struct Node {
		explicit Node(Type&& data) :
			Data { MoveTemp(data) } 
		{}

		explicit Node(const Type& data) :
			Data { data }
		{}

		Type Data;
		Atomic<Node*> Next{ nullptr };
	};

public:
	//NOTE : Alloctor has a cont use defualtresource
	//TODO : Uset Must inpout a ThreadSaft alloctor
	Queue_SPMC() :
		MoveAbleOnly{},
		m_PoolResource { NewDeleteResource()},
		m_Alloctor{ &this->m_PoolResource },
		m_Head{this->m_Alloctor.new_object<Node>(Type{})},
		m_Tail{this->m_Head.load(MemoryOrderRelaxd)} 
	{}

	~Queue_SPMC(void) {
		auto Current{ this->m_Head.load(MemoryOrderRelaxd) };
		while (nullptr != Current) {
			auto Next{ Current->Next.load(MemoryOrderRelaxd) };

			this->m_Alloctor.delete_object(Current);
			Current = Next;
		}

	}

	void EnQueue(Type&& data) {
		auto NewNode{ this->m_Alloctor.new_object<Node>(MoveTemp(data)) };
		
		auto PervTail{ this->m_Tail.exchange(NewNode,MemoryOrderAcqRel) };
		PervTail->Next.store(NewNode, MemoryOrderRelease);
	}

	void EnQueue(const Type& data) {
		auto NewNode { this->m_Alloctor.new_object<Node>(data) };

		auto PervTail { this->m_Tail.exchange(NewNode, MemoryOrderAcqRel) };
		PervTail->Next.store(NewNode, MemoryOrderRelease);
	}

	[[nodiscard]] Optional<Type> DeQueue(void) {
		Node* Current{ nullptr };
		Node* Next{ nullptr };

		for (;;) {
			Current = m_Head.load(MemoryOrderAcquire);

			if (nullptr==Current)
				return NullOpt;

			if (this->m_Head.compare_exchange_weak(
				Current, Next,
				MemoryOrderAcqRel,MemoryOrderRelaxd))
				break;
		}

		auto Resource{ MoveTemp(Next->Data) };
		this->m_Alloctor.delete_object(Current);

		return Resource;
	}

	[[nodiscard]] const Optional<Type> Peek(void) {
		auto Current{ m_Head.load(MemoryOrderRelaxd) };
		auto Next{ Current->Next.load(MemoryOrderRelaxd) };

		if (nullptr == Next)
			return NullOpt;

		return Next->Data;
	}

	[[nodiscard]] bool Is_Empty(void) {
		return
			this->m_Head.load(MemoryOrderRelaxd) ==
			this->m_Tail.load(MemoryOrderRelaxd);
	}

private:
	SynchronizedPoolResource m_PoolResource { nullptr };
	PolymorphicAllocator<Node> m_Alloctor;

	alignas(Alignment) volatile Atomic<Node*> m_Head;
	alignas(Alignment) Atomic<Node*> m_Tail;
};


export template<typename _Ty>
using Queue = std::queue<_Ty, std::deque<_Ty>>;//Dont Fix