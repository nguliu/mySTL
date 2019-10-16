#pragma once
#ifndef __HASH_TABLE__
#define __HASH_TABLE__


#include "alloctor.h"		//空间配置与管理
#include "construct.h"		//内存对象的构造、析构与重构等
#include "algobase.h"		//基本算法相关
#include "iterator.h"		//迭代器相关
#include <utility>			//包含 pair

#include "vector.h"			//用于实现聚合体

__mySTL_BEGIN__

/* hash table 节点结构 */
	template<class Value>
	struct __hashtable_node {
		Value val;
		__hashtable_node* next;
	};


/* hash table 结构声明 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
	struct hashtable;


/* hash table 迭代器定义 */
	//注意：hashtable 的迭代器没有后退操作，也没有reverse iterator
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
	struct __hashtable_iterator {
	public:
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>	hashtable;
		typedef __hashtable_iterator<Value, Key,
									 HashFunc, ExtractKey, EqualKey, Alloc>		iterator;
		typedef __hashtable_iterator<const Value, const Key,
									 HashFunc, ExtractKey, EqualKey, Alloc>		const_iterator;
		typedef Value					value_type;
		typedef size_t					size_type;
		typedef Value&					reference;
		typedef Value*					pointer;
		typedef ptrdiff_t				difference_type;
		typedef forward_iterator_tag	iterator_category;
		typedef __hashtable_node<Value> node_type;

		node_type* cur;		//迭代器目前所指结点
		hashtable* htb;		//指向hash table 的bucket，保持与容器之间的连结关系

	public:
		__hashtable_iterator() { }
		__hashtable_iterator(node_type* _cur, hashtable* _htb) : cur(_cur), htb(_htb) { }

		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }

		iterator& operator++() {
			const node_type* old = cur;
			cur = cur->next;		//如果存在就是他，否则进入以下 if 流程
			if (cur == nullptr) {
				size_type bucket = htb->bkt_num(old->val);		//找出当前元素所在的桶

				/* 寻找第一个存在元素的桶，其第一个元素即为所求 */
				while (!cur && ++bucket < htb->buckets.size()) {
					cur = htb->buckets[bucket];
				}
			}
			return *this;
		}
		//后置++，返回自加前的对象
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;		//调用operator++
			return tmp;
		}

		bool operator==(const iterator& it) { return (cur == it.cur); }
		bool operator!=(const iterator& it) { return (cur != it.cur); }
	};



