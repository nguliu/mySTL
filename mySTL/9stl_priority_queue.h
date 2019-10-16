/*
 *priority_queue实现
 *
 *priority_queue有了heap相关算法的帮助实现就很简单了，
 */

#ifndef _STL_PRIORITY_QUEUE_H_
#define _STL_PRIORITY_QUEUE_H_

#include "2stl_algobase.h"
#include "4stl_vector.h"
#include "9stl_heap.h"
#include <initializer_list>		//for 列表初始化

namespace lfp {

	//默认使用less，即大根堆
	template<class T, class Container = vector<T>, class Compare = less<T>>
	class priority_queue {
	public:
		typedef typename Container::value_type			value_type;
		typedef typename Container::size_type			size_type;
		typedef typename Container::reference			reference;
		typedef typename Container::const_reference		const_reference;
	protected:
		Container cont;		//底层容器
		Compare comp;		//权值比较法则
	public:
		priority_queue() { }
		priority_queue(const vector<T>& x) : cont(x) {
			make_heap(cont.begin(), cont.end(), comp);
		}
		priority_queue(std::initializer_list<T> init_list) : cont(init_list) {
			make_heap(cont.begin(), cont.end(), comp);
		}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) : cont(first, last) {
			make_heap(cont.begin(), cont.end(), comp);
		}

		bool empty() const { return cont.empty(); }
		size_type size() const { return cont.size(); }

		const_reference top() const { return cont.front(); }

		void push(const value_type& x) {
			try {
				cont.push_back(x);
				push_heap(cont.begin(), cont.end(), comp);
			}
			catch (...) {
				cont.clear();
				throw;
			}
		}

		void pop() {
			try {
				pop_heap(cont.begin(), cont.end(), comp);
				cont.pop_back();
			}
			catch (...) {
				cont.clear();
				throw;
			}
		}
	};
	
} // end of namespace lfp

#endif	//! _STL_PRIORITY_QUEUE_H_