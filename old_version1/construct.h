/*对象管理头文件：
 *
 *负责内存空间对象的构造、析构、移动和复制等操作
 *
 */


#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__

#include "config.h"		//相关配置头文件
#include "iterator.h"	//类型萃取相关头文件
#include "algobase.h"	//基本算法头文件
#include <new.h>		//欲使用placement new，需先包含此头文件
#include <string.h>		//memmove() 函数


__mySTL_BEGIN__

/* 构造和解析的基本工具：construct() 和 destroy()	*/
	template<typename T1, typename T2>
	inline void construct(T1* p, const T2& value) {
		new (p) T1(value);		//placement new，调用T1::T1(value)
	}

	//以下是destroy第一版本，接受一个指针
	template<typename T>
	inline void destroy(T* p) {
		p->~T();
	}


/*	以下是destroy第二版本，接受两个迭代器，此函数设法找出元素的数值型别 */
/*	进而利用__type_traits<>求取最恰当的措施 */
	template<typename ForwardIterator, typename T>		//函数声明
	inline void __destroy(ForwardIterator, ForwardIterator, T*);		

	template<typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		__destroy(first, last, value_type(first));
	}

	//判断元素的数值型别(value type)是否有trivial destructor
	template<typename ForwardIterator, typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	//如果元素数值型别有non-trivial destructor
	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first < last; ++first)
			destroy(&*first);
	}

	//如果元素数值型别有trivial destructor
	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) { }

	//以下是destroy第二版本针对迭代器为char*和wchar_t*的特化版
	inline void destroy(char*, char*) { }
	inline void destroy(wchar_t*, wchar_t*) { }



/*	内存基本处理工具	*/

/* uninitialized_copy: 对应高层的 copy  */
	/*	first:	输入端空间起始位置		*/
	/*	last:	输入端空间结束位置		*/
	/*	result:	欲初始化空间起始位置	*/
	template<typename InputIterator, typename ForwardIterator, typename T>		//函数声明
	inline ForwardIterator __uninitialized_copy(InputIterator, InputIterator, ForwardIterator, T*);

	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
		//以上利用 value_type 取出 result 的 value type
	}

	/* 这个函数的逻辑是首先萃取出 result 的 value type，然后判断该型别是否为POD型别 */
	template<typename InputIterator, typename ForwardIterator, typename T>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;			//__type_traits<>技法实现见3.7
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效 */

	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return copy(first, last, result);			//交由高阶STL算法处理，实现见 algobase.h
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
			throw;
		}
		return cur;
	}

	/* 针对 const char* 的特化版本 */
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);		//直接移动内存内容进行拷贝
		return result + (last - first);
	}

	/* 针对 const wchar_t* 的特化版本 */
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));		//直接移动内存内容进行拷贝
		return result + (last - first);
	}


/* uninitialized_fill: 对应高层的 fill */
	/*	first: 欲初始化空间起始位置	   */
	/*	last:  欲初始化空间结束位置	   */
	/*	x:	   初始化初值			   */
	template<typename ForwardIterator, typename T, typename T1>		//函数声明
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*);

	template<typename ForwardIterator, typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		__uninitialized_fill(first, last, x, value_type(first));
		//以上利用 value_type 取出 first 的 value type
	}

	/* 这个函数的逻辑是首先萃取出 first 的 value type，然后判断该型别是否为POD型别 */
	template<typename ForwardIterator, typename T, typename T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>技法实现见3.7
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效 */

	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
		fill(first, last, x);			//交由高阶STL算法处理，实现见 algobase.h
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
			throw;
		}
	}


/* uninitialized_fill_n: 对应高层的 fill_n */
	/*	first: 欲初始化空间起始处	*/
	/*	n:	   欲初始化空间大小		*/
	/*	x:	   初始化初值			*/
	template<typename ForwardIterator, typename Size, typename T, typename T1>		//函数声明
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator, Size, const T&, T1*);

	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
		//以上利用 value_type 取出 first 的 value type
	}

	/* 这个函数的逻辑是首先萃取出 first 的 value type，然后判断该型别是否为POD型别 */
	template<typename ForwardIterator, typename Size, typename T, typename T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>技法实现见3.7
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	/* 如果copy construct等同于assignment，并且destruct是trivial，以下就有效 */

	/* 如果是POD型别，执行流程就会转到下面函数，这是藉由function template的参数推导机制而得 */
	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		return fill_n(first, n, x);		//交由高阶STL算法处理，实现见 algobase.h
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
			throw;
		}
		return cur;
	}


__mySTL_END__

#endif	//end of __CONSTRUCT_H__