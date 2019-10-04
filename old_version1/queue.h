#pragma once
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "deque.h"		//以 deque 作为其底层容器
#include "list.h"		//也可以 list 作为其底层容器


__mySTL_BEGIN__

	//省缺以deque作为底层容器，也可指定为 list
	template<class T, class Sequence = deque<T>>
	class queue {
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;
	protected:
		Sequence c;		//底层容器

		/* 以下利用底层容器的操作实现queue的对外接口 */
	public:
		queue() : c() {}
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_front(); }

		bool operator==(const queue<T, Sequence>& x) {
			return (c == x.c);
		}
		bool operator<(const queue<T, Sequence>& x) {
			return (c < x.c);
		}
	};


__mySTL_END__

#endif	//end of __QUEUE_H__