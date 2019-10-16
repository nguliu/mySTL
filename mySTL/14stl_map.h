/*
 *map实现
 *
 *map使用rb-tree作为底层容器，rb-tree提供了所有map需要的操作
 *map使用RB_tree的 insert_unique 来插入元素，multimap使用 insert_equal
 *
 *map的节点实际保存的是一个pair，它通过投射函数select1st返回第一参数作为Key
 */

#ifndef _STL_MAP_H_
#define _STL_MAP_H_

#include "1stl_alloctor.h"
#include "2stl_algobase.h"	//select1st less
#include "11stl_rbtree.h"
#include <utility>			//pair

namespace lfp {

	//以下Key为键值型别，T为实值型别，省缺使用递增排序(less)
	template<class Key, class T, class Compare = std::less<Key>, class Alloc = alloc>
	class map {
	public:
		typedef Key							key_type;		//键值型别
		typedef T							data_type;		//实值型别
		typedef T							mapped_type;
		typedef std::pair<const Key, T>		value_type;		//map 的元素型别
		typedef Compare						key_compare;	//键值比较仿函数

		//以下定义一个仿函数，其作用是实现元素比较函数（通过调用键值比较函数实现）
		class value_compare : public binary_function<value_type, value_type, bool>
		{
		protected:
			friend class map<Key, T, Alloc>;

			Compare comp;
			value_compare(Compare c) : comp(c) { }
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};
	private:
		//以下第一、第二参数分别为键值和元素型别。键值为pair第一参数，实值为pair，第二参数
		typedef detail::rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;

		rep_type t;
	public:
		/* 以下并不像set将iterator定义为rb_tree的const_iterator，因为map可通过iterator改变元素实值 */
		typedef typename rep_type::iterator				iterator;
		typedef typename rep_type::const_iterator		const_iterator;
		typedef typename rep_type::pointer				pointer;
		typedef typename rep_type::const_pointer		const_pointer;
		typedef typename rep_type::reference			reference;
		typedef typename rep_type::const_reference		const_reference;
		typedef typename rep_type::size_type			size_type;
		typedef typename rep_type::difference_type		difference_type;
		//typedef typename rep_type::reverse_iterator	reverse_iterator;

		map() : t(Compare()) { }
		explicit map(const Compare& comp) : t(comp) { }
		template<class InputIterator>
		map(InputIterator first, InputIterator last) : t(Compare()) {
			t.insert_unique(first, last);
		}
		map(const map<Key, T, Compare, Alloc>& x) : t(x.t) { }

		map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
			t = x.t;
			return *this;
		}

		//以下所有的行为都转调用 rb_tree 的行为
		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return value_compare(t.key_comp()); }

		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		//注意以下 下标操作符，写法虽然有些复杂，但是很巧妙
		T& operator[](const key_type& k) {
			return (*((insert(value_type(k, T()))).first)).second;

			/* insert(value_type(k, T()))返回的是一个 pair<iterator, bool> */
			/* 其第一元素是指向成功插入或发生冲突的map元素（pair<Key, T>）的iterator */
			/* 所以 *((insert(value_type(k, T()))).first) 提领其第一元素，得到一个pair<Key, T> */
			/* 最终 (*((insert(value_type(k, T()))).first)).second 得到这个 pair<Key, T> 的T */
		}

		void swap(map<Key, T, Compare, Alloc>& x) { t.swap(x.t); }

		//insert/erase
		std::pair<iterator, bool> insert(const value_type& x) {
			return t.insert_unique(x);
		}
		/*iterator insert(iterator pos, const value_type& x) {
			return t.insert_unique(pos, x);
		}*/
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}
	
		void erase(iterator pos) { t.erase(pos); }
		void erase(const key_type& x) { t.erase(x); }
		void erase(iterator first, iterator last) { t.erase(first, last); }
		void clear() { t.clear(); }

		//map operations
		iterator find(const key_type& k) { return t.find(k); }
		const_iterator find(const key_type& k) const { return t.find(k); }
		size_type count(const key_type& k) const { return t.count(k); }

		iterator lower_bound(const key_type& k) { return t.lower_bound(k); }
		const_iterator lower_bound(const key_type& k) const {
			return t.lower_bound(k);
		}
		iterator upper_bound(const key_type& k) { return t.upper_bound(k); }
		const_iterator upper_bound(const key_type& k) const {
			return t.upper_bound(k);
		}

		std::pair<iterator, iterator> equal_range(const key_type& x) {
			return t.equal_range(x);
		}

//		friend bool operator==(const map&, const map&);
//		friend bool operator< (const map&, const map&);
	};

//	template<class Key, class T, class Compare, class Alloc>
//	inline bool operator==(const map<Key, T, Compare, Alloc>& x,
//						   const map<Key, T, Compare, Alloc>& y)
//	{
//		return x.t == y.t;
//	}
//	template<class Key, class T, class Compare, class Alloc>
//	inline bool operator<(const map<Key, T, Compare, Alloc>& x,
//		const map<Key, T, Compare, Alloc>& y)
//	{
//		return x.t < y.t;
//	}

} // end of namespace lfp

#endif	//! _STL_MAP_H_
