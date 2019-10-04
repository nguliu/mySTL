#pragma once
#ifndef __SET_H__
#define __SET_H__

#include "rb_tree.h"		//底层实现机制


__mySTL_BEGIN__

/* 注意：set使用RB_tree的 insert_unique 来插入元素，multiset使用 insert_equal */

	//省缺使用递增排序(less)
	template<class Key, class Compare = std::less<Key>, class Alloc = alloc>
	class set {
	public:
		typedef Key key_type;
		typedef Key value_type;		//key_type 与 value_type相同
		typedef Compare key_compare;
		typedef Compare value_compare;
	private:
		//注意：以下第一、第二参数分别为键值和实值型别，对于set键值和实值型别相同
		typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

		rep_type C;		//底层容器采用 RB_tree 实现set

	public:
		/* 注意：以下将set的iterator定义为rb_tree的const_iterator，表示 set 的元素值不可以改变 */
		typedef typename rep_type::const_iterator		iterator;
		typedef typename rep_type::const_iterator		const_iterator;
		typedef typename rep_type::pointer				pointer;
		typedef typename rep_type::const_pointer		const_pointer;
		typedef typename rep_type::reference			reference;
		typedef typename rep_type::const_reference		const_reference;
		typedef typename rep_type::size_type			size_type;
		typedef typename rep_type::difference_type		difference_type;
		//typedef typename rep_type::reverse_iterator	reverse_iterator;

		set() : C(Compare()) { }
		explicit set(const Compare& comp) : C(comp) { }
		template<class InputIterator>
		set(InputIterator first, InputIterator last) : C(Compare()) {
			C.insert_unique(first, last);
		}
		template<class InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp) : C(comp) {
			C.insert_unique(first, last);
		}
		set(const set<Key, Compare, Alloc>& x) : C(x.C) { }

		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
			C = x.C;
			return *this;
		}

		/* 以下的所有操作RB_tree已经提供，只需要传递调用即可 */

		//accessors:
		iterator begin() { return C.begin(); }
		iterator end() { return C.end(); }
		key_compare key_comp() const { return C.key_comp(); }
		value_compare value_comp() const { return C.key_comp(); }
		/*reverse_iterator rbegin() const { return C.rbegin(); }
		reverse_iterator rend() const { return C.rend(); }*/
		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }
		size_type max_size() const { return C.max_size(); }
		void swap(set<Key, Compare, Alloc>& x) { C.swap(x.C); }

		//insert
		std::pair<iterator, bool> insert(const value_type& x) {
			std::pair<typename rep_type::iterator, bool> p = C.insert_unique(x);
			return p;
			//return std::pair<iterator, bool>(p.first, p.second);
		}
		/*iterator insert(iterator position, const value_type& x) {
			return C.insert_unique(position, x);
		}*/
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			C.insert_unique(first, last);
		}

		//erase
		void erase(iterator position) {
			C.erase(position);
		}
		size_type erase(const key_type& x) {
			return C.erase(x);
		}
		void erase(iterator first, iterator last) {
			C.erase(first, last);
		}
		void clear() { C.clear(); }

		//set operations
		iterator find(const key_type& x) { return C.find(x); }
		size_type count(const key_type& x) { return C.count(x); }
		//iterator lower_bound(const key_type& x) const { return C.lower_bound(x); }
		//iterator upper_bound(const key_type& x) const { return C.upper_bound(x); }
		/* 返回元素 x 的区间 */
		//std::pair<iterator, iterator> equal_range(const key_type& x) const { return C.equal_range(x); }

		friend bool operator==(const set& x, const set& y);
		friend bool operator< (const set& x, const set& y);
	};

	template<class Key, class Compare, class Alloc>
	inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.C == y.C);
	}
	template<class Key, class Compare, class Alloc>
	inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.C < y.C);
	}


__mySTL_END__

#endif //end of __SET_H__
