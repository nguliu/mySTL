#pragma once
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "alloctor.h"		//空间配置与管理
#include "construct.h"		//内存对象的构造、析构与重构等
#include "algobase.h"		//基本算法相关

//#include <initializer_list>


__mySTL_BEGIN__

	//省缺使用第二级空间配置器
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vector的嵌套型别定义
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	private:
		//simple_alloc是SGI STL的空间配置器
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//目前使用空间的头
		iterator finish;			//目前使用空间的尾
		iterator end_of_storage;	//目前可用空间的尾

		//insert内调函数，在position处插入元素 x，若空间不足进行空间扩充
		void insert_aux(iterator position, const T& x);
		//内部函数，调用构造器释放vector占用的所有空间
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		//ROUND_UP()将byte上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}
		//配置内存空间并填充内容
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			
			//end_of_storage = finish;		//错误写法

			/* 以下写法才正确，实际得到的空间大小为: n * sizeof(value_type)向上取 8 的整数倍字节数 */
			end_of_storage = start + ROUND_UP(n * sizeof(value_type)) / sizeof(value_type);
		}

	public:
		vector() : start(0), finish(0), end_of_storage(0) { }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		vector(const vector<T>& x) {				//拷贝构造函数
			fill_initialize(x.size(), T());
			for (int i = 0; i < size(); ++i)
				*(start + i) = *(x.start + i);
		}
		/* 列表初始化 */
		vector(std::initializer_list<T> init_list) {
			fill_initialize(init_list.size(), T());
			auto beg = init_list.begin();
			for (int i = 0; i < size(); ++i)
				*(start + i) = *(beg + i);
		}
		~vector() {
			destroy(start, finish);		//将对象析构
			deallocate();			//释放内存
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end_of_storage - start); }
		reference operator[](size_type n) { return *(begin() + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		/* 在position位置之前，插入 n 个元素 x  */
		void insert(iterator position, size_type n, const T& x);
		/* 在position位置之前，插入元素 x  */
		void insert(iterator position, const T& x) {
			insert(position, 1, x);
		}

		//将元素插入最尾端
		void push_back(const T& x) {
			if (finish != end_of_storage) {
				construct(finish, x);		//全局函数，在尾段构造一个元素
				++finish;
			}
			else {
				insert_aux(end(), x);		//扩充空间并插入元素
			}
		}

		//弹出尾端元素
		void pop_back() {
			--finish;
			destroy(finish);
		}

		//删除位置 position 上的元素
		iterator erase(iterator position) {		
			if (position + 1 != end()) {		//如果不是删除最后一个元素
				copy(position + 1, finish, position);	//后续元素向前移动。交由高阶STL算法处理，实现见 algobase.h
			}
			--finish;
			destroy(finish);
			return position;
		}

		//删除[first, last)中所有元素
		iterator erase(iterator first, iterator last) {
			//将[last, finish)的内容移动到从first开始位置，返回最后复制的下一位置
			iterator i = copy(last, finish, first);

			/* 销毁 [i, finish) 内的所有内容 */
			destroy(i, finish);
			finish = i;		//更新 finish 指向新的结尾
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, T());
		}

		void swap(vector<T, Alloc>& vec) {

		}

		void clear() {
			destroy(begin(), end());
		}

	protected:
		//配置 n 个空间，并将其用 value 填满
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}
	};	//end of class vector


	/* insert内调函数，在position处插入元素 x，若空间不足进行空间扩充 */
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {		//还有备用空间
			//在备用空间起始(finish)处构造一个元素，并以vector最后一个元素(finish - 1)为初始值
			construct(finish, *(finish - 1));
			++finish;		//调整水位
			T x_copy = x;

			//将 [position, finish - 2) 内的元素向后移动一格
			copy_backward(position, finish - 2, finish - 1);	//交由高阶STL算法处理，实现见 algobase.h
			*position = x_copy;		//在 position 处插入新元素
		}
		else {		//没有备用空间
			const size_type old_size = size();
			const size_type new_size = (old_size == 0) ? 10 : 2 * old_size;
			/* 以上为内存空间配置原则: 如果原大小为0，则配置10，否则配置原空间大小的 2 倍 */
			/* 前半段用来放置原数据，后半段用来放置新数据 */

			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try {
				/* 先将原vector中 [start, position) 的内容拷贝到 new_start 开始的位置 */
				new_finish = uninitialized_copy(start, position, new_start);
				
				/* 在 new_finish(即position) 处构造插入的元素 x */
				construct(new_finish, x);
				++new_finish;		//调整水位

				/* 再将原vector中 [position, finish) 的内容拷贝到 new_finish 开始的位置 */
				new_finish = uninitialized_copy(position, finish, new_finish);

				//元素插入完成，新 vector 构造完成
			} catch (...) {
				//捕获到异常，进行相应的销毁工作
				destroy(new_start, new_finish);		//析构新空间上的对象
				data_allocator::deallocate(new_start, new_size);		//释放空间
				throw;
			}

			//析构并释放原 vector
			destroy(begin(), end());
			deallocate();

			//调整迭代器，指向新的 vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
	}


	/* 从position位置开始，插入 n 个元素 x  */
	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
		if (n > 0) {
			if (size_type(end_of_storage - finish) >= n) {
				//备用空间大于等于新增元素个数
				T x_copy = x;
				
				//以下计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//插入点之后的元素个数大于新增元素个数
					uninitialized_copy(finish - n, finish, finish);		//在备用空间开始出再构造 n 个元素
					finish += n;		//将vector尾端标记后移
					copy_backward(position, old_finish - n, old_finish);	//交由高阶STL算法处理，实现见 algobase.h
					//从插入点开始填入新值
					fill(position, position + n, x_copy);
				}
				else {
					//插入点之后的元素个数小于等于新增元素个数
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish = finish + n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else {		//备用空间小于新增元素个数，必须配置额外的内存
				//首先决定新长度：旧长度的两倍或新插入元素长度的两倍
				const size_type old_size = size();
				const size_type new_size = (old_size == 0) ? n : 2 * max(old_size, n);

				//以下配置新的 vector 空间
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;
				try{
					//以下首先将旧 vector 中插入点之前的元素复制到新空间中
					new_finish = uninitialized_copy(start, position, new_start);
					//再将新增元素 n 填入新空间
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//最后将旧 vector 中插入点之后的元素移动到新空间
					new_finish = uninitialized_copy(position, finish, new_finish);
				} catch (...) {
					//发生异常，执行相应的销毁工作
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, new_size);
					throw;
				}

				//以下清除并释放旧 vector
				destroy(start, finish);
				deallocate();

				//以下调整水位标记
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
			}
		}
	}

__mySTL_END__

#endif	//end of __VECTOR_H__