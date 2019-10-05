/*
 *定义了迭代器的相关属性及操作，实现了iterator_traits
 *
 *iterator_traits负责萃取迭代器的特性
 *详见 STL源码剖析 P85
 */

#ifndef _STL_ITERATOR_H_
#define _STL_ITERATOR_H_

#include <cstddef>	 //for ptrdiff_t

namespace lfp {

/* 五种迭代器标志 */
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

/* 为了避免写代码时遗漏某个内嵌型别，自行开发的迭代器最好继承下面这个基类 */
	template<class Category,
			 class T,			//必须要传入迭代器型别（以上五种之一）和对象型别
			 class Distance = ptrdiff_t,
			 class Pointer = T*,
			 class Reference = T&>
	struct iterator {
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};


/*	榨汁机traits */
	/* iterator_traits负责萃取迭代器的特性 */
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	/* 针对原生指针而设计的偏特化版traits */
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	/* 针对const原生指针而设计的偏特化版traits */
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};


	/* 这个函数可以很方便的决定某个迭代器的distance type */
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


/* 这个函数可以很方便的决定某个迭代器的类型（以上五种迭代器的一个对象） */
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}


/* 这个函数可以很方便的决定某个迭代器的value type */
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


namespace detail {		//这一级namespace下是内部实现细节

/* 以下是整组distance函数细节 */
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


/* 以下是整组advance函数细节 */
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& iter, Distance n, input_iterator_tag) {
		while (n--)
			++iter;
	}

	template<class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator& iter, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--) ++iter;
		else
			while (n++) --iter;
	}

	template<class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& iter, Distance n, random_access_iterator_tag) {
		iter += n;
	}

}// end of namespace detail


/* distance标准接口 */
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return detail::__distance(first, last, category());
	}

/* advance标准接口 */
	template<class InputIterator, class Distance>
	inline void advance(InputIterator& iter, Distance n) {
		detail::__advance(iter, n, iterator_category(iter));
	}

}// end of namespace lfp

#endif // !_STL_ITERATOR_H_