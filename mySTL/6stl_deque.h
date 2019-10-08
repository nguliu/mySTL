/*
 *deque的实现，其内实现为分段连续的存储空间
 *
 *deque是序列式容器中最复杂的容器
 */
#ifndef _STL_DEQUE_H_
#define _STL_DEQUE_H_

#include "1stl_alloctor.h"		//空间配置器
#include "1stl_construct.h"		//对象构造与析构
#include "1stl_iterator.h"		//for function distance
#include "1stl_uninitialized.h"
#include "2stl_algobase.h"

namespace lfp {
namespace detail {

/* 全局函数: 用于辅助决定缓冲区大小
 * 如果buf_size不为0，传回由用户定义的buf_size，表示一个缓冲区可以容纳 buf_size 个元素
 * 如果 buf_size 为0，表示 buffer size 使用默认值，那么：
 * 	 如果 val_size(sizeof(value_type))小于512，返回 512 / val_size，一个缓冲区可容纳 512 / val_size 个元素
 * 	 如果 val_size >= 512，返回 1，一个缓冲区可容纳 1 个元素
 */
inline size_t __deque_buf_size(size_t buf_size, size_t val_size) {
	return (buf_size != 0) ? buf_size
			: ( (val_size < 512) ? size_t(512 / val_size) : size_t(1) );
}


/* deque 迭代器的设计 */
	template<class T, class Ref, class Ptr, size_t BufSize>
	struct __deque_iterator {
	public:
		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }
		typedef __deque_iterator<T, T&, T*, BufSize>				iterator;
		typedef __deque_iterator<T, const T&, const T*, BufSize>	const_iterator;

		//未继承 std::iterato 必须自行定义五个必要迭代器相应型别
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Ptr							pointer;
		typedef const Ptr					const_pointer;
		typedef Ref							reference;
		typedef const Ref					const_reference;
		typedef ptrdiff_t					difference_type;

		typedef size_t						size_type;
		typedef T**							map_pointer;
		typedef __deque_iterator			self;

		//保持与容器的联结
		T* cur;				//指向 [first,last) 所指缓冲区行中的一个元素，表示当前迭代器所指元素
		T* first;			//指向缓冲区行的头
		T* last;			//指向缓冲区行的尾，永远指向最后一个元素的下一位置
		map_pointer node;	//指向中控器中当前迭代器所指节点

		// 以下是迭代器运算的一些关键行为 
		/* 使 node 跳跃到新的缓冲区 new_node */
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());		//指向当前缓冲区最后一个元素的下一位置
		}
		//以下各个运算子是 __deque_itreator<> 成功运作的关键
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		/* 重载 - 运算符，用于计算两个迭代器之间的距离 */
		/* 注意：两个迭代器的 this 指针均不发生改变 */
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1)	  //两个迭代器之间除头尾以外的缓冲区（他们是绝对饱和的）
									+ (cur - first)			//左操作迭代器所指缓冲区内元素个数（该缓冲区可能未饱和）
									+ (x.last - x.cur);		//右操作迭代器所指缓冲区内元素个数（该缓冲区可能未饱和）
		}

		// 以下操作实现迭代器的 自加/自减 运算，均会改变迭代器的 this 指针
		/* 前置++，返回自加后的 this */
		self& operator++() {
			++cur;						//切换至下一个元素
			if (cur == last) {			//如果已经到达缓冲区的尾端（最后一个元素的下一位置）
				set_node(node + 1);		//就切换至下一个缓冲区节点的第一个元素
				cur = first;
			}
			return *this;
		}
		/* 后置++，返回自加前的 this 的拷贝 */
		self operator++(int) {
			self tmp = *this;
			++*this;		//调用前置++
			return tmp;
		}
		/* 前置--，返回自减后的 this */
		self& operator--() {
			if (cur == first) {		//如果已经到达缓冲区的头端
				set_node(node - 1);	//就切换至前一个缓冲区节点
				cur = last;			//cur 指向最后一个元素的下一位置
			}
			--cur;			//切换至前一个元素
			return *this;
		}
		/* 后置--，返回自减前的 this 的拷贝 */
		self operator--(int) {
			self tmp = *this;
			--*this;		//调用前置--
			return tmp;
		}

		// 以下实现随机访问。迭代器可以直接跳跃 n 个距离 

		/* 重载 += 运算符。注意：+= 运算符改变 this 指针 */
		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);		//算出移动后的 cur 相对于当前 first 的位置
			if (offset >= 0 && offset < difference_type(buffer_size())) {
				cur += n;	 //目标位置在当前缓冲区内
			}
			else {		//目标位置不在当前缓冲区内
				difference_type node_offset = (offset > 0) ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				//切换到正确的缓冲区节点
				set_node(node + node_offset);
				//切换至正确的元素
				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}
			return *this;
		}
		/* 重载 + 运算符。注意：+ 运算符不改变 this 指针 */
		self operator+(difference_type n) const {
			self tmp = *this;
			return tmp += n;
		}
		/* 重载 -= 运算符，利用 operator+= 实现。注意：-= 运算符改变 this 指针 */
		self& operator-=(difference_type n) {
			return *this += -n;		
		}
		/* 重载 - 运算符，利用 operator-= 实现。注意：- 运算符不改变 this 指针 */
		self  operator- (difference_type n) const {
			self tmp = *this;
			return tmp -= n;
		}
		/* 随机访问，调用 operator* 和 operator+。不改变 this 指针 */
		reference operator[](difference_type n) const { return *(*this + n); }

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
		bool operator< (const self& x) const {
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}
	};

}// end of namespace detail



