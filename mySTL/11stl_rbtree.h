/*
 *红黑树是一种自平衡二叉搜索树，可提供对数时间的插入和访问操作，其平衡性不如AVL树高，
 *因此其维护平衡性的成本也不如AVL树高，相当于在平衡性和效率之间取了折中
 *
 *红黑树的定义为：
 *	·每个节点不是红色就是黑色
 *	·根节点为黑色
 *	·红节点的子节点必须为黑
 *	·任意节点到叶子节点的任意路径所含黑节点数必须相同
 *
 *红黑树的详细讲解请查看：https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
 *
 *红黑树删除节点算法请看：https://blog.csdn.net/qq_40843865/article/details/102498310
 */

#ifndef _STL_RB_TREE_H_
#define _STL_RB_TREE_H_

#include "1stl_alloctor.h"
#include "1stl_construct.h"
#include "8stl_queue.h"		//queue 用于清空红黑树
#include <utility>			//pair
#include <string.h>


namespace lfp {
namespace detail {

/* 结点颜色 */
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = true;
	const __rb_tree_color_type __rb_tree_black = false;

/* rb_tree 节点采用双层架构 */
/* rb_tree 基础节点结构，基础节点存储颜色、父节点和左右子节点 */
	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;	//节点颜色
		base_ptr parent;	//父节点指针
		base_ptr left;		//左子节点
		base_ptr right;		//右子节点

		static base_ptr get_min_node(base_ptr x) {
			while (x->left != nullptr)
				x = x->left;
			return x;
		}
		static base_ptr get_max_node(base_ptr x) {
			while (x->right != nullptr)
				x = x->right;
			return x;
		}
		
		//获得其兄弟节点，只能在已知当前节点有brother节点的情况下调用，否则结果未可预期
		base_ptr get_brother() {
			if (this->parent->left == this)
				return this->parent->right;
			return this->parent->left;
		}
	};

/* rb_tree 正规节点结构，正规节点存储元素值 */
	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<Value>* node_pointer;
		Value value_field;		//节点值
	};



/* RB_tree 基层迭代器结构 */
/* 注意：对于rb_tree，begin()指向 left_most，end()指向 header */
/* --begin() 或 ++right_most 得到 header，--end() 得到 right_most */
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base::base_ptr	base_ptr;
		typedef bidirectional_iterator_tag		iterator_category;		//双向迭代器
		typedef ptrdiff_t						difference_type;

		base_ptr node;		//用于产生和容器之间的连结关系

		__rb_tree_iterator_base(base_ptr n) : node(n) { }

		// operator++
		void incrment() {
			if (node->right == node)
				return;

			if (node->right != nullptr) {		//如果node有右子节点，就找到node右子树的最左节点即为解答
				node = node->right;
				while (node->left != nullptr) {
					node = node->left;
				}
			}
			else {			//node 没有右子节点
				base_ptr parent = node->parent;		//找到父节点
				while (parent->right == node) {		//如果现行节点本身是右子节点
					node = parent;					//就一直上溯，直至不为右子节点为止
					parent = node->parent;
				}

				if (node->right != parent) {		//若此时的右子节点不等于父节点
					node = parent;					//则父节点即为解答，否则此时的node为解答
				}
				//这里判断“若此时的右子节点不等于父节点”是为了应付一种特殊情况：我们求right-most
				//结点的下一节点。以上特殊做法必须配合RB-tree根节点与header之间的特殊关系
			}
		}

		// operator--
		void decrment() {
			if (node->left == node)
				return;
			if (node->color == __rb_tree_red &&		//如果node是红节点，且父节点的父节点等于自己，表示此时node指向
				node->parent->parent == node)		//header（即end()）。相当于执行 --end()，right-most 即为解答
			{
				node = node->right;		//注意：header之右子节点即right-most，执行整棵树的max节点
			}
			else if (node->left != nullptr) {	//如果有左子节点
				node = node->left;				//令 node 指向其左子节点
				while (node->right != nullptr)	//找到左子节点的最右节点即为解答
					node = node->right;					
			}
			else {	//node既header节点也无左子节点
				base_ptr parent = node->parent;	//找到 node 的父节点

				while (node == parent->left) {			//当现行节点身为左子节点
					node = parent;						//一直往上走，直至不为左子节点
					parent = node->parent;
				}
				node = parent;			//此时的父节点即为解答
			}
		}
	};

