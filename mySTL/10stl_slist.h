/*slist实现
 *
 *list和list最大的区别在于，前者的迭代器属于单向的ForwardIterator，而后者是双向迭代器BidirectionalIterator。因此
 *slist的功能就受到很多限制，但slist消耗的空间更少。和list一样，它们的插入、移除、接合等操作不会造成原有的迭代器失效。
 *
 *注意：根据STL的习惯，插入操作会将元素插入到迭代器所指位置之前而不是之后，但slist没办法
 *快速找到其前一个节点，只能从头遍历，这便是slist最大的缺点，因此slist不提供push_back操作，
 *只提供insert_after、erase_after、push_front操作
 */

#ifndef _STL_SLIST_H_
#define _STL_SLIST_H_

#include "1stl_alloctor.h"
#include "1stl_construct.h"
#include "1stl_iterator.h"

namespace lfp {
namespace detail {

	/* 这里单链表的节点和迭代器都采用双层架构 */

	//单链表的基本节点结构
	struct __slist_node_base {
		__slist_node_base* next;
	};
	//单向链表的节点结构
	template<class T>
	struct __slist_node : __slist_node_base {
		T data;
	};

	/* 单向链表迭代器的基本结构 */
	struct __slist_iterator_base {
	public:
		__slist_node_base* node;		//当前迭代器所指的节点

		__slist_iterator_base(__slist_node_base* x) : node(x) { }

		void incr() { node = node->next; }		//前进一个节点
		bool operator==(const __slist_iterator_base& x) const {
			return node == x.node;
		}
		bool operator!=(const __slist_iterator_base& x) const {
			return node != x.node;
		}
	};
	//单向链表的迭代器结构
	template<class T, class Ref = T&, class Ptr = T*>
	struct __slist_iterator : __slist_iterator_base {
		typedef forward_iterator_tag						iterator_category;
		typedef T											value_type;
		typedef size_t										size_type;
		typedef Ptr											pointer;
		typedef const Ptr									const_pointer;
		typedef Ref											reference;
		typedef const Ref									const_reference;
		typedef ptrdiff_t									defference_type;
		typedef __slist_iterator<T, T&, T*>					iterator;
		typedef __slist_iterator<T, const T&, const T*>		const_iterator;

		typedef __slist_node<T>								slist_node;
		typedef __slist_iterator<T, Ref, Ptr>				self;

		__slist_iterator() : __slist_iterator_base(0) { }
		__slist_iterator(slist_node* x) : __slist_iterator_base(x) { }
		__slist_iterator(const iterator& x) : __slist_iterator_base(x.node) { }

		reference operator*() const {
			return ((slist_node*)node)->data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		self& operator++() {	//前置++
			incr();
			return *this;
		}
		self operator++(int) {	//后置++
			self tmp = *this;
			incr();
			return tmp;
		}
	};

	//全局函数，已知某一节点，插入一新节点于其后
	inline __slist_node_base* __slist_make_link(
		__slist_node_base* prev_node,
		__slist_node_base* next_node)
	{
		next_node->next = prev_node->next;
		prev_node->next = next_node;
		return next_node;
	}

	//全局函数，计算单链表的元素个数
	inline size_t __slist_size(__slist_node_base* node) {
		size_t result = 0;
		for (; node != 0; node = node->next) {
			++result;
		}
		return result;
	}

} // end of namespace lfp



/* slist结构的实现 */
	template<class T, class Alloc = alloc>
	struct slist {
	public:
		typedef typename detail::__slist_iterator<T, T&, T*>::value_type			value_type;
		typedef typename detail::__slist_iterator<T, T&, T*>::size_type			size_type;
		typedef typename detail::__slist_iterator<T, T&, T*>::iterator			iterator;
		typedef typename detail::__slist_iterator<T, T&, T*>::const_iterator		const_iterator;
		typedef typename detail::__slist_iterator<T, T&, T*>::pointer			pointer;
		typedef typename detail::__slist_iterator<T, T&, T*>::const_pointer		const_pointer;
		typedef typename detail::__slist_iterator<T, T&, T*>::reference			reference;
		typedef typename detail::__slist_iterator<T, T&, T*>::const_reference	const_reference;
		typedef typename detail::__slist_iterator<T, T&, T*>::defference_type	defference_type;

	private:
		typedef detail::__slist_node<T>			slist_node;
		typedef detail::__slist_node_base		slist_node_base;
		typedef detail::__slist_iterator_base	slist_iterator_base;

		slist_node_base dummyHead;	//头部。注意这是实体，而不是指针（dummyHead机制的作用请自己百度）

		//以节点大小为单位的专属空间配置器
		typedef simple_alloc<slist_node, Alloc> slist_node_allocater;

		static slist_node* create_node(const value_type& x) {
			slist_node* node = slist_node_allocater::allocate();		//配置一个节点的空间
			try {
				construct(&node->data, x);
				node->next = 0;
			}
			catch (...) {
				slist_node_allocater::deallocate(node);
				throw;
			}
			return node;
		}
		static void destroy_node(slist_node* node) {
			destroy(&node->data);		//析构元素
			slist_node_allocater::deallocate(node);		//释放空间
		}
		void clear() {
			slist_node* node = (slist_node*)dummyHead.next;
			while (node != nullptr) {
				slist_node* tmp = node;
				node = (slist_node*)node->next;
				destroy_node(tmp);
			}
		}
		
	public:
		slist()  { dummyHead.next = 0; }
		~slist() { clear(); }

		iterator begin() { return iterator((slist_node*)dummyHead.next); }
		const_iterator begin() const { return iterator((slist_node*)dummyHead.next); }

		iterator end() { return iterator(0); }
		const_iterator end() const { return iterator(0); }

		reference front() { return ((slist_node*)dummyHead.next)->data; }
		const_reference front() const { return ((slist_node*)dummyHead.next)->data; }

		size_type size() const { return detail::__slist_size(dummyHead.next); }

		void push_front(const value_type& x) {
			detail::__slist_make_link(&dummyHead, create_node(x));
		}
		void pop_front() {
			slist_node* node = (slist_node*)dummyHead.next;
			dummyHead.next = node->next;
			destroy_node(node);
		}

		iterator insert_after(iterator pos, const value_type& x) {
			detail::__slist_make_link(pos.node, create_node(x));
			return iterator((slist_node*)pos.node->next);
		}

		iterator erase_after(iterator pos) {
			slist_node* node = (slist_node*)pos.node->next;
			pos.node->next = node->next;
			destroy_node(node);

			return iterator((slist_node*)pos.node->next);
		}

		//两个 slist 互换，只要将dummyHead.next交换互指即可
		void swap(slist& rhs) {
			slist_node_base* tmp = dummyHead.next;
			dummyHead.next = rhs.dummyHead.next;
			rhs.dummyHead.next = tmp;
		}
	};


} // end of namespace lfp

#endif	//! _STL_SLIST_H_