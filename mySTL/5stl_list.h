/*
 *list的实现，其实是双向链表的实现
 *
 *这里缺省使用内部空间配置器alloc（详见1stl_alloctor.h），使用者也可以自行传入空间配置器
 */

#ifndef _STL_LIST_H_
#define _STL_LIST_H_

#include "1stl_alloctor.h"		//空间配置器
#include "1stl_construct.h"		//对象构造与析构
#include "1stl_iterator.h"		//for function distance
#include "2stl_algobase.h"		//for diatance
#include <cstddef>				//for ptrdiff_t

namespace lfp {
namespace detail {

/* 以下是list节点结构 */
	template<class T>
	struct __node_type {
		typedef __node_type<T>* node_pointer;
		node_pointer prev;		//型别为__node_type<T>*，也可设为 void*
		node_pointer next;
		T data;
	};

/* 以下是list迭代器设计，型别为双向迭代器 */
	template<class T, class Ref = T&, class Ptr = T*>
	struct __list_iterator {
	public:
		typedef __list_iterator<T, Ref, Ptr>		self;
		typedef __list_iterator<T, T&, T*>			iterator;
		typedef const __list_iterator<T, T&, T*>	const_iterator;
		typedef bidirectional_iterator_tag			iterator_category;		//双向迭代器
		typedef T									value_type;
		typedef Ptr									pointer;
		typedef const Ptr							const_pointer;
		typedef Ref									reference;
		typedef const Ref							const_reference;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
		typedef __node_type<T>*						node_pointer;

		node_pointer node;		//迭代器内部拥有一个指针，指向list的节点

		//constructor
		__list_iterator() { }
		__list_iterator(node_pointer x) : node(x) { }
		__list_iterator(const iterator& x) : node(x.node) { }

		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		//以下对迭代器取值，取的是节点的数值
		reference operator*() const { return (*node).data; }
		//以下是迭代器的成员存取运算子的标准做法
		pointer operator->() const { return &(operator*()); }		//先用*运算符取得node的内容，再用&取得地址
		//对迭代器加1就是前进一个节点
		self& operator++() {	//前置++
			node = (*node).next;
			return *this;
		}
		self operator++(int) {	//后置++
			self tmp = *this;
			++*this;
			return tmp;
		}
		//对迭代器减1就是后退一个节点
		self& operator--() {	//前置--
			node = (*node).prev;
			return *this;
		}
		self operator--(int) {	//后置--
			self tmp = *this;
			--*this;
			return tmp;
		}
	};

}// end of namespace detail



/*	以下是list结构的实现	*/
/*	list 为双向循环链表结构	*/
	template<class T, class Alloc = alloc>		//缺省使用 alloc 为空间配置器
	struct list {
	protected:
		typedef detail::__node_type<T> node_type;
	public:
		typedef typename detail::__list_iterator<T, T&, T*>::node_pointer		node_pointer;

		typedef typename detail::__list_iterator<T, T&, T*>::iterator			iterator;
		typedef typename detail::__list_iterator<T, T&, T*>::const_iterator		const_iterator;
		typedef typename detail::__list_iterator<T, T&, T*>::iterator_category	category;
		typedef typename detail::__list_iterator<T, T&, T*>::value_type			value_type;
		typedef typename detail::__list_iterator<T, T&, T*>::pointer			pointer;
		typedef typename detail::__list_iterator<T, T&, T*>::const_pointer		const_pointer;
		typedef typename detail::__list_iterator<T, T&, T*>::reference			reference;
		typedef typename detail::__list_iterator<T, T&, T*>::const_reference	const_reference;
		typedef typename detail::__list_iterator<T, T&, T*>::size_type			size_type;
		typedef typename detail::__list_iterator<T, T&, T*>::difference_type	difference_type;
	protected:
		size_type size_;
		node_pointer node;
		/* 只需要一个指针即可实现双向循环链表。为了实现标准的前闭后开	*/
		/* 区间表示法，node 指向尾端后面的一个空白节点，便于其他操作	*/