/* RB_tree 正规迭代器结构 */
	template<class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base {
		typedef Value	value_type;
		typedef Ref		reference;
		typedef Ptr		pointer;
		typedef __rb_tree_node<Value>*	node_pointer;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;		//这里是顶层的const，表示不能修改元素的值

		__rb_tree_iterator() : __rb_tree_iterator_base(nullptr) { }
		__rb_tree_iterator(node_pointer x) : __rb_tree_iterator_base(x) { }
		__rb_tree_iterator(const iterator& x) : __rb_tree_iterator_base(x.node) { }

		reference operator*() const { return node_pointer(node)->value_field; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
		pointer operator->() const { return &(operator*()); }
#endif
		/* 前置++，返回自加后的this */
		self& operator++() {
			incrment();
			return *this;
		}
		/* 后置++，返回自加前的this */
		self operator++(int) {
			self tmp = *this;
			incrment();
			return tmp;
		}
		/* 前置--，返回自减后的this */
		self& operator--() {
			decrment();
			return *this;
		}
		/* 后置--，返回自减前的this */
		self operator--(int) {
			self tmp = *this;
			decrment();
			return tmp;
		}

		bool operator==(const iterator& x) const { return (node == x.node); }
		bool operator!=(const iterator& x) const { return (node != x.node); }
		bool operator==(const const_iterator& x) const { return (node == x.node); }
		bool operator!=(const const_iterator& x) const { return (node != x.node); }
	};


/* 相关函数声明 */
	void __rb_tree_rotate_left(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rotate_right(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rebalance(__rb_tree_node_base*, __rb_tree_node_base*&);




/* RB_tree 结构的定义 */
	/* Key:			rb_tree键值型别 */
	/* Value:		rb_tree实际存储的元素型别，通过对应键值的出排列顺序 */
	/* KeyOfValue:	键值提取方法，得出某个元素实值所对应的键值 */
	/* Compare:		键值比较规则 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
	struct rb_tree {
	protected:
		typedef __rb_tree_color_type	color_type;
		typedef __rb_tree_node_base*	base_ptr;
		typedef __rb_tree_node<Value>	rb_tree_node;
		typedef rb_tree<Key, Value, KeyOfValue, Compare, Alloc> self;

		/* 定义专属空间配置器，每次配置一个节点的空间 */
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	public:
		typedef Key					key_type;
		typedef Value				value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef rb_tree_node*		node_pointer;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::iterator		 iterator;
		typedef typename __rb_tree_iterator<value_type, const reference, const pointer>::const_iterator	 const_iterator;

	protected:
		/* RB_tree 只用以下三笔数据表现 */
		node_pointer header;		//头指针，其parent域和root互相指向对方，其left、right分别指向最小、最大节点
		size_type node_count;		//节点数量
		Compare key_compare;		//节点键值大小比较准则


		/* 以下三个函数用来方便取得 header 的成员 */
		node_pointer& root() const { return (node_pointer&) header->parent; }
		node_pointer& leftmost() const { return (node_pointer&) header->left; }
		node_pointer& rightmost() const { return (node_pointer&) header->right; }

		/* 以下六个函数方便用来取得node_pointer节点的成员 */
		static node_pointer& left(node_pointer x) { return (node_pointer&) (x->left); }
		static node_pointer& right(node_pointer x) { return (node_pointer&) (x->right); }
		static node_pointer& parent(node_pointer x) { return (node_pointer&) (x->parent); }
		static reference value(node_pointer x) { return x->value_field; }
		static const Key& key(node_pointer x) { return KeyOfValue()(x->value_field); }	  //提取元素对应的键值
		static color_type& color(node_pointer x) { return (color_type&) (x->color); }

		/* 以下六个函数用来方便取得base_ptr节点的成员 */
		static node_pointer& left(base_ptr x) { return (node_pointer&) (x->left); }
		static node_pointer& right(base_ptr x) { return (node_pointer&) (x->right); }
		static node_pointer& parent(base_ptr x) { return (node_pointer&) (x->parent); }
		static reference value(base_ptr x) { return ((node_pointer)x)->value_field; }
		static const Key& key(base_ptr x) { return KeyOfValue()(value(x)); }
		static color_type& color(base_ptr x) { return (color_type&)(node_pointer(x)->color); }

		//求取树 x 的最大值和最小值
		static node_pointer get_min_node(node_pointer x) {
			return (node_pointer)__rb_tree_node_base::get_min_node(x);
		}
		static node_pointer get_max_node(node_pointer x) {
			return (node_pointer)__rb_tree_node_base::get_max_node(x);
		}

	protected:
		node_pointer get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(node_pointer p) { rb_tree_node_allocator::deallocate(p); }

		/* 生成一个节点 */
		node_pointer create_node(const value_type& x) {
			node_pointer node = get_node();			//配置空间
			try {
				construct(&(node->value_field), x);	//构造对象
			}
			catch (...) {
				put_node(node);
			}
			return node;
		}
		/* 复制一个节点（颜色和值） */
		node_pointer clone_node(const node_pointer x) {
			node_pointer node = create_node(x->value_field);
			node->color = x->color;
			node->left = nullptr;
			node->right = nullptr;
			return node;
		}
		/* 销毁一个节点 */
		void destroy_node(node_pointer x) {
			destroy(&x->value_field);
			put_node(x);
		}

	private:
		/* 内部插入函数: x为新值插入点，y为插入点的父节点，v为待插入值。返回指向新插入节点的迭代器 */
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		
		/* 复制整棵树 x */
		node_pointer __copy(node_pointer x);

		/* 删除节点 x，并进行相应的调整 */
		void __erase_node(node_pointer x);

		//删除时组合2比较复杂，这里单独使用一个函数来处理
		void __erase_node_case2_reblance(node_pointer x);


		void init() {
			header = get_node();			//产生一个节点作为header
			color(header) = __rb_tree_red;	//令header为红色，用来区分header和root，在iterator.operator--中使用
			root() = nullptr;				//root 为空，即 header->parent
			leftmost() = header;			//令 header 的左右子节点为自己
			rightmost() = header;		
		}


	public:
		rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
			init();
		}
		rb_tree(const self& x) {		//拷贝构造函数
			init();
			*this = x;					//调用 operator=
		}
		~rb_tree() {
			clear();
			put_node(header);
		}

		self& operator=(const self& x);	//拷贝赋值运算符

		/* 清空 rb_tree(除header) */
		void clear();

		/* 将树*this和x交换，只需要交换header指针即可 */
		void swap(self& x) {
			node_pointer tmp = header;
			*(&header) = x.header;
			*(&(x.header)) = tmp;
		}

		/* 寻找RB_tree中是否存在键值为 k 的节点 */
		const_iterator find(const Key& k) const;
		/* 注意以下代码复用的技巧 */
		iterator find(const Key& k) {
			const_iterator it = static_cast<const self&>(*this).find(k);
			node_pointer p = static_cast<node_pointer>(it.node);
			return iterator(p);
		}

		/* 统计元素x的个数 */
		size_type count(const key_type& x) const {
			size_type x_count = 0;
			const_iterator it = find(x);		//当有多个相同元素时，find 返回第一个元素的迭代器

			if (it != end()) {
				++x_count;
				
				//向后遍历查看是否还存在键值为key的元素
				while (++it != end()) {
					if (!key_compare(x, key(it.node)) && !key_compare(key(it.node), x))	 // !(a<b) && !(b<a) 即 a==b
						++x_count;
					else
						break;
				}
			}
			return x_count;
		}

		Compare key_comp() const { return key_compare; }
		iterator begin() { return leftmost(); }		//第一个节点即 left-most
		const_iterator begin() const { return leftmost(); }
		iterator end() { return header; }			//end即header
		const_iterator end() const { return header; }
		bool empty() const { return (node_count == 0); }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }


		/* 插入新值：键值不允许重复，重复则插入失败 */
		std::pair<iterator, bool> insert_unique(const value_type& v);

		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			while (first != last) {
				insert_unique(*first);
				++first;
			}
		}

		/* 插入新值：键值允许重复 */
		iterator insert_equal(const value_type& v);

		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			while (first != last) {
				insert_equal(*first);
				++first;
			}
		}

		/* 删除节点 x */
		void erase(node_pointer x) { __erase_node(x); }
		void erase(iterator x) { __erase_node(node_pointer(x.node)); }
		size_type erase(const Key& x) {
			iterator it = find(x);
			size_type count = 0;

			while (it != end()) {
				__erase_node(node_pointer(it.node));
				it = find(x);
				++count;
			}

			return count;
		}
		void erase(iterator first, iterator last) {
			while (first != last) {
				iterator tmp = first++;
				erase(tmp);
			}
		}

		iterator lower_bound(const key_type& x) { return lower_bound(begin(), end(), x); }
		const_iterator lower_bound(const key_type& x) const { return lower_bound(begin(), end(), x); }
		iterator upper_bound(const key_type& x) { return upper_bound(begin(), end(), x); }
		const_iterator upper_bound(const key_type& x) const { return upper_bound(begin(), end(), x); }

		/* 返回元素 x 的区间 */
		std::pair<iterator, iterator> equal_range(const key_type& x) {
			iterator first = find(x);
			iterator last = find(x);
			while (last != end()) {
				if (!key_compare(x, key(last.node)) && !key_compare(key(last.node), x))
					++last;
				else
					break;
			}

			return std::pair<iterator, iterator>(first, last);
		}
	};



	/* 拷贝赋值运算符 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const self& x) {
		clear();		//先将原始的树清空

		key_compare = x.key_compare;
		header->parent = __copy(x.root());
		if (header->parent != nullptr)
		{
			root()->parent = header;
			node_count = x.node_count;

			leftmost() = get_min_node(root());
			rightmost() = get_max_node(root());
		}

		return *this;
	}


	/* 清空rb_tree(除header) */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
	{
		if (header->parent != nullptr) {
			queue<base_ptr> node_queue;

			node_queue.push(header->parent);
			while (!node_queue.empty()) {
				base_ptr cur = node_queue.front();
				node_queue.pop();
				
				if (cur->left != nullptr)
					node_queue.push(cur->left);
				if (cur->right != nullptr)
					node_queue.push(cur->right);
				destroy_node(node_pointer(cur));
			}

			/* 将 rb_tree 恢复到空状态 */
			node_count = 0;
			root() = nullptr;		//root 为空，即 header->parent
			leftmost() = header;	//令 header 的左右子节点为自己
			rightmost() = header;
		}
	}


	/* 寻找RB_tree中是否存在键值为 k 的节点 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const {
		node_pointer y = header;
		node_pointer x = root();

		//注意：当存在相同元素时，find 返回第一个元素的迭代器（递增迭代器可访问其他相同元素）

		/* 以下写法比较费事，无论k处于哪个位置都必须走到叶节点才停止 */
