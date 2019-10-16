/*
 *hash_multiset实现
 *
 *hash_multiset使用hashtable作为底层容器，hashtable提供了所有hash_multiset需要的操作
 *hash_multiset使用hashtable的insert_equal来插入元素
 */

#ifndef _STL_HASH_MULTISET_H_
#define _STL_HASH_MULTISET_H_

#include "1stl_alloctor.h"
#include "2stl_algobase.h"
#include "16stl_hashtable.h"

namespace lfp {

	/*	Value:		键和值的合体叫做Value
	 *	HashFunc:	hash函数
	 *	ExtractKey:	如何从Value里面提取出Key(函数或者仿函数)
	 *	EqualKey:	如何比较key(函数或者仿函数)
	 *	Alloc:		内存分配器
	 */
	template<class Value,
			 class HashFunc = std::hash<Value>,
			 class EqualKey = equal_to<Value>,
			 class Alloc = alloc>
	struct hash_multiset {
	private:
		typedef detail::hashtable<Value, Value, HashFunc, identity<Value>, EqualKey, Alloc>   hashtable;

		hashtable htb;
	public:
		typedef typename hashtable::key_type			key_type;
		typedef typename hashtable::value_type			value_type;
		typedef typename hashtable::size_type			size_type;
		typedef typename hashtable::const_iterator		iterator;
		typedef typename hashtable::const_iterator		const_iterator;
		typedef typename hashtable::const_pointer		pointer;
		typedef typename hashtable::const_pointer		const_pointer;
		typedef typename hashtable::const_reference		reference;
		typedef typename hashtable::const_reference		const_reference;
		typedef typename hashtable::difference_type		difference_type;

		HashFunc hash_funct() const { return htb.hash(); }
		EqualKey key_eq() const { return htb.equals(); }

	public:
		hash_multiset() : htb(50, HashFunc(), EqualKey()) { }		//缺省使用表格大小为50
		explicit hash_multiset(size_type n) : htb(n, HashFunc(), EqualKey()) { }
		hash_multiset(size_type n, const HashFunc& hf) : htb(n, hf, EqualKey()) { }
		hash_multiset(size_type n, const HashFunc& hf, const EqualKey& eql) : htb(n, hf, eql) { }

		//注意：以下全部使用 insert_equal
		template<class InputIterator>
		hash_multiset(InputIterator first, InputIterator last) : htb(50, HashFunc(), EqualKey()) {
			htb.insert_equal(first, last);
		}
		template<class InputIterator>
		hash_multiset(InputIterator first, InputIterator last, size_type n)
			: htb(n, HashFunc(), EqualKey())
		{
			htb.insert_equal(first, last);
		}
		template<class InputIterator>
		hash_multiset(InputIterator first, InputIterator last,
			size_type n, const HashFunc& hf) : htb(n, hf, EqualKey())
		{
			htb.insert_equal(first, last);
		}
		template<class InputIterator>
		hash_multiset(InputIterator first, InputIterator last,
				size_type n, const HashFunc& hf, const EqualKey& eql) : htb(n, hf, eql)
		{
			htb.insert_equal(first, last);
		}

	public:
		size_type size() const { return htb.size(); }
		size_type max_size() const { return htb.max_bucket_count(); }
		bool empty() const { return htb.empty(); }
		void swap(hash_multiset& rhs) { htb.swap(rhs.htb); }
		iterator begin() const { return htb.begin(); }
		iterator end() const { return htb.end(); }

	public:
		std::pair<iterator, bool> insert(const value_type& obj) {
			std::pair<typename hashtable::iterator, bool> tmp = htb.insert_equal(obj);
			return std::pair<iterator, bool>(tmp.first, tmp.second);
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			htb.insert(first, last);
		}

		void erase(const key_type& key) { htb.erase(key); }
		void erase(iterator it) { htb.erase(it); }
		void erase(iterator first, iterator last) { htb.erase(first, last); }

		void clear() { htb.clear(); }

		iterator find(const key_type& key) const { return htb.find(key); }
		size_type count(const key_type& key) const { return htb.count(key); }
		void resize(size_type new_sz) { htb.resize(new_sz); }
		size_type bucket_count() const { return htb.bucket_count(); }
		size_type max_bucket_size() const { return htb.max_bucket_count(); }
		size_type elems_in_bucket(size_type n) const { return htb.elemts_in_buckets(n); }
	};

}// end of namespace lfp

#endif	//! _STL_HASH_MULTISET_H_