/* deque 结构的设计 */
	template<class T, class Alloc = alloc, size_t BufSize = 0>
	struct deque {
	public:			//Basic types
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::iterator_category	category;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::iterator				iterator;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::const_iterator		const_iterator;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::value_type			value_type;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::pointer				pointer;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::const_pointer		const_pointer;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::reference			reference;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::const_reference		const_reference;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::size_type			size_type;
		typedef typename detail::__deque_iterator<T, T&, T*, BufSize>::difference_type		difference_type;
		
		size_t (*buffer_size)() = detail::__deque_iterator<T, T&, T*, BufSize>::buffer_size;

	protected:
		typedef T** map_pointer;

		iterator start;			//指向第一个缓冲区节点
		iterator finish;		//指向最后一个缓冲区节点
		map_pointer map;		//指向中控器map，map是连续空间，其每个元素都指向某行缓冲区（缓冲区连续）
		size_type map_size;		//map 总共课容纳缓冲区的个数

		//deque 专属空间配置器
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		/* 以一个元素大小为单位进行配置 */
		typedef simple_alloc<pointer, Alloc> map_allocator;
		/* 以一个节点指针大小为单位进行配置 */

		//用于维护 deque 结构的内部方法

		void fill_initialize(size_type n, const value_type& value);
		/* 负责实现deque的结构，并将元素设定为初值 value */
		void create_map_and_nodes(size_type num_elem);
		/* 负责产生并安排好deque的内存结构，不设定初值 */

		/* 产生更大的新 map，复制旧 map 并释放 */
		void reallocate_map(size_type nodes_to_add, bool add_at_front);

		//push_back() 内调函数
		void push_back_aux(const value_type& t);
		//push_back_aux() 内调函数
		void reserve_map_at_back(size_type nodes_to_add = 1) {
			if (nodes_to_add + 1 > map_size - (finish.node - map)) {
				//如果 map 尾端节点的备用空间不足
				//则必须更换一个更大的map（配置更大的、拷贝原来的、释放原来的）
				reallocate_map(nodes_to_add, false);
			}
		}

		//push_front() 内调函数
		void push_front_aux(const value_type& t);
		//push_front_aux() 内调函数
		void reserve_map_at_front(size_type nodes_to_add = 1) {
			if (nodes_to_add > size_type(start.node - map)) {
				//如果 map 前端节点的备用空间不足
				//则必须更换一个更大的map（配置更大的、拷贝原来的、释放原来的）
				reallocate_map(nodes_to_add, true);
			}
		}

		//insert() 内调函数
		iterator insert_aux(iterator pos, const value_type& x);

	/* deque 对外接口 */
	public:		//Basic accessors
		deque() : start(), finish(), map(0), map_size(0) {
			create_map_and_nodes(0);		//拥有0个元素、1个缓冲区、map大小为 3
		}
		deque(int n, const value_type& value) : start(), finish(), map(0), map_size(0) {
			fill_initialize(n, value);
		}

		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		reference operator[](size_type n) {
			return start[difference_type(n)];		//调用 __deque_iterator<>::operator[]
		}
		reference front() { return *start; }		//调用 __deque_iterator<>::operator*
		const_reference front() const { return *start; }		//调用 __deque_iterator<>::operator*
		reference back() {
			iterator tmp = finish;
			--tmp;			//调用 __deque_iterator<>::operator--
			return *tmp;	//调用 __deque_iterator<>::operator*
			//以上三行不能改为 return *(finish - 1) 
			//因为 operator-(difference_type n) 的操作比 -- 复杂很多
		}
		const_reference back() const {
			iterator tmp = finish;
			--tmp;			//调用 __deque_iterator<>::operator--
			return *tmp;	//调用 __deque_iterator<>::operator*
			//以上三行不能改为 return *(finish - 1) 
			//因为 operator-(difference_type n) 的操作比 -- 复杂很多
		}

		size_type size() const { return finish - start; }
		//以上调用 __deque_iterator<>::operator-(self&)
		size_type max_size() const { return size_type(-1); }
		bool empty() const { return finish == start; }

		void push_back(const value_type& t) {
			if (finish.cur != finish.last - 1) {		//最后的缓冲区还有至少两个空间（为了维持左闭右开区间）
				construct(finish.cur, t);		//直接在备用空间上构造元素
				++finish.cur;		//调整 finish.cur 指向最后一个元素的下一位置
			}
			else {		//最后缓冲区只剩一个元素备用空间，在原 finish 的末尾添加元素
				push_back_aux(t);		//需要再生成一个缓冲区，并使 finish.cur 指向新缓冲区的第一个位置
			}
		}

		void push_front(const value_type& t) {
			if (start.cur != start.first) {		//第一缓冲区内尚有备用空间
				construct(start.cur - 1, t);		//直接在备用空间上构造元素
				--start.cur;		//调整第一缓冲区的使用状态
			}
			else {		//第一缓冲区已无备用空间
				push_front_aux(t);		//需要再生成一个缓冲区
			}
		}

		void pop_back() {
			if (finish.cur != finish.first) {	//最后一个缓冲区有最少一个元素
				--finish.cur;			//调整指针，相当于排除了最后元素
				destroy(finish.cur);	//将最后元素析构
			}
			else {		//最后缓冲区没有元素，这里将进行缓冲区的释放
				data_allocator::deallocate(finish.first, buffer_size());		//释放最后一个缓冲区
				finish.set_node(finish.node - 1);		//调整 finish 的状态，使其指向上一个缓冲区的最后一个元素
				finish.cur = finish.last - 1;
				destroy(finish.cur);		//将该元素析构
			}
		}

		void pop_front() {
			if (start.cur != start.last - 1) {	//第一缓冲区有至少两个元素
				destroy(start.cur);		//将第一元素销毁
				++start.cur;			//调整指针，相当于排除了第一元素
			}
			else {			//第一缓冲区仅有一个元素，这里将进行缓冲区的释放
				destroy(start.cur);		//将第一个缓冲区的最后一个元素析构
				data_allocator::deallocate(start.first, buffer_size());		//释放第一缓冲区
				start.set_node(start.node + 1);		//调整缓冲区的状态，使其指向下一个缓冲区的第一个元素
				start.cur = start.first;
			}
		}

		//清空deque。注意：最终需要保留一个缓冲区，这是deque的策略，也是deque的初始状态
		void clear();
		//删除 pos 所指元素，pos 为清除点
		iterator erase(iterator pos);
		//删除 [first, last) 内的所有元素
		iterator erase(iterator first, iterator last);
		//在 pos 出插入一个值为 x 的元素
		iterator insert(iterator pos, const value_type& x);
	};




	/* fill_initialize() 负责安排好deque的结构，并将元素设定初值 */
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
		create_map_and_nodes(n);		//负责产生并安排好deque的内存结构
		map_pointer cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur) {
				/* 先为每个饱和节点设定初值 */
				uninitialized_fill(*cur, *cur + buffer_size(), value);
			}
			/* 最后一个节点的设定稍有不同，因为它可能不饱和 */
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...) {
			/* 先将已构造的元素销毁 */
			for (map_pointer beg = start.node; beg < cur; ++beg)
				destroy(*cur, *cur + buffer_size());
			/* 将所有缓冲区释放 */
			for (cur = start.node; cur <= finish.node; ++cur)
				data_allocator::deallocate(*cur, buffer_size());
			/* 将 map 指针所指空间释放 */
			map_allocator::deallocate(map, map_size);
			throw;
		}
	}
	
	/* create_map_and_nodes()负责申请 num_elements 个元素的存储空间，并安排好deque的结构 */
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
		//需要节点数 = (元素个数 / 缓冲区大小可容纳元素个数) + 1，如果刚好整除会多配一个节点设为last
		size_type num_nodes = num_elements / buffer_size() + 1;

		//一个map最少管理3个节点，最多管理所需节点数+2，前后各留一个便于扩展
		map_size = num_nodes + 2;
		map = map_allocator::allocate(map_size);
		
		//以下令 nstart 和 nfinish 指向map所拥有的全部节点的最中央区域
		//保持在最中央，可使头尾两端的扩充能量一样大，每个节点课对应一个缓冲区
		map_pointer nstart = map + 1;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try {
			//为map的节点 [nstart,nfinish] 配置缓冲区，deque的可用空间为 buffer_size() * (nfinish - nstart + 1)
			for (cur = nstart; cur <= nfinish; ++cur)
				*cur = data_allocator::allocate(buffer_size());
		}
		catch (...) {
			//若非全部成功，就将 [nstart, cur) 内配置完成的缓冲区全部释放
			for (map_pointer b = nstart; b < cur; ++b)
				data_allocator::deallocate(*b, buffer_size());
			//将 map 指针所指空间释放
			map_allocator::deallocate(map, map_size);
			throw;
		}
		/* 设置 start 指向第一个缓冲区、start.cur指向第一个缓冲区的第一个元素 */
		start.set_node(nstart);
		start.cur = start.first;

		/* 设置 finish 指向最后一个缓冲区、finish.cur指向最后一个元素的下一位置 */
		finish.set_node(nfinish);
		//为了维持前闭后开区间属性，如果未整除，则 finish.cur 指向最后一个元素的下一个位置，此时 finish 指向的块内有元素
		//如果刚好能整除，前面说会多配一个节点，此时令 finish.cur 指向这多配的一个节点的起始处，此时 finish 指向的块内没有元素
		finish.cur = finish.first + (num_elements % buffer_size());
	}


	/* 重新生成一个更大的 map */
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;		//计算之前实际缓冲区个数
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		/* 如果旧map的大于2倍实际需要的缓冲区节点数（即map有一半还没有用到） */
		if (map_size > 2 * new_num_nodes) {
			new_nstart = map + (map_size - new_num_nodes) / 2	//将缓冲区新起点移动到 map 中间位置
						+ (add_at_front ? nodes_to_add : 0);
			/* 如果新起点在原起点的前面 */
			if (new_nstart < start.node) {
				copy(start.node, finish.node + 1, new_nstart);
				/* 将原缓冲区信息向前拷贝 */
			}
			else {
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
				/* 将原缓冲区信息向后拷贝 */
			}
		}
		else {		//重新配置一块空间作为 map
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
						+ (add_at_front ? nodes_to_add : 0);

			//接下来把原map内容拷贝过来，释放原map，设定新map的起始地址和大小
			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		//更新 start 和 finish
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	//push_back() 内调函数
	/* 只有当 finish.cur == finish.last - 1 是才会被调用 */
	/* 也就是说只有当最后一个缓冲区只剩一个元素存储空间了才会被调用 */
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();		//当 map 的备用空间已用完时，无法再增加缓冲区节点，必须换一个更大的 map
		*(finish.node + 1) = data_allocator::allocate(buffer_size());		//配置一个新缓冲区节点
		try {
			construct(finish.cur, t_copy);		//在原始 finish 所指缓冲区的最后一个存储位置构造元素
			finish.set_node(finish.node + 1);	//更新 finish 所指节点
			finish.cur = finish.first;			//设置 finish 的状态
		}
		catch (...) {		//发生异常，将新配置的节点(finish所指节点的下一个节点)释放
			data_allocator::deallocate(*(finish.node + 1));
			throw;
		}
	}

	//push_front() 内调函数
	/* 只有当 start.cur == start.first 时才会被调用 */
	/* 也就是只有当第一缓冲区内没有任何备用空间时才会被调用 */
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_front();		//当 map 的备用空间已用完时，无法再增加缓冲区节点，必须换一个更大的 map
		*(start.node - 1) = data_allocator::allocate(buffer_size());		//配置一个新缓冲区节点
		try {
			start.set_node(start.node - 1);		//更新 start 指向新节点
			start.cur = start.last - 1;			//设置 start 的状态
			construct(start.cur, t_copy);		//构造元素
		}
		catch (...) {		//发生异常，释放新节点，恢复到原始状态
			start.set_node(start.node + 1);
			start.cur = start.first;
			data_allocator::deallocate(*(start.node - 1));
			throw;
		}
	}

	//清空deque。注意：最终需要保留一个缓冲区，这是deque的策略，也是deque的初始状态
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::clear() {
		/* 以下针对头尾以外的每一个缓冲区，他们一定都是饱满的 */
		for (map_pointer node = start.node + 1; node < finish.node; ++node) {
			destroy(*node, *node + buffer_size());
			/* 将缓冲区内的所有元素析构，调用的是destroy的第二版本 */
			data_allocator::deallocate(*node, buffer_size());
			/* 释放缓冲区内存 */
		}

		if (start.node != finish.node) {			//至少有头尾两个缓冲区
			destroy(start.cur, start.last);			//将头缓冲区内目前所有元素析构
			destroy(finish.first, finish.cur);		//将尾缓冲区内目前所有元素析构
			//以下释放缓冲区。注意：头缓冲区保留
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else {		//只有一个缓冲区
			destroy(start.cur, finish.cur);		//将元素析构。注意这里不释放缓冲区
		}
		//调整状态
		start.cur = start.first;
		finish = start;		
	}

	//清除 pos 所指元素，pos 为清除点
	template<class T, class Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	deque<T, Alloc, BufSize>::erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type elem_before = pos - start;		//清除点之前的元素个数
		if (elem_before < (finish - next)) {			//清除点之前的元素比较少就移动清除点之前的元素
			copy_backward(start, pos, next);
			pop_front();		//移动完毕，第一个元素多余，将其清除
		}
		else {			//清除点之后的元素比较少，移动清除点之后的元素
			copy(next, finish, pos);
			pop_back();			//移动完毕，最后一个元素多余，将其清除
		}
		return start + elem_before;
	}

	//删除 [first, last) 内的所有元素
	template<class T, class Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	deque<T, Alloc, BufSize>::erase(iterator first, iterator last) {
		if (first == start && last == finish) {		//如果清除整个 deque，直接调用 clear 就可以了
			clear();
			return finish;
		}
		else {
			difference_type len = last - first;				//清除区间长度
			difference_type elem_before = first - start;	//清除区间前方的元素个数
			if (elem_before < (finish - last)) {			//如果前方的元素比较少
				copy_backward(start, first, last);		//向后移动前方的元素，覆盖掉清除区间
				iterator new_start = start + len;		//标记deque的新起点
				destroy(start, new_start);				//移动完毕，将多余的元素析构
				/* 以下将多余的缓冲区释放 */
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				start = new_start;		//设定 deque 新起点
			}
			else {			//清除区间后方的元素比较少
				copy(last, finish, first);		//将后方的元素向前移动，覆盖掉清除区间
				iterator new_finish = finish - len;		//标记 deque 的新尾点
				destroy(new_finish, finish);			//移动完毕，将多余的元素析构
				/* 以下将多余的缓冲区释放 */
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				finish = new_finish;		//设定 deque 的新尾点
			}
			return start + elem_before;
		}
	}

	//在 pos 处插入一个值为 x 的元素。返回元素的插入位置
	template<class T, class Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	deque<T, Alloc, BufSize>::insert(iterator pos, const value_type& x) {
		if (pos.cur == start.cur) {			//如果插入点是 deque 的最前端
			push_front(x);		//直接调用push_front()
			return start;
		}
		else if (pos.cur == finish.cur) {		//插入点是 deque 的最尾端
			push_back(x);		//交给push_back去处理
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else {
			return insert_aux(pos, x);
		}
	}

	//insert() 内调函数
	template<class T, class Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& x) {
		value_type x_copy = x;
		difference_type elem_before = pos - start;		//插入点之前的元素个数
		
		if (elem_before < (finish - pos)) {			//如果插入点之前的元素个数比较少
			iterator front_old = start;				//记录最初的起始位置
			push_front(front());					//在最前端加入一个与第一元素相同的元素
			iterator move_front = front_old + 1;	//原起始位置的元素已压入最前端，因此从原起始位置的下一位置开始移动
			copy(move_front, pos, front_old);		//将 [move_front, pos) 内的元素前移一格
			--pos;		//pos 前移指向插入位置
		}
		else {			//插入点之后的元素个数比较少，在尾端进行操作，过程同上
			iterator back_old = finish;
			push_back(back());
			iterator move_back = back_old - 1;		//从原结束位置的前一位置开始复制
			copy_backward(pos, move_back, back_old);		//移动元素
			//注意：此时 pos 已经指向正确的插入位置
		}
		*pos = x_copy;		//修改插入位置的元素值
		return pos;
	}

}// end of namespace lfp

#endif // !_STL_DEQUE_H_