//		while (x != nullptr) {
//			if (key_compare(key(x), k))
//				x = right(x);			//运行到这里表示 x 键值小于 k，x 向右走
//			else
//				y = x, x = left(y);		//运行到这里表示 x 键值大于或等于 k，x 向左走
//		}
//		iterator j = iterator(y);
//		return ( j == end() || key_compare(k, key(j.node)) ) ? end() : j;

		while (x != nullptr) {
			if (key_compare(key(x), k))
				x = right(x);			//运行到这里表示 x 键值小于 k，x 向右走
			else if ( key_compare(k, key(x)) )
				x = left(x);			//运行到这里表示 x 键值大于 k，x 向左走
			else
				return iterator(x);		//返回第一个目标元素
		}

		return end();
	}


	/* 插入新值：节点值不允许重复，重复则插入无效 */
	/* 返回一个pair，第一个元素指向RB_tree中新增节点，第二个元素表示是否插入成功 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
		node_pointer y = header;
		node_pointer x = root();
		bool comp = true;

		while (x != nullptr) {		//从根节点开始，向下寻找适当的插入点
			y = x;

			comp = key_compare(KeyOfValue()(v), key(x));		//less
			x = comp ? left(x) : right(x);
		} //结束后，x指向插入点，y指向x的父节点

		iterator j = iterator(y);

		/* 如果comp为真，表示 v < x */
		if (comp) {
			if (j == begin())	//如果插入节点的父节点为 left_most，则直接插入
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			else		//否则将调整 j 用于后面测试
				--j;
		}
		/* 如果comp为假，表示 v >= x，如果是 v > x 则进行插入 */
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		}

		/* 进行到这里，键值一定重复，插入失败 */
		/* 注意：这里有个技巧是返回一个第一元素指向发生冲突 */
		/* 的节点的迭代器，在map的下标操作中起了重要的作用 */
		return std::pair<iterator, bool>(j, false);
	}

	/* 插入新值：节点值允许重复 */
	/* 返回指向新插入节点的迭代器 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {

		/* 注意：当插入相同元素时，新元素成为已有元素的后继（即已有元素右孩子的最左节点） */

		node_pointer y = header;
		node_pointer x = root();		//从根节点开始往下寻找适当的插入点
		while (x != nullptr) {
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);		//less
		}
		return __insert(x, y, v);
		// x为新值插入点，y为插入点的父节点，v为待插入值
	}


	/* 真正的插入程序: x为新值插入点，y为插入点的父节点，v为待插入值。返回指向新插入节点的迭代器 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y, const value_type& v) {
		node_pointer x = (node_pointer)_x;
		node_pointer y = (node_pointer)_y;

		node_pointer new_node = create_node(v);			//产生一个新节点
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			left(y) = new_node;		//这使得当y为header时，leftmost() = new_node
			if (y == header) {
				root() = new_node;
				rightmost() = new_node;
			}
			else if (y == leftmost()) {
				leftmost() = new_node;
			}
		}
		else {
			right(y) = new_node;		//令新节点为插入点父节点的右子节点
			if (y == rightmost()) {
				rightmost() = new_node;		//维护 rightmost 永远指向最右节点
			}
		}
		parent(new_node) = y;		//设定新节点的父节点
		left(new_node) = 0;
		right(new_node) = 0;		//设定新节点的左右子节点

		//节点颜色一定为红，将在全局函数__rb_tree_rebalance() 中设定并调整
		__rb_tree_rebalance(new_node, header->parent);		//参数一为新增节点，参数二为 root
		++node_count;
		return iterator(new_node);		//返回指向新增节点的迭代器
	}

	/* 复制整棵树 x */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(node_pointer x) {
		if (x == nullptr)
			return nullptr;

		node_pointer x_copy = clone_node(x);
		x_copy->left = __copy(left(x));
		x_copy->right = __copy(right(x));

		if (x_copy->left != nullptr)
			x_copy->left->parent = x_copy;
		if (x_copy->right != nullptr)
			x_copy->right->parent = x_copy;

		return x_copy;
	}

	
	//以下算法的具体过程请看这里：https://blog.csdn.net/qq_40843865/article/details/102498310

	/* 删除节点 x，并进行相应的调整 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node(node_pointer x)
	{
		if (x->left == nullptr && x->right == nullptr)	//删除叶节点
		{
			if (x->color == __rb_tree_red)		//组合1：删除红色的叶节点，直接删除即可（因为红节点不可能是root）
			{
				if (leftmost() == x)
					leftmost() = (node_pointer)x->parent;
				else if (rightmost() == x)
					rightmost() = (node_pointer)x->parent;
			}
			else {									//组合2：最麻烦的情况
				if (root() == x) {	//删除根节点
					root() = nullptr;
					leftmost() = header;
					rightmost() = header;
				}
				else {		//否则调用组合2的处理函数，此时x一定有兄弟节点（你不妨画个图出来看一下）
					__erase_node_case2_reblance(x);

					if (leftmost() == x)
						leftmost() = (node_pointer)x->parent;
					if (rightmost() == x)
						rightmost() = (node_pointer)x->parent;
				}
			}

			if (x->parent->left == x)
				x->parent->left = nullptr;
			else
				x->parent->right = nullptr;

			destroy_node(x);
			--node_count;
		}
		else if (x->left == nullptr) {		//一定是组合4：x是黑节点，它的右孩子是红节点（因为组合3不可能存在）
			node_pointer son = (node_pointer)x->right;
			son->parent = x->parent;
			son->color = __rb_tree_black;

			if (x->parent->left == x)
				x->parent->left = son;
			else
				x->parent->right = son;

			if (root() == x) {		//如果删除的是root节点，一点要调整相关指针
				root() = son;
			}
			if (leftmost() == x)
				leftmost() = son;

			destroy_node(x);
			--node_count;
		}
		else if (x->right == nullptr) {		//一定是组合4：x是黑节点，它的左孩子是红节点（因为组合3不可能存在）
			node_pointer son = (node_pointer)x->left;
			son->parent = x->parent;
			son->color = __rb_tree_black;

			if (x->parent->left == x)
				x->parent->left = son;
			else
				x->parent->right = son;
			
			if (root() == x) {		//如果删除的是root节点，一点要调整相关指针
				root() = son;
			}
			if (rightmost() == x)
				rightmost() = son;

			destroy_node(x);
			--node_count;
		}
		else {								//组合5、6：x有两个儿子
			iterator it = iterator(x);
			++it;	//这里找x的后继，也可以找x的前驱

			memmove(&(x->value_field), &(((node_pointer)it.node)->value_field), sizeof(x->value_field));
			//x->value_field = *it;  //std::pair deleted operator=

			__erase_node((node_pointer)it.node);	//转化为删除节点x的后继，会回到组合1 2 4中的一种
		}
	}

	/* 组合2处理函数（这里只是进行旋转或者变色，并不删除节点） */
	// 这里传入的x在算法思路上我们认为它含有一个额外的黑色权重，且x一定有brother节点
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node_case2_reblance(node_pointer x) {
		node_pointer brother = (node_pointer)x->get_brother();

		if (brother->color == __rb_tree_black)		//情形一 二 三
		{
			if ((brother->left && brother->left->color == __rb_tree_red) ||		//情形一 二
				(brother->right && brother->right->color == __rb_tree_red))
			{
				if (brother == x->parent->right)
				{
					if (brother->right != nullptr) {	//情形一
						__rb_tree_rotate_left(x->parent, header->parent);
						brother->right->color = __rb_tree_black;
						brother->color = x->parent->color;
						x->parent->color = __rb_tree_black;
					}
					else {							//情形二
						__rb_tree_rotate_right(brother, header->parent);
						brother->color = __rb_tree_red;
						brother->parent->color = __rb_tree_black;  //这里brother->parent是原来brother的儿子节点

						__erase_node_case2_reblance(x);	  //这里回到情形一，递归重新调整
					}
				}
				else	//brother == x->parent->left
				{
					if (brother->left != nullptr) {	  //情形一
						__rb_tree_rotate_right(x->parent, header->parent);
						brother->left->color = __rb_tree_black;
						brother->color = x->parent->color;
						x->parent->color = __rb_tree_black;
					}
					else {							//情形二
						__rb_tree_rotate_left(brother, header->parent);
						brother->color = __rb_tree_red;
						brother->parent->color = __rb_tree_black;  //这里brother->parent是原来brother的儿子节点

						__erase_node_case2_reblance(x);	//这里回到情形一，递归重新调整
					}
				}
			}
			else					//情形三
			{
				if (x == root())	//递归结束
					return;
				if (x->parent->color == __rb_tree_red)	//情形3.1
				{
					x->parent->color = __rb_tree_black;
					brother->color = __rb_tree_red;
				}
				else {				//情形3.2
					brother->color = __rb_tree_red;

					__erase_node_case2_reblance((node_pointer)x->parent);	//对parent递归这个过程（这时parent在算法思想上黑色权重为2）

					//以上将brother子树的黑节点减少一层，将x的黑色权重2（只是在算法思路上存在）移一个到father，
					//递归这个过程，直至遇到其他情形解决问题或递归到根节点
				}
			}
		}
		else				//情形四（brother为红，x->parent一定为黑）
		{
			if (brother == x->parent->right) {
				__rb_tree_rotate_left(x->parent, header->parent);
			}
			else {
				__rb_tree_rotate_right(x->parent, header->parent);
			}

			brother->color = __rb_tree_black;
			x->parent->color = __rb_tree_red;

			__erase_node_case2_reblance(x);		//对x重新执行这个过程，此时一定是情形一 二 三中的一种
		}
	}




