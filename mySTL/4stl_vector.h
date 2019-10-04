/*
 *vector实现：
 *
 *这里缺省使用内部空间配置器alloc（详见1stl_alloctor.h），使用者也可以自行传入空间配置器
 */

#ifndef _STL_VECTOR_H_
#define _STL_VECTOR_H_

#include "1stl_alloctor.h"		//空间配置器
#include "1stl_construct.h"		//对象构造与析构
#include "1stl_iterator.h"		//for function distance
#include "1stl_uninitialized.h"	//for function uninitialized_*
#include "2stl_algobase.h"		//for diatance
#include <initializer_list>		//for 列表初始化
#include <cstddef>				//for ptrdiff_t

namespace lfp {

	//省缺使用第二级空间配置器
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vector的嵌套型别定义
		/* 因为vector是是在连续空间内存放，所以原生指针就可作为其迭代器 */
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	private:
		//专属的空间配置器，配置时不是以字节为单位，而是以元素个数为单位
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//目前使用空间的头
		iterator finish;			//目前使用空间的尾
		iterator end_of_storage;	//目前可用空间的尾

		//内部函数，调用构造器释放vector占用的所有空间
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		//配置n个元素的空间并填充内容
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		//内调函数，在pos处插入n个元素x，若空间不足进行扩容
		void insert_aux(iterator pos, size_type n, const T& x);

	public:		//vector的6种初始化方式
		vector() : start(0), finish(0), end_of_storage(0) { }	//1 默认构造
		explicit vector(size_type n) { fill_initialize(n, T()); }	//2 指定大小
		vector(int n, const T& value) { fill_initialize(n, value); }	//3	创建n个value
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		template<class Iterator>
		vector(Iterator first, Iterator last) {		//4 用区间[first, last)初始化
			size_type n = distance(first, last);
			start = data_allocator::allocate(n);
			uninitialized_copy(first, last, start);
			finish = start + n;
			end_of_storage = finish;
		}
		vector(const vector<T>& rhs) {		//5 拷贝构造
			start = data_allocator::allocate(rhs.size());
			uninitialized_copy(rhs.start, rhs.finish, start);
			finish = start + rhs.size();
			end_of_storage = finish;
		}
		vector(std::initializer_list<T> init_list) {		//6 列表初始化
			start = data_allocator::allocate(init_list.size());
			uninitialized_copy(init_list.begin(), init_list.end(), start);
			finish = start + init_list.size();
			end_of_storage = finish;
		}
		~vector() {
			if (start)
				destroy(start, finish);		//将对象析构
			deallocate();			//释放内存
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return finish - start; }
		size_type capacity() const { return end_of_storage - start; }
		reference operator[](size_type n) { return *(start + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		/* 在pos位置插入n个元素x */
		void insert(iterator pos, size_type n, const T& x) {
			insert_aux(pos, n, x);
		}
		/* 在pos位置插入一个元素x */
		void insert(iterator pos, const T& x) {
			insert_aux(pos, 1, x);
		}

		void push_front(const T& x) {
			insert_aux(begin(), 1, x);
		}

		void push_back(const T& x) {
			if (finish != end_of_storage) {	  //如果未填满则直接在末尾构造元素
				construct(finish, x);
				++finish;
			}
			else {		//否则需要扩容
				insert_aux(end(), 1, x);
			}
		}

		void pop_front() {
			erase(begin());
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		iterator erase(iterator pos) {		
			if (pos + 1 != end()) {		//如果不是删除最后一个元素，需要将后续元素向前移动
				copy(pos + 1, finish, pos);
			}
			--finish;
			destroy(finish);
			return pos;
		}

		//删除[first, last)中所有元素
		iterator erase(iterator first, iterator last) {
			//将[last, finish)的内容移动到从first开始位置，返回最后复制的下一位置
			iterator it = copy(last, finish, first);

			/* 销毁[it, finish) 内的所有内容 */
			destroy(it, finish);
			finish = it;		//更新 finish 指向新的结尾
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert_aux(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, T());
		}

		void swap(vector<T>& rhs) {
			swap(start, rhs.start);
			swap(end, rhs.end);
			swap(end_of_storage, rhs.end_of_storage);
		}

		void clear() {
			erase(begin(), end());
		}

	protected:
		//配置n个元素空间，并将其填充为value
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);		//申请n个元素的空间
			uninitialized_fill_n(result, n, value);		//填充元素
			return result;
		}
	};	//end of class vector



