#pragma once
#ifndef __HASH_SET_H__
#define __HASH_SET_H__

#include "hashtable.h"		//底层实现机制


__mySTL_BEGIN__

	template<class Value, class HashFunc = std::hash<Value>,
			 class EqualKey = equal_to<Value>, class Alloc = alloc>
	struct hash_set {
	private:
		typedef hashtable<Value, Value, HashFunc, identity<Value>, EqualKey, Alloc>   htb;
		htb rep;		//底层容器以 hashtable 实现
	public:
		typedef typename htb::key_type			key_type;
		typedef typename htb::value_type		value_type;
		typedef typename htb::hasher			hasher;
		typedef typename htb::key_equal			key_equal;
		typedef typename htb::size_type			size_type;
		typedef typename htb::difference_type	difference_type;
		typedef typename htb::const_pointer		pointer;
		typedef typename htb::const_pointer		const_pointer;
		typedef typename htb::const_reference	reference;
		typedef typename htb::const_reference	const_reference;
		typedef typename htb::const_iterator	iterator;
		typedef typename htb::const_iterator	const_iterator;

		hasher hash_funct() const { return rep.hash(); }
		key_equal key_eq() const { return rep.equals(); }

	public:
		hash_set() : rep(50, hasher(), key_equal()) { }		//缺省使用表格大小为50
		explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) { }
		hash_set(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
		hash_set(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

		//注意：以下全部使用 insert_unique
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last) : rep(50, hasher(), key_equal()) {
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last,
			size_type n) : rep(n, hasher(), key_equal())
		{
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last,
			size_type n, const hasher& hf) : rep(n, hf, key_equal())
		{
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last, size_type n,
			const hasher& hf, const key_equal& eql) : rep(n, hf, eql)
		{
			rep.insert_unique(first, last);
		}

		friend bool operator==(const hash_set&, const hash_set&);
	public:
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_bucket_count(); }
		bool empty() const { return rep.empty(); }
		void swap(hash_set& hs) { rep.swap(hs.rep); }
		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }
	public:
		std::pair<iterator, bool> insert(const value_type& obj) {
			std::pair<typename htb::iterator, bool> tmp = rep.insert_unique(obj);
			return std::pair<iterator, bool>(tmp.first, tmp.second);
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			rep.insert(first, last);
		}

		void erase(const key_type& key) { rep.erase(key); }
		void erase(iterator it) { rep.erase(it); }
		void erase(iterator first, iterator last) { rep.erase(first, last); }

		void clear() { rep.clear(); }

		iterator find(const key_type& key) const { rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }
		void resize(size_type new_sz) { rep.resize(new_sz); }
		size_type bucket_count() const { return rep.bucket_count(); }
		size_type max_bucket_size() const { return rep.max_bucket_count(); }
		size_type elems_in_bucket(size_type n) const { return rep.elemts_in_buckets(n); }
	};

	template<class Value, class HashFunc, class EqualKey, class Alloc>
	inline bool operator==(const hash_set<Value, HashFunc, EqualKey, Alloc>& x,
						   const hash_set<Value, HashFunc, EqualKey, Alloc>& y) {
		return x.rep == y.rep;
	}


__mySTL_END__


#endif		//end of __HASH_SET_H__