		//专属空间配置器，每次配置一个节点大小
		typedef simple_alloc<node_type, Alloc> node_type_allocator;
	
		/* 以下函数分别用来配置、释放、构造、销毁一个节点配置一个节点并传回 */
		node_pointer get_node() { return node_type_allocator::allocate(); }
		//释放一个节点
		void put_node(node_pointer p) { node_type_allocator::deallocate(p); }
		//配置并构造一个节点
		node_pointer create_node(const T& x) {
			node_pointer p = get_node();
			construct(&p->data, x);		//在 p 的 data 成员的地址处构造一个对象
			return p;
		}
		//析构并释放一个节点
		void destroy_node(node_pointer p) {
			destroy(&p->data);	//销毁 data 处的内容
			put_node(p);		//释放内存
		}
		//产生一个空链表
		void empty_initialize() {
			node = get_node();		//配置一个节点，令 node 指向它
			node->next = node;
			node->prev = node;		//node的两个指针均指向自己，不设元素值
			size_ = 0;
		}

		/* list 内部函数，为 splice sort merge 等奠定基础 */
		//将 [first, last) 内的所有元素接合于 pos 所指位置之前，pos 和
		//[first, last)可以指向同一个list，但 pos 不能位于 [first, last) 之内
		void transfer(iterator first, iterator last, iterator pos);

		//sort 内调函数声明
		iterator QuickSortCore(iterator low, iterator high);
		void QuickSort(iterator low, iterator high);

	public:
		list() { empty_initialize(); }
		~list() {
			clear();
			destroy_node(node);
		}

		//这里会构造一个iterator对象
		iterator begin() { return (*node).next; }
		const_iterator begin() const { return (*node).next; }
		iterator end() { return node; }
		const_iterator end() const { return node; }
		bool empty() const { return node->next == node;	}
		size_type size() const { return size_;  }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		//在迭代器pos所指位置插入一个节点，返回插入后该位置的迭代器
		iterator insert(iterator pos, const T& x);

		//移除迭代器pos所指结点，返回移除后该位置的迭代器
		iterator erase(iterator pos);

		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { erase(--end()); }

		//清空整个链表
		void clear();
		//将值为value的所有元素移除
		void remove(const T& value);
		//移除数值相同的连续元素为只剩一个
		void unique();
		
		//将 x 接合于 pos 所指位置，x 必须不同于 *this
		void splice(iterator pos, list& x);
		//将 i 所指的元素接合于 pos 所指位置，pos 和 i 可以指向同一个 list 
		void splice(iterator pos, iterator i);
		//将[first, last)内的所有元素接合于 pos 所指位置，pos 和
		//[first, last)可以指向同一个list，但 pos 不能位于 [first, last) 之内
		void splice(iterator pos, iterator first, iterator last);

		//merge 将x合并到this身上，前提是两个list已经递增排序好了
		void merge(list<T, Alloc>& x);
		//reserve 将this的内容逆向重置
		void reverse();

		//以下我使用快速排序按非递减方式排序
		//list不能使用STL的排序算法，因为STL的sort只接受随机迭代器
		void sort();
	};



