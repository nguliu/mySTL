#pragma once
#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include "heap.h"		//以 heap 为底部容器
#include "algobase.h"		//基本算法相关

__mySTL_BEGIN__

	template<class T, class Sequence = heap<T>>
	class priority_queue {
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
	protected:
		Sequence C;			//底层容器
	public:
		priority_queue() : C() { }
		priority_queue(const vector<T>& x) : C(x) { }
		priority_queue(std::initializer_list<T> init_list) : C(init_list) { }

		bool empty() { return C.empty(); }
		size_type size() const { return C.size(); }
		reference front() { return C.top(); }
		void push(const value_type& x) {
			try {
				C.push(x);
			}
			catch (...) {
				C.C.clear();
				throw;
			}
		}
		void pop() {
			try {
				C.pop();
			}
			catch (...) {
				C.C.clear();
				throw;
			}
		}
	};
	

__mySTL_END__

#endif	//end of __PRIORITY_QUEUE_H__