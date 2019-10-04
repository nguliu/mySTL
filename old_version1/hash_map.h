#pragma once
#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include "hashtable.h"		//底层实现机制

__mySTL_BEGIN__

	template<class Key, class T, class HashFunc = std::hash<Key>,
			 class EqualKey = equal_to<Key>, class Alloc = alloc>
	struct hash_map {
	private:
		typedef hashtable<std::pair<const Key, T>, Key, HashFunc, select1st<std::pair<const Key, T>>, EqualKey, Alloc> htb;
		htb rep;		//底层以 hashtable 实现
	public:
		typedef T		 data_type;
		typedef T		 mapped_type;
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
		hash_map() : rep(50, hasher(), key_equal()) { }		//缺省使用表格大小为50
		explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) { }
		hash_map(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
		hash_map(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

		//注意：以下全部使用 insert_unique
		template<class InputIterator>
		hash_map(InputIterator first, InputIterator last) : rep(50, hasher(), key_equal()) {
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_map(InputIterator first, InputIterator last,
			size_type n) : rep(n, hasher(), key_equal())
		{
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_map(InputIterator first, InputIterator last,
			size_type n, const hasher& hf) : rep(n, hf, key_equal())
		{
			rep.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_map(InputIterator first, InputIterator last, size_type n,
			const hasher& hf, const key_equal& eql) : rep(n, hf, eql)
		{
			rep.insert_unique(first, last);
		}

		friend bool operator==(const hash_map&, const hash_map&);
	public:
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_bucket_count(); }
		bool empty() const { return rep.empty(); }
		void swap(hash_map& hs) { rep.swap(hs.rep); }
		iterator begin() { return rep.begin(); }
		iterator end() { return rep.end(); }
		const_iterator begin() const { return rep.begin(); }
		const_iterator end() const { return rep.end(); }
	public:
		std::pair<iterator, bool> insert(const value_type& obj) { return rep.insert_unique(obj); }
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) { rep.insert(first, last); }

		void erase(const key_type& key) { rep.erase(key); }
		void erase(iterator it) { rep.erase(it); }
		void erase(iterator first, iterator last) { rep.erase(first, last); }

		void clear() { rep.clear(); }

		iterator find(const key_type& key) { rep.find(key); }
		const_iterator find(const key_type& key) const { rep.find(key); }

		T& operator[](const key_type& key) {
			return ((*(rep.insert_unique(value_type(key, T())).first)).second);
		}

		size_type count(const key_type& key) const { return rep.count(key); }
		void resize(size_type new_sz) { rep.resize(new_sz); }
		size_type bucket_count() const { return rep.bucket_count(); }
		size_type max_bucket_size() const { return rep.max_bucket_count(); }
		size_type elems_in_bucket(size_type n) const { return rep.elemts_in_buckets(n); }
	};

	template<class Key, class T, class HashFunc, class EqualKey, class Alloc>
	inline bool operator==(const hash_map<Key, T, HashFunc, EqualKey, Alloc>& x,
						   const hash_map<Key, T, HashFunc, EqualKey, Alloc>& y) {
		return x.rep == y.rep;
	}



__mySTL_END__

#endif //end of __HASH_MAP_H__
