#pragma once
#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "alloctor.h"		//空间配置与管理
#include "construct.h"		//内存对象的构造、析构与重构等
#include "algobase.h"		//基本算法相关
#include "queue.h"			//queue 用于清空红黑树
#include <utility>			//包含 pair


__mySTL_BEGIN__

typedef bool	__rb_tree_color_type;
#define RED		false		//红色为0
#define BLACK	true		//黑色为1


/* rb_tree 基本节点结构 */
	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;		//节点颜色
		base_ptr parent;		//指向父节点
		base_ptr left;
		base_ptr right;

		static base_ptr get_min_node(base_ptr x) {
			if (!x)
				return nullptr;

			while (x->left != nullptr)
				x = x->left;
			return x;
		}
		static base_ptr get_max_node(base_ptr x) {
			if (!x)
				return nullptr;

			while (x->right != nullptr)
				x = x->right;
			return x;
		}
	};

/* rb_tree 节点结构 */
	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<Value>* link_type;
		Value value_field;		//节点值
	};


/* 函数声明 */
	void __rb_tree_rotate_left(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rotate_right(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rebalance(__rb_tree_node_base*, __rb_tree_node_base*&);


/* RB_tree 基层迭代器结构 */
/* 注意：对于rb_tree，begin()指向 left_most，end()指向 header */
/*		 --begin() 或 ++right_most 得到 header，--end() 得到 right_most */
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base::base_ptr	base_ptr;
		typedef bidirectional_iterator_tag		iterator_category;		//双向迭代器
		typedef ptrdiff_t						difference_type;

		base_ptr node;		//用于产生和容器之间的连结关系

		// operator ++
		void incrment() {
			if (node->right != nullptr) {		//如果node有右子节点，就找到node右子树的最左节点即为解答
				node = node->right;
				while (node->left != nullptr) {
					node = node->left;
				}
			}
			else {			//node 没有右子节点
				base_ptr _parent = node->parent;	//找到父节点
				while (_parent->right == node) {	//如果现行节点本身是右子节点
					node = _parent;					//就一直上溯，直至不为右子节点为止
					_parent = node->parent;
				}
				if (node->right != _parent) {		//若此时的右子节点不等于父节点
					node = _parent;					//则父节点即为解答，否则此时的node为解答
				}
			}
		}

		// operator --
		void decrment() {
			if (node->color == RED &&		//如果是红节点，且父节点的父节点等于自己
				node->parent->parent == node)	//表示此时 node 指向 header（即end()）
			{
				node = node->right;				//相当于执行 --end()，right_most 即为解答
			}
			else if (node->left != nullptr) {		//如果有左子节点
				node = node->left;			//令 node 指向其左子节点

				while (node->right != nullptr)		//找到左子节点的最右节点即为解答
					node = node->right;					
			}
			else {			//既非根节点也无左子节点
				base_ptr _parent = node->parent;		//找到 node 的父节点

				while (node == _parent->left) {			//当现行节点身为左子节点
					node = _parent;						//一直往上走，直至不为左子节点
					_parent = node->parent;
				}
				node = _parent;			//此时的父节点即为解答
			}
		}
	};

/* RB_tree 正规迭代器结构 */
	template<class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base {
		typedef Value	value_type;
		typedef Ref		reference;
		typedef Ptr		pointer;	
		typedef __rb_tree_node<Value>*	link_type;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;		//这里是顶层的const，表示不能修改元素的值

		__rb_tree_iterator() : node(nullptr) { }
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator& x) { node = x.node; }

		reference operator*() const { return link_type(node)->value_field; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
		pointer operator->() const { return &(operator*()); }
#endif
		/* 前置++，返回自加后的this */
		self& operator++() { incrment(); return *this; }
		/* 后置++，返回自加前的this */
		self operator++(int) {
			self tmp = *this;
			incrment();
			return tmp;
		}
		/* 前置--，返回自减后的this */
		self& operator--() { decrment(); return *this; }
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




/* RB_tree 结构的定义 */
	/* Key:			rb_tree键值型别 */
	/* Value:		rb_tree实际存储的元素型别，通过对应键值的出排列顺序 */
	/* KeyOfValue:	键值提取方法，得出某个元素实值所对应的键值 */
	/* Compare:		键值比较法则 */
	template<class Key, class Value, class KeyOfValue, class Compare = std::less<Key>, class Alloc = alloc>
	struct rb_tree {
	protected:
		typedef void* void_pointer;
		typedef __rb_tree_node_base*	base_ptr;
		typedef __rb_tree_color_type	color_type;
		typedef __rb_tree_node<Value>	rb_tree_node;

		/* 定义专属空间配置器，每次配置一个节点的空间 */
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	public:
		typedef Key					key_type;
		typedef Value				value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef rb_tree_node*		link_type;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::iterator iterator;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::const_iterator const_iterator;
	protected:
		link_type get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		/* 生成一个节点 */
		link_type create_node(const value_type& x) {
			link_type tmp = get_node();
			try {
				construct(&(tmp->value_field), x);
			}
			catch (...) {
				put_node(tmp);
			}
			return tmp;
		}
		/* 复制一个节点（颜色和值） */
		link_type clone_node(const link_type& x) {
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = nullptr;
			tmp->right = nullptr;
			return tmp;
		}
		/* 销毁一个节点 */
		void destroy_node(link_type x) {
			destroy(&x->value_field);
			put_node(x);
		}

	protected:
		/* RB_tree 只以三笔数据表现 */
		link_type header;			//头指针，其parent域和root互相指向对方，其left、right分别指向最小、最大节点
		size_type node_count;		//节点数量
		Compare key_compare;		//节点键值大小比较准则

		/* 以下三个函数用来方便取得 header 的成员 */
		link_type& root() const { return (link_type&) header->parent; }
		link_type& leftmost() const { return (link_type&) header->left; }
		link_type& rightmost() const { return (link_type&) header->right; }

		/* 以下六个函数方便用来取得节点 x 的成员 */
		static link_type& left(link_type x) { return (link_type&) (x->left); }
		static link_type& right(link_type x) { return (link_type&) (x->right); }
		static link_type& parent(link_type x) { return (link_type&) (x->parent); }
		static reference value(link_type x) { return x->value_field; }
		static const Key& key(link_type x) { return KeyOfValue() (value(x)); }
		static color_type& color(link_type x) { return (color_type&) (x->color); }

		/* 以下六个函数用来方便取得节点 x 的成员 */
		static link_type& left(base_ptr x) { return (link_type&) (x->left); }
		static link_type& right(base_ptr x) { return (link_type&) (x->right); }
		static link_type& parent(base_ptr x) { return (link_type&) (x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value_field; }
		static const Key& key(base_ptr x) { return KeyOfValue() (value(link_type(x))); }
		static color_type& color(base_ptr x) { return (color_type&) (link_type(x)->color); }

		//求取树 x 的最大值和最小值
		static link_type get_min_node(link_type x) {
			return (link_type)__rb_tree_node_base::get_min_node(x);
		}
		static link_type get_max_node(link_type x) {
			return (link_type)__rb_tree_node_base::get_max_node(x);
		}
	private:
		/* 真正的插入程序: x为新值插入点，y为插入点的父节点，v为待插入值。返回指向新插入节点的迭代器 */
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		/* 复制整棵树 x */
		link_type __copy(link_type x);
		/* 删除节点 x，并进行相应的调整 */
		void __erase(link_type x);

		void init() {
			header = get_node();	//产生一个节点，令 header 指向它
			color(header) = RED;	//令header为红色，用来区分header和root，在iterator.operator--中使用
			root() = nullptr;		//root 为空，即 header->parent
			leftmost() = header;	//令 header 的左右子节点为自己
			rightmost() = header;		
		}
	public:
		rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
			init();
		}
		rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {		//拷贝构造函数
			init();
			*this = x;		//调用 operator=
		}
		~rb_tree() {
			clear();
			put_node(header);
		}

		/* 复制整棵树 x */
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
		operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

		/* 清空 rb_tree(除header) */
		void clear();

		/* 将树 *this 和 x 交换 */
		void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
			link_type tmp = header;
			*(&header) = x.header;
			*(&(x.header)) = tmp;
		}

		/* 寻找RB_tree中是否存在键值为 k 的节点 */
		iterator find(const Key& k);

		/* 统计元素 x 的个数 */
		size_type count(const key_type& x) {
			size_type x_count = 0;
			iterator it = find(x);		//当有多个相同元素时，find 返回第一个元素的迭代器
			if (it != end()) {
				++x_count;
										//向后遍历查看是否还存在元素 x
				for (++it; !key_compare(key(it.node), x) && !key_compare(x, key(it.node)); ++it)
					++x_count;
			}
			return x_count;
		}

		iterator begin() { return leftmost(); }		//返回RB_tree最小节点
		iterator end() { return header; }			//RB_tree的终点为 header
		bool empty() const { return (node_count == 0); }
		size_type size() const { return node_count; }
		Compare key_comp() const { return key_compare; }
		size_type max_size() const { return size_type(-1); }

		/* 插入新值：节点值不允许重复，重复则插入失败 */
		std::pair<iterator, bool> insert_unique(const value_type& v);
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_unique(*first);
		}

		/* 插入新值：节点值允许重复 */
		iterator insert_equal(const value_type& v);

		/* 删除节点 x */
		void erase(link_type x) { __erase(x); }
		void erase(iterator x) { __erase(link_type(x.node)); }
		void erase(const value_type& x) {
			iterator it = find(x);
			if (it != end())
				__erase(link_type(it.node));
		}
		template<class InputIterator>
		void erase(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				erase(first);
		}
	};



	/* 复制整棵树 x */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
		clear();		//先将原始的树清空

		key_compare = x.key_compare;
		header->parent = __copy(x.root());
		if (header->parent != nullptr) {
			root()->parent = header;
			node_count = x.node_count;

			leftmost() = get_min_node(root());
			rightmost() = get_max_node(root());
		}

		return *this;
	}


	/* 清空 rb_tree(除header) */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
	{
		if (header->parent != nullptr) {
			queue<base_ptr> node_queue;

			node_queue.push(header->parent);
			while (!node_queue.empty()) {
				base_ptr cur = node_queue.front(); node_queue.pop();
				
				if (cur->left != nullptr)
					node_queue.push(cur->left);
				if (cur->right != nullptr)
					node_queue.push(cur->right);
				destroy_node(link_type(cur));
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
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
		link_type y = header;
		link_type x = root();

	//注意：当存在相同元素时，find 返回第一个元素的迭代器（递增迭代器可访问其他相同元素）

		/* 以下写法比较费事，无论 k 处于哪个位置都必须走到叶节点才停止。因为C++标准中 */
		/* map 只要求 key 必须定义小于关系而没有要求定义等于关系，所有只能这样写 */
		while (x != nullptr) {
			if (key_compare(key(x), k))
				x = right(x);			//运行到这里表示 x 键值小于 k，x 向右走
			else
				y = x, x = left(y);		//运行到这里表示 x 键值大于或等于 k，x 向左走
		}
		iterator j = iterator(y);
		return ( j == end() || key_compare(k, key(j.node)) ) ? end() : j;
	}


	/* 插入新值：节点值不允许重复，重复则插入无效 */
	/* 返回一个pair，第一个元素指向RB_tree中新增节点，第二个元素表示是否插入成功 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;

		while (x != nullptr) {		//从根节点开始，向下寻找适当的插入点
			y = x;

			comp = key_compare(KeyOfValue()(v), key(x));	//less
			x = comp ? left(x) : right(x);
		}//结束后，x指向插入点，y指向x的父节点

		iterator j = iterator(y);
		if (comp) {		//如果结束时 comp 为真，表示将插入左侧
			if (j == begin())	//如果插入节点的父节点为 left_most，则直接插入
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			else		//否则将调整 j 用于后面测试
				--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			//新键值不与既有键值重复，进行插入操作
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

		link_type y = header;
		link_type x = root();		//从根节点开始往下寻找适当的插入点
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
		link_type x = (link_type)_x;
		link_type y = (link_type)_y;

		link_type new_node = create_node(v);			//产生一个新节点
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
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x) {
		if (x == nullptr)
			return nullptr;

		link_type x_copy = clone_node(x);
		x_copy->left = __copy(left(x));
		x_copy->right = __copy(right(x));

		if (x_copy->left != nullptr)
			x_copy->left->parent = x_copy;
		if (x_copy->right != nullptr)
			x_copy->right->parent = x_copy;

		return x_copy;
	}

	/* 以下写法是自己写的简单的方法，但效率很低，有待改进 */

	/* 删除节点 x，并进行相应的调整 */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x)
	{
		if (x->left == nullptr && x->right == nullptr)			//如果 x 已经是叶节点，就将 x 删除，并进行调整
		{
			if (x == x->parent->left)
				x->parent->left = nullptr;
			else
				x->parent->right = nullptr;

			if (x->color == RED) {		//如果 x 为红色的叶节点，则直接将其删除即可
				destroy_node(x);
			}
			else {			//否则需要进行相应调整
				
				//。。。。。。还是有点难

			}
			--node_count;
		}
		else if (x->left != nullptr && x->right == nullptr) {		//x 只有左子树，找到 x 的前驱替换 x，转换为删除x的前驱
			link_type prev = x->left;
			x->value_field = prev->value_field;
			__erase(prev);
		}
		else if (x->left == nullptr && x->right != nullptr) {		//x 只有右子树，找到 x 的后继替换 x，转换为删除x的后继
			link_type next = x->right;
			x->value_field = next->value_field;
			__erase(next);
		}
		else {			//x 拥有左右子树，找到 x 的后继替换 x，转换为删除x的后继
			link_type next = x->right;
			x->value_field = next->value_field;
			__erase(next);
		}
	}




/* 全局函数：调整树形使其平衡（改变颜色及旋转树形） */
/* 参数一为新增节点，参数二为 root */
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = RED;		//新节点必为红（根据规则4）

		while (x != root && x->parent->color == RED)	//父节点也为红色
		{
			if (x->parent == x->parent->parent->left) {			//父节点为祖父节点的左子节点
				__rb_tree_node_base* y = x->parent->parent->right;	//令 y 为伯父节点
				if (y && y->color == RED) {			//伯父节点存在且为红色
					y->color = BLACK;				//更改伯父节点为黑
					x->parent->color = BLACK;		//更改父节点为黑
					x->parent->parent->color = RED;	//更改祖父节点为红

					/* x 上升到祖父节点，下次继续判断祖父节点和太祖父节点是否同时为红 */
					x = x->parent->parent;
				}
				else {			//无伯父节点，或伯父节点为黑
					if (x == x->parent->right) {		//如果新节点为父节点的右子节点
						x = x->parent;					//修改 x 为其父节点，先对其父节点进行一次左旋操作
						__rb_tree_rotate_left(x, root);		//第一参数为左旋点
					}
					x->parent->color = BLACK;			//将父节点改为黑
					x->parent->parent->color = RED;		//将祖父节点改为红
					__rb_tree_rotate_right(x->parent->parent, root);	//对祖父节点执行右旋

					/* 此时x和兄弟节点(原祖父节点)为红，x父节点为黑，已满足rb_tree要求，下次循环将退出while */
				}
			}
			else {			//父节点为祖父节点的右子节点
				__rb_tree_node_base* y = x->parent->parent->left;		//令 y 为伯父节点
				if (y && y->color == RED) {			//伯父节点存在且为红
					y->color = BLACK;				//更改伯父节点为黑
					x->parent->color = BLACK;		//更改父节点为黑
					x->parent->parent->color = RED;	//更改祖父节点为红

					/* x 上升到祖父节点，下次继续判断祖父节点和太祖父节点是否同时为红 */
					x = x->parent->parent;
				}
				else {		//伯父节点不存在或伯父节点为黑
					if (x == x->parent->left) {			//如果新节点为父节点的左子节点
						x = x->parent;					//修改 x 为其父节点，先对其父节点进行一次右旋操作
						__rb_tree_rotate_right(x, root);	//第一参数为右旋点
					}
					x->parent->color = BLACK;			//将父节点改为黑
					x->parent->parent->color = RED;		//将祖父节点改为红
					__rb_tree_rotate_left(x->parent->parent, root);		//对祖父节点执行左旋

					/* 此时x和兄弟节点(原祖父节点)为红，x父节点为黑，已满足rb_tree要求，下次循环将退出while */
				}
			}
		}		//while 结束
		root->color = BLACK;		//根节点永远为黑
	}

/* 全局函数：x为左旋点，root为根节点 */
/* 新节点必为红节点，如果插入点的父节点也为红节点，则可能需要进行树形旋转及颜色改变 */
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->right;		//令 y 为旋转点的右子节点
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;		//别忘了设定父节点
		}
		y->parent = x->parent;

		//令 y 完全顶替 x 的地位（必须将 x 对其父节点的关系完全接收过来）
		if (x == root)						//如果 x 为根节点，将 y 设为新的根节点
			root = y;
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
		__rb_tree_node_base* y = x->left;		//令 y 为旋转点的左子节点
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;		//别忘了设定父节点
		}
		y->parent = x->parent;

		//令 y 完全顶替 x 的地位（必须将 x 对其父节点的关系完全接收过来）
		if (x == root)						//如果 x 为根节点，将 y 设为新的根节点
			root = y;
		else if (x == x->parent->right)		//x 为其父节点的右子节点
			x->parent->right = y;
		else								//x 为其父节点的左子节点
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}


__mySTL_END__

#endif	//end of __RB_TREE_H__