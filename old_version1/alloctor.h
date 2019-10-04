/*
 *空间配置头文件：
 *
 *负责内存空间的配置与管理
 *实现了第一级空间配置器、第二级空间配置器，维护自由链表和内存池等
 *
 *默认使用第二级空间配置器(在末尾定义)
 */


#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "config.h"		//相关配置头文件


__mySTL_BEGIN__

/*	SGI封装的 alloc 配置器接口	*/
	template<typename T, class Alloc>
	class simple_alloc {
	public:
		static T* allocate(size_t n) {
			return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}
		static T* allocate(void) {
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T* p, size_t n) {
			if (0 != n)
				Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T* p) {
			Alloc::deallocate(p, sizeof(T));
		}
	};		//end of class simple_alloc


#if 0
	#include <new>
	#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
	#include <iostream>
	#define __THROW_BAD_ALLOC std::cerr << "out of memory\n"; exit(1);
#endif


/*	第一级配置器 __malloc_alloc_template	*/
/*	以malloc() realloc() free() 等C函数执行实际的内存配置与释放操作		*/
	//注意，无template型别参数，至于非型别参数inst，则完全没有派上用场
	template<int inst>
	class __malloc_alloc_template {
	private:
		/* 以下函数将用来处理内存不足的情况 */
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*, size_t);
		static void(*__malloc_alloc_oom_handler)();

	public:
		static void* allocate(size_t n) {
			void* result = malloc(n);		//第一级配置器直接用 malloc，也可用 operator new
			if (0 == result)
				result = oom_malloc(n);		//无法满足需求时，改用 oom_malloc 
			return result;
		}

		static void deallocate(void* p, size_t n) {
			free(p);		//第一级配置器直接用 free，也可用 operator delete
		}

		static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
			void* result = realloc(p, new_sz);		//第一级配置器直接用realloc
			if (0 == result)
				result = oom_realloc(p, new_sz);
			return result;
		}

		/*以下仿真C++的set_new_handler()，你可以通过它指定你自己的 out-of-memory-handler*/
		static void(*set_malloc_handler(void(*f)()))() {
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return (old);
		}
	};	//end of class __malloc_alloc_template

	//设定malloc_alloc out-of-memory handling初值为0，有待客端设定
	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
		void (*my_malloc_handler)();
		void* result;
		while (1) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)		//如果未定义处理例程
				__THROW_BAD_ALLOC;
			(*my_malloc_handler)();		//调用处理例程，企图释放内存
			result = malloc(n);			//再次尝试配置内存
			if (result)
				return (result);
		}
	}

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
		void (*my_malloc_handler)();
		void* result;
		while (1) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)		//如果未定义处理例程
				__THROW_BAD_ALLOC;
			(*my_malloc_handler)();		//调用处理例程，企图释放内存
			result = realloc(p, n);		//再次尝试配置内存
			if (result)
				return result;
		}
	}
	//以下直接将参数inst指定为0
	typedef __malloc_alloc_template<0> malloc_alloc;


