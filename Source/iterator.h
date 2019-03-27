#pragma once
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "config.h"		//相关配置头文件
#include <cstddef>		//使用宏 ptrdiff_t


__mySTL_BEGIN__

/*	五种迭代器类型	*/
	/* 只写迭代器 */
	struct input_iterator_tag { };
	/* 只读迭代器 */
	struct output_iterator_tag { };
	/* 读写迭代器 */
	struct forward_iterator_tag : public input_iterator_tag { };
	/* 双向迭代器 */
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	/* 随机迭代器 */
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };


/*	为了避免写代码时挂一漏万，自行开发的代码最好继承自下面这个基类	*/
	template<class Category, class T, class Distance = ptrdiff_t,
			 class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};


/*	榨汁机 traits	*/
/*	iterator_traits 负责萃取迭代器的特性	*/
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};


/*	针对原生指针而设计的偏特化版 traits	*/
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};


/*	针对 const 原生指针而设计的偏特化版 traits	*/
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};


/*	这个函数可以很方便的决定某个迭代器的类型 (category)	*/	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}


/*	这个函数可以很方便的决定某个迭代器的 distance type	*/
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


/*	这个函数可以很方便的决定某个迭代器的 value type	*/
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


/*	以下是整组 distance 函数	*/
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}


/*	以下是整组 advance 函数	*/
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--)
			++i;
	}

	template<class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n) {
		__advance(i, n, iterator_category(i));
	}


/*	SGI STL 的型别特征推导部分	*/

/* 定义两个代表真假性质的对象，使编译器对响应结果进行参数推导 */
	struct __true_type	{	};
	struct __false_type {	};

/*	实现于 SGI STL 的 type_traits.h 文件	*/
/*	__type_traits 负责萃取型别的特性	*/

	template<class type>
	struct __type_traits {
		typedef __true_type this_dummy_member_must_be_first;
			/* 不要移除这个成员，他通知“有能力将 __type_traits 特化”的编译器说：我们现在所看到的这个		*/
			/* __type_traits template 是特殊的。这是为了确保万一编译器也使用一个名为 __type_traits 而其		*/
			/* 实与此处定义并无任何关联的 template，所有事情都仍将顺利运作。								*/

		/* 以下条件应该被遵守，因为编译器有可能自动为各型别产生专属的 __type_traits 特化版本： */
			/* 你可以重新排列以下成员的次序										*/
			/* 你可以移除一下任何成员											*/
			/* 绝对不可以将以下成员重新命名却没有改变编译器中的对应名称			*/
			/* 新加入的成员会被视为一般成员，除非你在编译器中加上适当的支持		*/

		typedef __false_type	has_trivial_default_constructor;
		typedef __false_type	has_trivial_copy_constructor;
		typedef __false_type	has_trivial_assignment_operator;
		typedef __false_type	has_trivial_destructor;
		typedef __false_type	is_POD_type;
	};


/* 以下是 type_traits.h 中对所有c++标量型别所定义的 __type_traits 特化版本，这些定义对于内建有	__type_traits */
/* 支持能力的编译器并无伤害，对于无该等支持能力的编译器则属于必要。		以下为C++基本型别提供特化版本，每一个 */
/* 成员值都是 __true_type，表示这些型别都可以采用最快速方式(例如memcpy)进行拷贝、赋值等操作。				  */

	__STL_TEMPLATE_NULL struct __type_traits<char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<signed char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<float> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	//以下是针对原生指针设计的偏特化版本，原生指针也被视为一种标量型别
	template<class T>
	struct __type_traits<T*> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};


__mySTL_END__

#endif	//end of __ITERATOR_H__