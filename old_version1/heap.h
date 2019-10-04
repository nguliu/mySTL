#pragma once
#ifndef __HEAP_H__
#define __HEAP_H__

#include "iterator.h"		//迭代器型别相关
#include "vector.h"

__mySTL_BEGIN__

	//省缺以vector作为底层容器，也可指定为数组
	template<class T, class Sequence = vector<T>>
	class heap {
	public:
		typedef typename Sequence::iterator			iterator;
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::difference_type	Distance;
		typedef typename Sequence::reference		reference;
	
		Sequence C;		//底层容器

	protected:
		/* 上溯程序 */
		/* first:底层容器起始位置  topIndex:堆顶位置  holeIndex:洞号  value:需要上溯的元素值 */
		void percolate_up(iterator first, Distance topIndex, Distance holeIndex, value_type value);

		/* 下溯程序 */
		/* first:底层容器起始位置  holeIndex:洞号  len:堆的总长度  value:需要下溯的元素值 */
		void percolate_down(iterator first, Distance holeIndex, Distance len, value_type value);

		/* 从 heap 中移除，但不从底层容器中移除 */
		void pop_heap(iterator first, iterator last) {
			value_type value = *(last - 1);
			*(last - 1) = *first;			//将堆顶移动到底层容器的尾部

			percolate_down(first, Distance(0), Distance(last - first - 1), value);
			/* 以上依次传入：底层容器起始位置 洞号 堆的总长度 需要下溯的元素值 */
		}

		/* 以下利用底层容器的操作实现queue的对外接口 */
	public:
		heap() : C() { }
		heap(const Sequence& x) : C(x) { make_heap(); }
		heap(std::initializer_list<T> init_list) : C(init_list) { make_heap(); }

		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }

		/* top返回堆顶，即堆中最大/最小值 */
		reference top() { return C.front(); }

		/* 向堆中压入元素 */
		void push(const value_type& x) {
			C.push_back(x);
			percolate_up(C.begin(), Distance(0), Distance(C.size() - 1), C.back());
			/* 以上依次传入：底层容器起始位置 堆顶位置 洞号 新插入元素值 */
		}

		/* 从堆中弹出元素 */
		void pop() {
			pop_heap(C.begin(), C.end());	//先弹出堆
			C.pop_back();	//再弹出底层容器
		}

		/* 对底层容器排序 */
		void sort() {
			iterator first = C.begin();
			iterator last = C.end();
			while (last - first > 1) {
				pop_heap(first, last--);	//每次扣除一个尾端元素
			}
		}

		/* 以下使堆的底层容器满足堆的要求 */
		/* 其本质过程为：从最大的非叶节点开始，对每个非叶节点执行一次下溯程序 */
		void make_heap() {
			iterator first = C.begin();
			iterator last = C.end();

			Distance len = last - first;
			if (len < 2) return;		//如果长度为0或1，则不必重排元素

			/* 找出第一个需要重排的节点，以holeIndex标出，从第一个非叶节点开始 */
			Distance holeIndex = (len - 2) / 2;
			while (holeIndex >= 0) {
				percolate_down(first, holeIndex, len, *(first + holeIndex));
				--holeIndex;
			}
		}

		bool operator==(const heap<T, Sequence>& x) {
			return (C == x.C);
		}
		bool operator<(const heap<T, Sequence>& x) {
			return (C < x.C);
		}
	};


	/* 上溯程序 */
	/* first:底层容器起始位置  topIndex:堆顶位置  holeIndex:洞号  value:需要上溯的元素值 */
	template<class T, class Sequence>
	void heap<T, Sequence>::percolate_up(iterator first, Distance topIndex, Distance holeIndex, value_type value)
	{
		Distance parent = (holeIndex - 1) / 2;		//找到洞号的父节点

		/* 当尚未达到顶端且新插入的值大于洞号父节点 */
		while (holeIndex > topIndex && value > *(first + parent))
		{
			*(first + holeIndex) = *(first + parent);		//令洞值为父节点值
			holeIndex = parent;					//调整洞号，向上提升至父节点
			parent = (holeIndex - 1) / 2;		//找到新洞号的父节点
		}
		/* 令洞值为新值，插入操作完成 */
		*(first + holeIndex) = value;
	}


	/* 下溯程序 */
	/* first:底层容器起始位置  holeIndex:洞号  len:堆的总长度  value:需要下溯的元素值 */
	template<class T, class Sequence>
	void heap<T, Sequence>::percolate_down(iterator first, Distance holeIndex, Distance len, value_type value)
	{
		Distance maxChild = 2 * (holeIndex + 1);	//找到洞号的右子节点
		while (maxChild < len)			//走完所有左右孩子都有的节点（maxChild == len表示洞号只有左孩子没有右孩子）
		{
			/* 找到洞号较大的子节点，以 maxChild 标出 */
			if (*(first + maxChild) < *(first + maxChild - 1)) {
				--maxChild;
			}

			if (value >= *(first + maxChild))		//如果 value 不小于洞号较大子节点值，表明找到了 lalue 应插入的位置
				break;

			*(first + holeIndex) = *(first + maxChild);		//令洞值为较大子节点值
			holeIndex = maxChild;				//洞号下移至较大子节点处
			maxChild = 2 * (holeIndex + 1);		//找到新洞号的右孩子
		}
		if (maxChild == len) {			//maxChild == len表示洞号只有左孩子没有右孩子
			if (*(first + maxChild - 1) > value) {				//如果左孩子值大于插入值
				*(first + holeIndex) = *(first + maxChild - 1);	//将左孩子值填充到洞号
				holeIndex = maxChild - 1;		//将洞号移到左孩子处
			}
		}
		*(first + holeIndex) = value;		//填充洞号
	}


__mySTL_END__

#endif	//end of __HEAP_H__