/* hash table 结构定义 */
	/*
	 *	Value:		键和值的合体叫做Value
	 *	Key:		key节点键的类别
	 *	HashFcn:	hash函数
	 *	ExtractKey:	如何从Value里面提取出Key(函数或者仿函数)
	 *	EqualKey:	如何比较key(函数或者仿函数)
	 *	Alloc:		内存分配
	 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	struct hashtable {
	public:
		typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey,
											  EqualKey, Alloc>::iterator			iterator;
		typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey,
											  EqualKey, Alloc>::const_iterator		const_iterator;
		typedef Value			value_type;
		typedef Key				key_type;
		typedef size_t			size_type;
		typedef HashFunc		hasher;
		typedef EqualKey		key_equal;
		typedef ptrdiff_t		difference_type;
		typedef Value*			pointer;
		typedef const Value*	const_pointer;
		typedef Value&			reference;
		typedef const Value&			const_reference;
		typedef __hashtable_node<Value> node_type;
		typedef simple_alloc<node_type, Alloc> node_allocater;

		/* bucket 聚合体，以vector实现便于扩展 */
		vector<node_type*, Alloc> buckets;

	private:
		hasher		hash;
		key_equal	equals;
		ExtractKey	get_key;
		size_type	num_elements;	//元素个数
	protected:
		/* 配置一个新节点 */
		node_type* create_node(const value_type& x) {
			node_type* node = node_allocater::allocate();
			node->next = nullptr;
			try {
				construct(&node->val, x);
				return node;
			}
			catch (...) {
				node_allocater::deallocate(node);
				throw;
			}
		}
		/* 销毁一个节点 */
		void delete_node(node_type* node) {
			destroy(&node->val);
			node_allocater::deallocate(node);
		}

		/* 初始化配置 buckets */
		void initialize_buckets(size_type n) {
			const size_type bucket_size = next_size(n);
			buckets.resize(bucket_size, nullptr);		//修改 buckets 大小，并初始化新空间为空
			num_elements = 0;
		}

		/* 判断是否需要重建表格，如果不需要就立刻返回，否则重建一个更大的表格 */
		void resize(size_type num_elements_hint);
		/* 在不重建表格的情况下插入节点。键值不允许重复 */
		std::pair<iterator, bool> insert_unique_noresize(const value_type& x);
		/* 在不重建表格的情况下插入节点。键值不允许重复 */
		iterator insert_equal_noresize(const value_type& x);
		/* 实际执行删除元素的操作 */
		void __erase(const value_type& x);
		/* nest_size 返回最接近 n 且不小于 n 的质数 */
		size_type next_size(size_type n) const { return __stl_next_prime(n); }

	public:
		/* 计算元素 x 落脚点的函数 */

		//版本1：接受实值(value)和 buckets 个数
		size_type bkt_num(const value_type& obj, size_t n) const {
			return bkt_num_key(get_key(obj), n);		//调用版本4
		}
		//版本2：只接受实值
		size_type bkt_num(const value_type& obj) const {
			return bkt_num_key(get_key(obj));			//调用版本3
		}
		//版本3：只接受键值
		size_type bkt_num_key(const key_type& key) const {
			return bkt_num_key(key, buckets.size());		//调用版本4
		}
		//版本4：接受键值和 buckets 个数
		size_type bkt_num_key(const key_type& key, size_t n) const {
			//return (key % n);
			return hash(key) % n;
		}

	public:
		hashtable(size_type n, const HashFunc& hf, const EqualKey& eql)
		: hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0){
			initialize_buckets(n);
		}

		iterator begin() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				if (buckets[i])
					return iterator(buckets[i], this);
			}
			return end();
		}
		iterator end() { return iterator(nullptr, this); }
		size_type size() const { return num_elements; }					//返回元素个数
		bool empty() { return num_elements == 0; }
		size_type bucket_count() const { return buckets.size(); }	//返回聚合体vector的大小
		size_type max_bucket_count() const {
			return __stl_prime_list[__stl_nun_primes - 1];
			//其值将会是 4294967291ul
		}
		/* 计算 n 号桶的元素个数 */
		int elemts_in_buckets(size_type n) {
			int count = 0;
			for (node_type* first = buckets[n]; first; first = first->next)
				++count;
			return count;
		}

		/* 查找键值为 key 的元素 */
		iterator find(const key_type& key) {
			size_type n = bkt_num_key(key);		//找到 key 落在哪个桶内

			node_type* cur = buckets[n];
			while (cur) {
				if (equals(get_key(cur->val), key))
					return iterator(cur, this);
				cur = cur->next;
			}
			return end();
		}
		const_iterator find(const key_type& key) const {
			size_type n = bkt_num_key(key);		//找到 key 落在哪个桶内

			node_type* cur = buckets[n];
			while (cur) {
				if (equals(get_key(cur->val), key))
					return iterator(cur, this);
				cur = cur->next;
			}
			return end();
		}
		size_type count(const key_type& key) {
			int result = 0;
			iterator it = find(key);
			if (it != end()) {
				node_type* cur = it.cur;
				while (cur) {
					if (equals(get_key(cur->val), key))
						++result;
					cur = cur->next;
				}
			}
			return result;
		}

		/* 插入元素，不允许重复 */
		std::pair<iterator, bool> insert_unique(const value_type& x) {
			resize(num_elements + 1);		//判断是否需要重建表格
			return insert_unique_noresize(x);
		}
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_unique(*first);
		}
		/* 插入元素，允许重复 */
		iterator insert_equal(const value_type& x) {
			resize(num_elements + 1);		//判断是否需要重建表格
			return insert_equal_noresize(x);
		}
		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_equal(*first);
		}

		/* 删除元素 */
		void erase(iterator it) { __erase(*it); }
		void erase(node_type* node) { __erase(node->val); }
		void erase(const value_type& x) { __erase(x); }
		template<class InputIterator>
		void erase(InputIterator first, InputIterator last) {
			while (first != last) {
				InputIterator tmp = first++;
				__erase(*tmp);		//注意不能直接使用 first 进行删除，这会使 first 失效
			}
		}

		/* 清空 hash table */
		void clear();

		/* 复制 ht 到本hashtable */
		void copy_from(const hashtable& ht);

		void swap(hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& ht) {
			size_type size1 = buckets.size();
			size_type size2 = ht.buckets.size();
			size_type min_size = size1 < size2 ? size1 : size2;

			int i = 0;
			for (; i < min_size; ++i) {
				std::swap(buckets[i], ht.buckets[i]);
			}

			if (size1 < size2) {
				while (i < size2) 
					buckets.push_back(ht.buckets[i++]);
				ht.buckets.erase(ht.buckets.begin() + size1, ht.buckets.end());
			}
			else {
				while (i < size1)
					ht.buckets.push_back(buckets[i++]);
				buckets.erase(buckets.begin() + size2, buckets.end());
			}
			std::swap(num_elements, ht.num_elements);
		}
	};


	/* 在不重建表格的情况下插入节点。键值不允许重复 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool>
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);		//找到 obj 应该插入的串行
		node_type* first = buckets[n];			//令 first 指向对应串行的头部

		/* 查看当前串行中是否存在和 obj 相同的元素 */
		for (node_type* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj)))
				return std::pair<iterator, bool>(iterator(cur, this), false);
			//如存在相同的元素则立即返回
		}

		/* 进行到此说明当前串行为空或无重复元素，可进行插入 */
		node_type* tmp = create_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return std::pair<iterator, bool>(iterator(tmp, this), true);
	}

	/* 在不重建表格的情况下插入节点。键值允许重复 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);		//找到 obj 应该插入的串行
		node_type* first = buckets[n];			//令 first 指向对应串行的头部

		/* 查找串行中是否存在与 obj 相同的元素，如果有则立即插入 */
		for (node_type* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				node_type* tmp = create_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}

		/* 进行到此说明当前串行为空或无重复元素 */
		node_type* tmp = create_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}


	/* 以下函数判断是否需要重建表格，如果不需要就立刻返回，否则重建一个更大的表格 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
	{	
		const size_type bucket_size = buckets.size();
		
		/* 注意是通过比较聚合体大小和元素个数判断是否需要重建 */
		if (num_elements_hint > bucket_size) {
			const size_type new_size = next_size(bucket_size);		//找出下一个质数
			vector<node_type*, Alloc> tmp_buckets(buckets);			//建立一个临时的 buckets 用于保存原始 buckets 的内容

			//将原 buckets 清空（注意只是清空了buckets的指针，原来的链表还在）
			for (int i = 0; i < bucket_size; ++i)
				buckets[i] = nullptr;

			buckets.resize(new_size);		//将原 buckets 扩大为新 buckets

			//以下处理每个旧 buckets 的每条链表
			for (size_type i = 0; i < bucket_size; ++i) {
				node_type* first = tmp_buckets[i];			//指向节点链表对应的第一个节点

				/* 以下处理 first 对应链表的每一个节点 */
				while (first != nullptr) {
					/* 首先找出节点落在新 buckets 的哪个位置 */
					size_type pos = bkt_num(first->val, new_size);

					/* 以下4步实现节点交换 */
					tmp_buckets[i] = first->next;	//将节点 first 从旧 buckets 的链表中删除
					first->next = buckets[pos];
					buckets[pos] = first;			//将first插入新buckets的串行
					first = tmp_buckets[i];			//令 first 重新指向旧buckets的第一个节点，以便处理后序节点
				}
			}
		}
	}

	/* 清空 hash table */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
		for (size_type i = 0; i < buckets.size(); ++i) {		//对 buckets 的每一个元素进行处理
			node_type* cur = buckets[i];
			while (cur) {
				node_type* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = nullptr;
		}
		num_elements = 0;
		//注意：clear操作并未释放buckets vector 的空间
	}

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
		//先将自己的 buckets 清空
		clear();
		buckets.size(ht.buckets.size());

		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				//复制每一个存在的串行
				node_type* cur = ht.buckets[i];
				if (cur != nullptr) {
					node_type* copy = create_node(cur->val);
					buckets[i] = copy;

					/* 复制当前串行的每一个节点 */
					for (cur = cur->next; cur; cur = cur->next) {
						copy->next = create_node(cur->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;
		}
		catch (...) {
			clear();
		}
	}

	/* 实际执行删除元素的操作 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::__erase(const value_type& x) {
		size_type n = bkt_num(x);
		node_type* cur = buckets[n];
		if (cur) {
			if (equals(get_key(cur->val), get_key(x))) {
				buckets[n] = cur->next;
				delete_node(cur);
				--num_elements;
				return;
			}

			while (cur->next) {
				if (equals(get_key(cur->next->val), get_key(x))) {
					node_type* tmp = cur->next;
					cur->next = tmp->next;
					delete_node(tmp);
					--num_elements;
					return;
				}
				cur = cur->next;
			}
		}
	}



__mySTL_END__

#endif //end of __HASH_TABLE__