/* 全局函数：调整树形使其平衡（改变颜色及旋转树形） */
/* 参数一为新增节点，参数二为 root */
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = __rb_tree_red;		//新节点必为红（根据规则4）

		while (x != root && x->parent->color == __rb_tree_red)	//父节点也为红色
		{
			if (x->parent == x->parent->parent->left)		//父节点为祖父节点的左子节点
			{
				__rb_tree_node_base* y = x->parent->parent->right;	//令 y 为伯父节点

				if (y && y->color == __rb_tree_red)			//伯父节点存在且为红色（对应P211状况4）
				{
					y->color = __rb_tree_black;				//更改伯父节点为黑
					x->parent->color = __rb_tree_black;		//更改父节点为黑
					x->parent->parent->color = __rb_tree_red;	//更改祖父节点为红

					x = x->parent->parent;		/* x 上升到祖父节点，下次继续判断祖父节点和太祖父节点是否同时为红 */
				}
				else	//无伯父节点，或伯父节点为黑
				{
					if (x == x->parent->right)			//如果新节点为父节点的右子节点（对应P210状况2）
					{
						x = x->parent;					//修改 x 为其父节点，先对其父节点进行一次左旋操作
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;			//将父节点改为黑
					x->parent->parent->color = __rb_tree_red;	//将祖父节点改为红
					__rb_tree_rotate_right(x->parent->parent, root);	//对祖父节点执行右旋

					/* 此时x和兄弟节点(原祖父节点)为红，x父节点为黑，已满足rb_tree要求，下次循环将退出while */
				}
			}
			else		//父节点为祖父节点的右子节点
			{
				__rb_tree_node_base* y = x->parent->parent->left;	//令 y 为伯父节点

				if (y && y->color == __rb_tree_red)		//伯父节点存在且为红色（对应上述状况4的镜像）
				{
					y->color = __rb_tree_black;				//更改伯父节点为黑
					x->parent->color = __rb_tree_black;		//更改父节点为黑
					x->parent->parent->color = __rb_tree_red;	//更改祖父节点为红

					x = x->parent->parent;		/* x 上升到祖父节点，下次继续判断祖父节点和太祖父节点是否同时为红 */
				}
				else	///无伯父节点，或伯父节点为黑
				{
					if (x == x->parent->left)		//如果新节点为父节点的左子节点（对应上述状况2的镜像）
					{
						x = x->parent;				//修改 x 为其父节点，先对其父节点进行一次右旋操作
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;			//将父节点改为黑
					x->parent->parent->color = __rb_tree_red;	//将祖父节点改为红
					__rb_tree_rotate_left(x->parent->parent, root);		//对祖父节点执行左旋

					/* 此时x和兄弟节点(原祖父节点)为红，x父节点为黑，已满足rb_tree要求，下次循环将退出while */
				}
			}
		}		//while 结束
		root->color = __rb_tree_black;		//根节点永远为黑
	}

/* 全局函数：x为左旋点，root为根节点 */
/* 新节点必为红节点，如果插入点的父节点也为红节点，则可能需要进行树形旋转及颜色改变 */
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->right;		//令 y 为左旋点的右子节点
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;		//别忘了设定父节点
		}
		y->parent = x->parent;

		//令 y 完全顶替 x 的地位（必须将 x 对其父节点的关系完全接收过来）
		if (x == root) {					//如果 x 为根节点，将 y 设为新的根节点
			root = y;
			x->parent->parent = y;			//令header的parent指向新的根节点
		}
		else if (x == x->parent->left)		//x 为其父节点的左子节点
			x->parent->left = y;
		else								//x 为其父节点的右子节点
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

/* 全局函数：x为右旋点，root为根节点 */
/* 新节点必为红节点，如果插入点的父节点也为红节点，则可能需要进行树形旋转及颜色改变 */
	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->left;		//令 y 为右旋点的左子节点
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;		//别忘了设定父节点
		}
		y->parent = x->parent;

		//令 y 完全顶替 x 的地位（必须将 x 对其父节点的关系完全接收过来）
		if (x == root) {					//如果 x 为根节点，将 y 设为新的根节点
			root = y;
			x->parent->parent = y;			//令header的parent指向新的根节点
		}
		else if (x == x->parent->right)		//x 为其父节点的右子节点
			x->parent->right = y;
		else								//x 为其父节点的左子节点
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

} // end of namespace detail
} // end of namespace lfp

#endif	//!_STL_RB_TREE_H_