/*	第二级配置器 __default_alloc_template	*/
/*	注意，无template型别参数，第一参数用于多线程环境，非型别参数inst完全没有派上用场 */
	enum { __ALIGN = 8 };		// 小型区块的上调边界
	enum { __MAX_BYTES = 128 };		// 小型区块的上限
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };		// 自由链表的个数

	template<bool threads, int inst>
	class __default_alloc_template {
	private:
		//ROUND_UP()将byte上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}
	private:
		//自由链表节点结构
		union obj {
			union obj* free_list_link;
			char client_data[1];		//The client see this
		};

		// 16个free list，初始值设置为0
		static obj* volatile free_list[__NFREELISTS];

		//以下函数根据区块大小决定使用第 n 号free-list，n 从0算起
		static size_t FREELIST_INDEX(size_t bytes) {
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}

	private:
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到 free-list
		static void* refill(size_t n);
		//配置一大块空间，可容纳 nobjs 个大小为 size 的区块
		//若配置 nobjs 个区块不便，nobjs 可能会降低
		static char* chunk_alloc(size_t size, int &nobjs);

		// Chunk allocation state
		static char*  start_free;		//内存池起始位置，只在chunk_alloc()中变化
		static char*  end_free;			//内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size;

	public:
		/* 空间配置函数 */
		static void* allocate(size_t n);
		/* 空间释放函数 */
		static void deallocate(void* p, size_t n);
		/* 空间重新配置函数 */
		static void* reallocate(void* p, size_t old_sz, size_t new_sz);

	};	// end of __default_alloc_template

	/* 以下是static data member 的定义与初值设定 */
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;
	template<bool threads, int inst>
	typename __default_alloc_template<threads, inst>::obj * volatile
	__default_alloc_template<threads, inst>::free_list[__NFREELISTS] =
	{ 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

	/* 空间配置函数 */
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::allocate(size_t total_byte) {
		obj* volatile* my_free_list;
		obj* result;

		//大于128就调用第一级配置器
		if (total_byte > (size_t)__MAX_BYTES)
			return malloc_alloc::allocate(total_byte);

		//否则寻找16个free lis中最适当的一个
		my_free_list = free_list + FREELIST_INDEX(total_byte);
		result = *my_free_list;
		if (result == 0) {		//没找到可用的free list，将 n 上调至 8 的倍数，准备重新填充free list
			void* r = refill(ROUND_UP(total_byte));
			return r;
		}
		//调整free list，取出一个区块
		*my_free_list = result->free_list_link;
		return result;
	}

	/* 空间释放函数, p不能为0 */
	template<bool threads, int inst>
	void __default_alloc_template<threads, inst>::deallocate(void* p, size_t n) {
		obj* volatile* my_free_list;
		obj* q = (obj *)p;

		//大于128就调用第一级配置器
		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}

		//否则寻找对应的free list，将区块归还
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

/* 重新填充free list，返回一个大小为 size 的内存块给客端使用，其余纳入自由链表 */
/* size 已经增加至 8 的倍数，如果得到不止一个内存块则将其他内存块链接掉到 free list 上 */
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::refill(size_t size) {
		int nobjs = 10;			//一次获取 10 个大小为 size 的内存块
		obj *volatile *my_free_list;
		obj *result, *next_obj;

		//调用chunk_alloc()尝试获得 nonjs 个区块作为free-list的新节点
		//注意参数 nobjs 是以引用方式传入，便于在chunk_alloc中修改 nobjs 为实际得到的区块个数
		char* chunk = chunk_alloc(size, nobjs);

		//如果只获得一个区，将这个区块分配给调用者，free list无区块加入
		if (1 == nobjs)
			return (chunk);
		//准备调整free list，纳入新节点
		my_free_list = free_list + FREELIST_INDEX(size);

		//以下在chunk空间内建立free list
		result = (obj *)chunk;		//这一块返回给客端

		//以下将在内存池中新配置的空间纳入free-list
		*my_free_list = next_obj = (obj*)(chunk + size);	//从第二个区块开始
		
		for (int i = 1; ; ++i) {		//从1开始，因为0已经返回给客端
			obj* current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + size);	//指向下一个区块
			if (nobjs - 1 == i) {
				current_obj->free_list_link = 0;		//链接完成
				break;
			}
			else
				current_obj->free_list_link = next_obj;
		}
		return (result);
	}

/* 从内存池中取内存填充free list，size表示一个区块的大小，已经增加至8的倍数 */
/* nobjs表示希望获得的区块个数，以引用方式传递为了在函数中将其修改为实际得到的区块个数 */
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
		char* result;
		size_t total_bytes = size * nobjs;			//请求取得的总空间
		size_t bytes_left = end_free - start_free;	//内存池剩余空间

		if (bytes_left >= total_bytes) {		//内存池空间完全满足需求量
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else if (bytes_left >= size) {		//内存池剩余空间不能完全满足需求，但可以满足至少一个区块
			nobjs = (int)(bytes_left / size);	//将nobjs修改为实际得到的内存块个数
			total_bytes = size * nobjs;			//计算实际获得的总空间
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {		//内存剩余区域一个区块都无法满足
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//以下试着让内存池中剩余的零头还有利用价值
			if (bytes_left > 0) {
				//寻找适当的free list，然后配备给free list
				obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);

				//调整free list，将内存池中剩余的空间纳入
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj *)start_free;
			}

			/* 配置heap的空间，用来补充内存池 */
			start_free = (char *)malloc(bytes_to_get);
			if (0 == start_free) {		//heap空间不足，malloc失败
				obj * volatile * my_free_list, *p;
				//以下搜索适当的free list，即“尚有未用区块，且区块足够大”的free list
				for (int i = __MAX_BYTES; i > 0; i -= __ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {		//free list内尚有未用区块
						//调整free list以释放未用区块
						*my_free_list = p->free_list_link;
						start_free = (char *)p;
						end_free = start_free + i;

						//递归调用自己，为了修正nobjs
						return (chunk_alloc(size, nobjs));
						//任何残余零头都将被编入free list中备用
					}
				}
				end_free = 0;		//如果到处都没有内存可用了
				//调用第一级配置器，看看out of memory机制能否解决
				//这回抛出异常，或内存不足的情况获得改善
				start_free = (char *)malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归调用自己，为了修正nobjs
			return (chunk_alloc(size, nobjs));
		}
	}

	#define __NODE_ALLOCATER_THREADS false		//非多线程环境

//直接将第二级配置器设置为配置器
	typedef __default_alloc_template<__NODE_ALLOCATER_THREADS, 0> alloc;

__mySTL_END__

#endif