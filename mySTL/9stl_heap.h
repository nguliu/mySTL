/*
 *heap算法实现
 *
 *heap不属于标准库的内容，它主要提供了push_heap、pop_head、sort_heap、make_heap几个算法，
 *heap算法被运用于priority_queue中，用于实现优先级队列
 *
 *以下只提供泛型算法，用户可以自己封装为类
 */

#ifndef _STL_HEAP_H_
#define _STL_HEAP_H_

#include "1stl_iterator.h"	 //for distance_type、value_type......

namespace lfp {
namespace detail {

/* 上溯程序 */
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	void percolate_up(RandomAccessIterator first,		// 底部容器的起始位置
					  Distance topIndex,				// 堆顶位置（即堆顶与first的距离）
					  Distance holeIndex,				// 洞号（即需要上溯的位置与first的距离）
					  T value,							// 需要上溯的元素值
					  Compare comp)						// 元素权值比较规则
	{
//		Distance holeParent = (holeIndex - 1) / 2;		//找到洞号的父节点
//
//		/* 当尚未达到顶端且value权值更高 */
//		while (holeIndex > topIndex && comp(*(first + holeParent), value))
//		{
//			*(first + holeIndex) = *(first + holeParent);	//令洞值为父节点值
//			holeIndex = holeParent;							//调整洞号，向上提升至父节点
//			holeParent = (holeIndex - 1) / 2;				//找到新洞号的父节点
//		}
//		/* 填充洞号，上溯操作完成 */
//		*(first + holeIndex) = value;
//	}
//
///* 下溯程序 */
//	template<class RandomAccessIterator, class Distance, class T, class Compare>
//	void percolate_down(RandomAccessIterator first,		// 底部容器的起始位置
//						Distance holeIndex,				// 洞号（即需要下溯的位置与first的距离）
//						Distance len,					// 堆的长度
//						T value,						// 需要下溯的元素值
//						Compare comp)					// 元素权值比较规则
//	{
		Distance largerChild = 2 * (holeIndex + 1);		//找到洞号的右子节点
		while (largerChild < len)		//走完所有左右孩子都有的节点
		{
			/* 找到权值更高的子节点，以 largerChild 标出 */
			if (comp(*(first + largerChild), *(first + largerChild - 1))) {
				--largerChild;
			}
			
			/* 如果洞号较大的子节点权值不比value高，则下溯结束（当前的holeIndex即为value应插入的位置） */
			if (!comp(value, *(first + largerChild)))
				break;

			*(first + holeIndex) = *(first + largerChild);		//令洞值为较大子节点值
			holeIndex = largerChild;				//洞号下移至较大子节点处
			largerChild = 2 * (holeIndex + 1);		//找到新洞号的右孩子
		}
		if (largerChild == len) {		//largerChild == len说明洞号只有左孩子没有右孩子（例如序列1 2 3 4, holeIndex=1时的情况）
			if (comp(value, *(first + largerChild - 1))) {			//如果洞号左子节点权值比value高
				*(first + holeIndex) = *(first + largerChild - 1);	//将左孩子值填充到洞号
				holeIndex = largerChild - 1;		//将洞号移到左孩子处
			}
		}
		/* 填充洞号，下溯操作完成 */
		*(first + holeIndex) = value;
	}


/* push_heap辅助函数 */
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __push_heap_aux(RandomAccessIterator first,
								RandomAccessIterator last,
								Distance*, T*,
								Compare comp)
	{
		percolate_up(first, Distance(0), Distance(last - first - 1), *(last - 1), comp);
		//以上分别传入：底部容器起始位置、堆顶位置、洞号、需要上溯的元素值、元素权值比较规则
	}


/* pop_heap辅助函数 */
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __pop_heap_aux(RandomAccessIterator first,
							   RandomAccessIterator last,
							   Distance*, T*,
							   Compare comp)
	{
		T value = *(last - 1);
		*(last - 1) = *first;			//将堆顶移动到底层容器的尾部

		percolate_down(first, Distance(0), Distance(last - first - 1), value, comp);
		//以上分别传入：底部容器起始位置、洞号、堆的长度、需要下溯的元素值、元素权值比较规则
	}


/* make_heap辅助函数 */
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __make_heap_aux(RandomAccessIterator first,	// 底部容器的起始位置
								RandomAccessIterator last,	// 底部容器最后一个元素的下一位置
								Distance*, T*,
								Compare comp)					// 元素权值比较规则
	{
		Distance len = last - first;
		if (len < 2) return;		//如果长度为0或1，则不必重排元素

		/* 找出第一个需要重排的节点，以holeIndex标出，从第一个非叶节点开始 */
		Distance holeIndex = (len - 2) / 2;
		while (holeIndex >= 0) {
			percolate_down(first, holeIndex, len, T(*(first + holeIndex)), comp);
			--holeIndex;
		}
	}

} // end of namespace detail




/*  以下是标准算法接口  */


/* push_heap */
	//此函数被调用时，新元素已经插入底部容器尾部 last-1 位置，且 [first, last-1) 满足heap要求，否则结果不可预期
	template<class RandomAccessIterator, class Compare>
	inline void push_heap(RandomAccessIterator first,	// 底部容器的起始位置
						  RandomAccessIterator last,	// 底部容器最后一个元素的下一位置
						  Compare comp)					// 元素权值比较规则
	{
		detail::__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}


/* pop_heap */
	//调用此函数时，[first, last) 内的序列应已满足堆要求，否则结果未可预期
	//它只从heap中移除，但不从底层容器中移除
	template<class RandomAccessIterator, class Compare>
	inline void pop_heap(RandomAccessIterator first,	// 底部容器的起始位置
						 RandomAccessIterator last,		// 底部容器最后一个元素的下一位置
						 Compare comp)					// 元素权值比较规则
	{
		detail::__pop_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}


/* make_heap 使堆的底层容器满足堆的要求 */
	//其本质过程为：从最大的非叶节点开始，对每个非叶节点执行一次下溯程序
	template<class RandomAccessIterator, class Compare>
	inline void make_heap(RandomAccessIterator first,	// 底部容器的起始位置
						  RandomAccessIterator last,	// 底部容器最后一个元素的下一位置
						  Compare comp)					// 元素权值比较规则
	{
		detail::__make_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}


/* sort_heap 对底层容器排序 */
	//调用此函数时，[first, last) 内的序列应已满足堆要求，否则结果未可预期
	template<class RandomAccessIterator, class Compare>
	inline void sort_heap(RandomAccessIterator first,	// 底部容器的起始位置
						  RandomAccessIterator last,	// 底部容器最后一个元素的下一位置
						  Compare comp)					// 元素权值比较规则
	{
		while (last - first > 1) {			//每次重堆中弹出一个元素到底部容器尾端
			pop_heap(first, last, comp);
			--last;
		}
	}


} // end of namespace lfp

#endif	//! _STL_HEAP_H_