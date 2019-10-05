#pragma once
#ifndef __SLIST_H__
#define __SLIST_H__

#include "alloctor.h"		//空间配置与管理
#include "construct.h"		//内存对象的构造、析构与重构等
#include "algobase.h"		//基本算法相关

__mySTL_BEGIN__

	//单链表的基本节点结构
	struct __slist_node_base {
		__slist_node_base* next;
	};
	//单向链表的节点结构
	template<class T>
	struct __slist_node : public __slist_node_base {
		T data;
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


	/* 单向链表迭代器的基本结构 */
	struct __slist_iterator_base {
	public:
		typedef size_t size_type;
		typedef ptrdiff_t defference_type;
		typedef forward_iterator_tag iterator_category;

		__slist_node_base* node;		//指向节点基本结构

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
	struct __slist_iterator : public __slist_iterator_base {
		typedef __slist_iterator<T, T&, T*>  iterator;
		typedef __slist_iterator<T, const T&, const T*>  const_iterator;
		typedef __slist_iterator<T, Ref, Ptr>  self;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __slist_node<T> slist_node;

		__slist_iterator() : __slist_iterator_base(0) { }
		__slist_iterator(slist_node* x) : __slist_iterator_base(x) { }
		__slist_iterator(const iterator& x) : __slist_iterator_base(x.node) { }

		reference operator*() const {
			return ((slist_node*)node)->data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		self& operator++() {
			incr();
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			incr();
			return tmp;
		}
	};


/* slist结构的实现 */
	template<class T, class Alloc = alloc>
	struct slist {
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			defference_type;

		typedef __slist_iterator<T, T&, T*> iterator;
		typedef __slist_iterator<T, const T&, const T*> const_iterator;

	private:
		typedef __slist_node<T> list_node;
		typedef __slist_node_base list_node_base;
		typedef __slist_iterator_base list_iterator_base;

		//以节点大小为单位的专属空间配置器
		typedef simple_alloc<list_node, Alloc> list_node_allocater;

		static list_node* create_node(const value_type& x) {
			list_node* node = list_node_allocater::allocate();		//配置一个节点的空间
			try {
				construct(&node->data, x);
				node->next = 0;
			}
			catch (...) {
				list_node_allocater::deallocate(node);
				throw;
			}
			return node;
		}
		static void destroy_node(list_node* node) {
			destroy(&node->data);		//析构元素
			list_node_allocater::deallocate(node);		//释放空间
		}
		void clear() {
			list_node* node = (list_node*)head.next;
			while (node != 0) {
				list_node* tmp = node;
				node = (list_node*)node->next;
				destroy_node(tmp);
			}
		}

	private:
		list_node_base head;	//头部。注意这是实体，而不是指针
	public:
		slist()  { head.next = 0; }
		~slist() { clear(); }
	public:
		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size() const { return __slist_size(head.next); }
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& x) {
			__slist_make_link(&head, create_node(x));
		}
		void pop_front() {
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		void insert(iterator ite, const value_type& x) {
			__slist_make_link(ite.node, create_node(x));
		}

		//两个 slist 互换，只要将 head 交换互指即可
		void swap(slist& L) {
			list_node_base* tmp = head.next;
			head.next = L.head.next;
			L.head.next = tmp;
		}
	};


__mySTL_END__


#endif		//end of __SLIST_H__