	//在迭代器pos所指位置插入一个节点，返回插入后该位置的迭代器
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator pos, const T& x) {
		node_pointer tmp = create_node(x);

		//调整指针，将 tmp 插入
		tmp->next = pos.node;
		tmp->prev = pos.node->prev;
		(pos.node->prev)->next = tmp;
		pos.node->prev = tmp;
		++size_;
		return tmp;
	}

	//移除迭代器pos所指结点，返回移除后该位置的迭代器
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator pos) {
		node_pointer next_node = pos.node->next;
		node_pointer prev_node = pos.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(pos.node);
		--size_;
		return iterator(next_node);
	}

	//清空整个链表
	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		node_pointer cur = node->next;	//begin()
		while (cur != node) {		//遍历每一个节点
			node_pointer tmp = cur;
			cur = cur->next;
			destroy_node(tmp);		//销毁节点
		}
		//将链表恢复到空链表状态
		node->next = node;
		node->prev = node;
		size_ = 0;
	}

	//将值为 value 的所有元素移除
	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			if (*first == value) {
				first = erase(first);
				continue;
			}
			++first;
		}
	}

	//移除数值相同且连续的元素为只剩一个
	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		if (size_ == 0)
			return;

		iterator first = begin();
		iterator last = --end();
		while (first != last) {
			iterator tmp = first++;
			if (*first == *tmp) {
				erase(tmp);
			}
		}
	}

	//将链表 x 接合于 pos 所指位置之前，x必须不同于 *this
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x) {
		if (!x.empty()) {
			transfer(pos, x.begin(), x.end());
			size_ += x.size();
		}
	}

	//将 i 所指的元素接合于 pos 所指位置之前，pos 和 i 可以指向同一个 list 
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, iterator i) {
		iterator j = i;
		++j;
		if (pos == i || pos == j)
			return;
		transfer(pos, i, j);
		++size_;
	}

	//将 [first, last) 内的所有元素接合于 pos 所指位置之前，pos 和
	//[first, last)可以指向同一个list，但 pos 不能位于 [first, last) 之内
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, iterator first, iterator last) {
		if (first != last) {
			tranfer(pos, first, last);
			size_ += distance(first, last);
		}
	}

	//merge 将x合并到this身上，前提是两个list已经递增排序好了
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		size_ += x.size();

		//注意：前提是两个 list 已经经过递增排序
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else {
				++first1;
			}
		}
		if (first2 != last2) {
			transfer(last1, first2, last2);
		}
	}

	//reserve 将this的内容逆向重置
	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		//首先判断链表为空或只有一个元素则不进行任何操作
		if (node->next == node || (node->next)->next == node)
			return;

		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	//对外接口
	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		//首先判断链表为空或只有一个元素则不进行任何操作
		if (node->next == node || (node->next)->next == node)
			return;
		QuickSort(begin(), --end());
	}




	/* list 内部函数，为 splice、sort、merge 等奠定基础 */
	//将 [first, last) 内的所有元素接合于 pos 所指位置，pos 和
	//[first, last)可以指向同一个list，但 pos 不能位于 [first, last) 之内
	template<class T, class Alloc>
	void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last) {
		if (pos != last)
		{
			(last.node)->prev->next = pos.node;
			(first.node)->prev->next = last.node;
			(pos.node)->prev->next = first.node;
			node_pointer tmp = (pos.node)->prev;
			(pos.node)->prev = (last.node)->prev;
			(last.node)->prev = (first.node)->prev;
			(first.node)->prev = tmp;

			//(*((*last.node).prev)).next = pos.node;
			//(*((*first.node).prev)).next = last.node;
			//(*((*pos.node).prev)).next = first.node;
			//node_pointer tmp = (*pos.node).prev;
			//(*pos.node).prev = (*last.node).prev;
			//(*last.node).prev = (*first.node).prev;
			//(*first.node).prev = tmp;
		}
	}


	//以下使用快速排序
	//list 不能使用STL的排序算法，因为STL的sort只接受随机迭代器
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::QuickSortCore(iterator low, iterator high) {
		value_type pivotkey = (low.node)->data;
		while (low != high) {
			while (low != high && (high.node)->data >= pivotkey)
				--high;
			(low.node)->data = (high.node)->data;
			while (low != high && (low.node)->data <= pivotkey)
				++low;
			(high.node)->data = (low.node)->data;
		}
		(low.node)->data = pivotkey;
		return low;		//返回枢轴所在位置
	}

	template<class T, class Alloc>
	void list<T, Alloc>::QuickSort(iterator low, iterator high) {
		if (low != high) {
			iterator pivot = QuickSortCore(low, high);
			if (pivot == low) {
				QuickSort(++pivot, high);
			}
			else if (pivot == high) {
				QuickSort(low, --high);
			}
			else {
				iterator pivot2 = pivot;
				QuickSort(low, --pivot2);		//对前半部分进行排序
				QuickSort(++pivot, high);		//对后半部分进行排序
			}
		}
	}

}// end of namespace lfp

#endif // !_STL_VECTOR_H_
