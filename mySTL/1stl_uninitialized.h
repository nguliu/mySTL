/*
 *这里定义了一些全局函数，用来填充、复制大块内存数据。
 *
 *主要定义了uninitialized_copy、uninitialized_fill和uninitialized_fill_n，
 *
 *它们都具有“commit or rollback”语意，意思是要么成功构造出所有必要元素，
 *要么不构造任何东西（当有任何一个constructor失败时，需要rollback）
 */

#ifndef _STL_UNINITIALIZED_H_
#define _STL_UNINITIALIZED_H_

#include "1stl_iterator.h"		//for function value_type
#include "1stl_type_traits.h"	//for __type_traits
#include "2stl_algobase.h"		//for algorithm
#include <string.h>

namespace lfp {
namespace detail {

/* uninitialized_copy辅助函数 */
	/* 这个函数的逻辑是首先萃取出 result 的 value type，然后判断该型别是否为POD型别 */
	template<typename InputIterator, typename ForwardIterator, typename T>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;			//__type_traits<>技法实现见3.7
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效。 */
	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return copy(first, last, result);	//交由高阶STL算法处理，实现见 2algorithm.h
	}

	/* 如果不是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
		ForwardIterator cur = result;
		try {
			for (; first != last; ++first, ++cur) {
				construct(&*cur, *first);		//必须一个一个的构造，无法批量处理、
			}
		}
		catch (...) {	//发生异常，[result,cur)内的元素已完成构造，必须将其全部消灭
			ForwardIterator p = result;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;	 //向高层抛出一个异常
		}
		return cur;
	}


/* uninitialized_fill辅助函数 */
	/* 这个函数的逻辑是首先萃取出 first 的 value type，然后判断该型别是否为POD型别 */
	template<typename ForwardIterator, typename T, typename T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>技法实现见3.7
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效。 */
	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
		fill(first, last, x);		//交由高阶STL算法处理，实现见 2algorithm.h
	}

	/* 如果不是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
		ForwardIterator cur = first;
		try {
			for (; cur != last; ++cur) {
				construct(&*cur, x);		//必须一个一个的构造，无法批量处理
			}
		}
		catch (...) {	//发生异常，[first,cur)内的元素已完成构造，必须将其全部消灭
			ForwardIterator p = first;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;	 //向高层抛出一个异常
		}
	}


/* uninitialized_fill_n辅助函数 */
	/* 这个函数的逻辑是首先萃取出 first 的 value type，然后判断该型别是否为POD型别 */
	template<typename ForwardIterator, typename Size, typename T, typename T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>技法实现见3.7
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效。 */
	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		return fill_n(first, n, x);		//交由高阶STL算法处理，实现见 2algorithm.h
	}

	/* 如果不是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		ForwardIterator cur = first;
		try {
			for (; n > 0; --n, ++cur) {
				construct(&*cur, x);		//必须一个一个的构造，无法批量处理
			}
		}
		catch (...) {	//发生异常，[first,cur)内的元素已完成构造，必须将其全部消灭
			ForwardIterator p = first;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;	 //向高层抛出一个异常
		}
		return cur;
	}

}// end of namespace detail


	/* uninitialized_copy: 对应高层的 copy  */
	/*	first:	输入端空间起始位置		*/
	/*	last:	输入端空间结束位置		*/
	/*	result:	欲初始化空间起始位置	*/
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return detail::__uninitialized_copy(first, last, result, value_type(result));
		//以上利用value_type取出result的value type
	}
	/* 针对 const char* 的特化版本，采用高效的memmove */
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}
	/* 针对 const wchar_t* 的特化版本，采用高效的memmove */
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}


	/* uninitialized_fill: 对应高层的 fill */
	/*	first: 欲初始化空间起始位置	   */
	/*	last:  欲初始化空间结束位置	   */
	/*	x:	   初始化初值			   */
	template<typename ForwardIterator, typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		detail::__uninitialized_fill(first, last, x, value_type(first));
		//以上利用 value_type 取出 first 的 value type
	}


	/* uninitialized_fill_n: 对应高层的 fill_n */
	/* first: 欲初始化空间起始处	*/
	/* n:	  欲初始化空间大小		*/
	/* x:	  初始化初值			*/
	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return detail::__uninitialized_fill_n(first, n, x, value_type(first));
		//以上利用 value_type 取出 first 的 value type
	}

}// end of namespace lfp

#endif // !_STL_CONSTRUCT_H_