	/* 内调函数，在pos处插入元素 x，若空间不足进行扩容 */
	//template<class T, class Alloc>
	//void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
	//	if (finish != end_of_storage) {		//还有备用空间
	//		//在备用空间起始(finish)处构造一个元素，并以vector最后一个元素(finish - 1)为初始值
	//		construct(finish, *(finish - 1));
	//		++finish;		//调整水位
	//		T x_copy = x;

	//		//将 [pos, finish - 2) 内的元素向后移动一格
	//		copy_backward(pos, finish - 2, finish - 1);		//交由高阶STL算法处理，实现见 2stl_algobase.h
	//		*pos = x_copy;		//在 pos 处插入新元素
	//	}
	//	else {		//没有备用空间
	//		const size_type old_capacity = size();
	//		const size_type new_capacity = (old_capacity == 0) ? 8 : 2 * old_capacity;
	//		/* 以上为内存空间配置原则: 如果原大小为0，则配置8，否则配置原空间大小的 2 倍 */
	//		/* 前半段用来放置原数据，后半段用来放置新数据 */

	//		iterator new_start = data_allocator::allocate(new_capacity);
	//		iterator new_finish = new_start;
	//		try {
	//			/* 先将原vector中 [start, pos) 的内容拷贝到 new_start 开始的位置 */
	//			new_finish = uninitialized_copy(start, pos, new_start);
	//			
	//			/* 在 new_finish(即pos) 处构造插入的元素 x */
	//			construct(new_finish, x);
	//			++new_finish;		//调整水位

	//			/* 再将原vector中 [pos, finish) 的内容拷贝到 new_finish 开始的位置 */
	//			new_finish = uninitialized_copy(pos, finish, new_finish);

	//			//元素插入完成，新 vector 构造完成
	//		} catch (...) {
	//			//捕获到异常，执行相应的销毁工作。注意：因为uninitialized_copy遵循commit or rollback
	//			//原则，所以构造失败时不需要自己执行对象销毁工作，只需要释放申请的内存就好
	//			data_allocator::deallocate(new_start, new_capacity);		//释放空间
	//			throw;		//注意要向高层抛出异常
	//		}

	//		//析构并释放原 vector
	//		destroy(begin(), end());
	//		deallocate();

	//		//调整迭代器，指向新的 vector
	//		start = new_start;
	//		finish = new_finish;
	//		end_of_storage = new_start + new_capacity;
	//	}
	//}


	//内调函数，在pos处插入n个元素x，若空间不足进行扩容
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& x) {
		if (n > 0)
		{
			if (size_type(end_of_storage - finish) >= n) {
				//备用空间大于等于新增元素个数
				T x_copy = x;
				
				//以下计算插入点之后的现有元素个数
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after >= n) {		//插入点之后的元素个数大于等于新增元素个数
					uninitialized_copy(finish - n, finish, finish);		//在备用空间开始出再构造 n 个元素
					finish += n;		//将vector尾端标记后移
					copy_backward(pos, old_finish - n, old_finish);		//向后拷贝剩余元素，实现见 2stl_algobase.h
					//从插入点开始填入新值
					fill(pos, pos + n, x_copy);
				}
				else {		//插入点之后的元素个数小于新增元素个数
					uninitialized_fill_n(finish, n - elems_after, x_copy);	//现在finish之后构造n-elems_after个x_copy（因为插入之后这里就应该是x_copy）
					finish = finish + n - elems_after;
					uninitialized_copy(pos, old_finish, finish);	//将原本pos后的元素拷贝到插入之后正确的位置
					finish += elems_after;
					fill(pos, old_finish, x_copy);
				}
			}
			else {		//备用空间小于新增元素个数，必须配置额外的内存
				//首先决定新长度：旧长度的两倍或新插入元素长度的两倍
				const size_type old_capacity = capacity();
				const size_type new_capacity = 2 * max(old_capacity, n);

				//以下配置新的 vector 空间
				iterator new_start = data_allocator::allocate(new_capacity);
				iterator new_finish = new_start;
				try{
					//以下首先将旧 vector 中插入点之前的元素复制到新空间中
					new_finish = uninitialized_copy(start, pos, new_start);
					//再将新增元素 n 填入新空间
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//最后将旧 vector 中插入点之后的元素移动到新空间
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					//捕获到异常，执行相应的销毁工作。注意：因为uninitialized_copy、uninitialized_fill_n都遵循
					//commit or rollback原则，所以构造失败时不需要自己执行对象销毁工作，只需要释放申请的内存就好
					data_allocator::deallocate(new_start, new_capacity);
					throw;		//注意要向高层抛出异常
				}

				//析构并释放原 vector
				destroy(begin(), end());
				deallocate();

				//调整迭代器，指向新的 vector
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_capacity;
			}
		}
	}

}// end of namespace lfp

#endif // !_STL_VECTOR_H_