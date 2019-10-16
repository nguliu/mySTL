#pragma once
#ifndef __MAP_H__
#define __MAP_H__

#include "rb_tree.h"		//底层实现机制


__mySTL_BEGIN__

/* 注意：map使用RB_tree的 insert_unique 来插入元素，multimap使用 insert_equal */

	//以下Key为键值型别，T为实值型别，省缺使用递增排序(less)
	template<class Key, class T, class Compare = std::less<Key>, class Alloc = alloc>
	class map {
	public:
		typedef Key							key_type;		//键值型别
		typedef T							data_type;		//实值型别
		typedef T							mapped_type;
		typedef std::pair<const Key, T>		value_type;		//map 的元素型别
		typedef Compare						key_compare;	//键值比较型别

		//以下定义一个仿函数，其作用就是调用键值比较函数
		class value_compare : public binary_function<value_type, value_type, bool> {
			friend class map<Key, T, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) { }
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};
	private:
		//注意：以下第一、第二参数分别为键值和实值型别
		//键值型别为 pair 第一型别，实值型别为 pair
		typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;

		rep_type C;
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

		map() : C(Compare()) { }
		explicit map(const Compare& comp) : C(comp) { }
		template<class InputIterator>
		map(InputIterator first, InputIterator last) : C(Compare()) {
			C.insert_unique(first, last);
		}
		template<class InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp) : C(comp) {
			C.insert_unique(first, last);
		}
		map(const map<Key, T, Compare, Alloc>& x) : C(x.C) { }

		map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
			C = x.C;
			return *this;
		}

		//以下所有的行为都转调用 rb_tree 的行为
		key_compare key_comp() const { return C.key_comp(); }
		value_compare value_comp() const { return value_compare(C.key_comp()); }
		iterator begin() { return C.begin(); }
		const_iterator begin() const { return C.begin(); }
		iterator end() { return C.end(); }
		const_iterator end() const { return C.end(); }
		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }
		size_type max_size() const { return C.max_size(); }

		//注意以下 下标操作符，写法虽然有些复杂，但是很巧妙
		T& operator[](const key_type& k) {
			return (*((insert(value_type(k, T()))).first)).second;

			/* 注意：insert(value_type(k, T()))返回的是一个 pair<iterator, bool> */
			/*       其第一元素指向成功插入或发生冲突的 map 的元素 pair<Key, T>  */
			/*       所以 *((insert(value_type(k, T()))).first) 提领其第一元素，得到一个map元素 */
			/*       最终 (*((insert(value_type(k, T()))).first)).second 得到 map 元素的实值    */
		}
		void swap(map<Key, T, Compare, Alloc>& x) { C.swap(x.C); }

		//insert/erase
		std::pair<iterator, bool> insert(const value_type& x) {
			return C.insert_unique(x);
		}
		/*iterator insert(iterator pos, const value_type& x) {
			return C.insert_unique(pos, x);
		}*/
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			C.insert_unique(first, last);
		}
	
		void erase(iterator pos) { C.erase(pos); }
		void erase(const key_type& x) { C.erase(x); }
		void erase(iterator first, iterator last) { C.erase(first, last); }
		void clear() { C.clear(); }

		//map operations
		iterator find(const key_type& k) { return C.find(k); }
		const_iterator find(const key_type& k) const { return C.find(k); }
		size_type count(const key_type& k) const { return C.count(k); }

		friend bool operator==(const map&, const map&);
		friend bool operator< (const map&, const map&);
	};

	template<class Key, class T, class Compare, class Alloc>
	inline bool operator==(const map<Key, T, Compare, Alloc>& x,
						   const map<Key, T, Compare, Alloc>& y)
	{
		return x.C == y.C;
	}
	template<class Key, class T, class Compare, class Alloc>
	inline bool operator<(const map<Key, T, Compare, Alloc>& x,
		const map<Key, T, Compare, Alloc>& y)
	{
		return x.C < y.C;
	}


__mySTL_END__

#endif //end of __MAP_H__
