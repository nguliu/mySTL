/*
 *空间配置器实现：
 *
 *负责内存空间的配置与管理
 *实现了第一级空间配置器、第二级空间配置器，维护自由链表和内存池等
 *
 *默认使用第二级空间配置器(在末尾定义)
 */

#ifndef _STL_ALLOCATOR_H_
#define _STL_ALLOCATOR_H_

#include <stdlib.h>		//malloc realloc free

#if 0
	#include <new>
	#define __THROW_BAD_ALLOC throw std::exception("bad_alloc")
#elif !defined(__THROW_BAD_ALLOC)
	#include <iostream>
	#define __THROW_BAD_ALLOC std::cerr << "out of memory\n"; exit(1);
#endif

namespace lfp {			//第一级namespace是提供给外部用户使用的（后同）
namespace detail {		//第二级namespace是stl内部使用的实现细节（后同）

	//第一级配置器 __malloc_alloc_template
	//以malloc() realloc() free() 等C函数执行实际的内存配置与释放操作
	/* 无template型别参数，非型别参数inst也并没有使用 */
	template<int inst>
	class __malloc_alloc_template {
	private:
		/* 以下函数将用来处理内存不足的情况 */
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*, size_t);
		static void(*__malloc_alloc_oom_handler)();  //用于仿照C++ new handler机制

	public:
		static void* allocate(size_t n) {
			void* result = ::malloc(n);			//第一级配置器直接用malloc
			//void* result = ::operator new(n);	//作用同上
			if (nullptr == result)
				result = oom_malloc(n);			//无法满足需求时，改用oom_malloc 
			return result;
		}

		static void deallocate(void* p, size_t n) {
			::free(p);				//第一级配置器直接用 free
			//::operator delete(p);	//作用同上
		}

		static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
			void* result = ::realloc(p, new_sz);	//第一级配置器直接用realloc
			if (nullptr == result)
				result = oom_realloc(p, new_sz);
			return result;
		}

		//以下仿真C++的set_new_handler()，可以通过它指定你自己的out-of-memory-handler
		/* 传入一个函数指针，返回一个函数指针 */
		static void(*set_malloc_handler( void(*f)() ))() {
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return (old);
		}
	};

	//设定malloc_alloc out-of-memory handling初值为0，由待客端设定
	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
		void(*my_malloc_handler)();
		void* result;

		//不断尝试释放、配置、再释放、再配置......
		for (int i = 0; i < 32; ++i) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)	 //如果未定义处理例程则直接退出
				__THROW_BAD_ALLOC;
			(*my_malloc_handler)();		 //否则调用处理例程，企图释放内存
			result = ::malloc(n);		 //再次尝试配置内存
			if (result)
				return (result);
		}

		__THROW_BAD_ALLOC;
	}

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
		void(*my_malloc_handler)();
		void* result;

		//不断尝试释放、配置、再释放、再配置......
		for (int i = 0; i < 32; ++i) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)	 //如果未定义处理例程则直接退出
				__THROW_BAD_ALLOC;
			(*my_malloc_handler)();		 //否则调用处理例程，企图释放内存
			result = ::realloc(p, n);	 //再次尝试配置内存
			if (result)
				return (result);
		}

		__THROW_BAD_ALLOC;
	}

	typedef __malloc_alloc_template<0> malloc_alloc;	//直接将inst指定为0



	enum { __ALIGN = 8 };							// 小型区块的上调边界
	enum { __MAX_BYTES = 128 };						// 小型区块的上限
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };	// 自由链表的个数

	//第二级配置器 __default_alloc_template
	/* 无template型别参数，第一个参数用于多线程环境，非型别参数inst并没有使用 */
	template<bool threads, int inst>
	class __default_alloc_template {
	private:
		//自由链表节点结构
		union obj {
			union obj* free_list_link;
			char client_data[1];		//The client see this
		};

		//16个free list，初始值设置为0
		/* 注意：这里free_lists是一个指针数组，即数组的每个元素都是一个指针 */
		static obj* volatile free_lists[__NFREELISTS];

		//ROUND_UP()将byte上调至不小于byte的8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}

		//FREELIST_INDEX根据区块大小决定使用第n号free-list，n从0算起
		static size_t FREELIST_INDEX(size_t bytes) {
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}

	private:
		//返回一个大小为n的区块，可能导致区块大小为n的free-list被填充
		static void* refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//若配置 nobjs 个区块不便，nobjs可能会降低
		static char* chunk_alloc(size_t size, int &nobjs);

		// Chunk allocation state
		static char*  start_free;		//内存池起始位置，只在chunk_alloc()中变化
		static char*  end_free;			//内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size;

	public:
		/* 空间配置 */
		static void* allocate(size_t);
		/* 空间释放 */
		static void deallocate(void*, size_t);
		/* 重新配置空间大小 */
		static void* reallocate(void*, size_t, size_t);
	};

	/* static data member 的初值设定 */
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	typename __default_alloc_template<threads, inst>::obj*
	volatile __default_alloc_template<threads, inst>::free_lists[__NFREELISTS] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	/* 空间配置函数 */
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::allocate(size_t total_byte) {
		//大于128byte就调用第一级配置器
		if (total_byte > (size_t)__MAX_BYTES) {
			return malloc_alloc::allocate(total_byte);
		}

		//否则寻找16个free list中最适当的一个
		/* 因为free_lists是一个指针数组，同时数组又可以转化为指针使用，所有free_lists相当于指向指针的指针 */
		obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(total_byte);
		obj* result = *pp_free_list;

		//没找到可用的free list，将n上调至8的倍数，准备重新填充free list
		if (0 == result) {
			void* r = refill(ROUND_UP(total_byte));
			return r;
		}
		//调整free list，删除取走的那个区块
		*pp_free_list = result->free_list_link;
		return result;
	}

	/* 空间释放函数, p不能为0 */
	template<bool threads, int inst>
	void __default_alloc_template<threads, inst>::deallocate(void* p, size_t n) {
		//大于128就调用第一级配置器
		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}

		//否则寻找对应的free list，将区块归还
		obj* pObj = (obj*)p;
		obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(n);
		pObj->free_list_link = *pp_free_list;
		*pp_free_list = pObj;
	}

	/* 重新配置空间大小 */
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::reallocate(void* p, size_t old_size, size_t new_size) {
		//直接调用第一级空间配置器
		return malloc_alloc::reallocate(p, old_size, new_size);
	}

	/* 重新填充free list，返回一个大小为 size 的内存块给客端使用，其余纳入自由链表 */
	/* size 已经增加至 8 的倍数，如果得到不止一个内存块则将其他内存块链接掉到 free list 上 */
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::refill(size_t size) {
		int nobjs = 20;		//一次获取 20 个大小为 size 的内存块
		//调用chunk_alloc()尝试获得 nonjs 个区块
		/* 注意nobjs既是传入参数也是传出参数，传出实际获得区块的个数 */
		char* chunk = chunk_alloc(size, nobjs);

		//如果只获得一个区，将这个区块分配给调用者，free list无区块加入
		if (1 == nobjs)
			return (chunk);

		obj* result = (obj *)chunk;	  //第一块返回给客端

		//准备填充free list
		obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(size);
		*pp_free_list = (obj*)(chunk + size);	//从第二个区块开始

		obj* cur_obj = *pp_free_list;
		for (int i = 2; i < nobjs; ++i)		//从2开始，因为第1块已经返回给客端
		{
			obj* next_obj = (obj *)((char *)cur_obj + size);
			cur_obj->free_list_link = next_obj;
			cur_obj = next_obj;
		}
		cur_obj->free_list_link = 0;	//别忘了设置最后一块的指针

		return (result);
	}

	/* 从内存池中取nobjs个内存区块，size表示一个区块的大小，已经增加至8的倍数 */
	/* nobjs表示希望获得的区块个数，既是传入参数也是传出参数，传出实际获得的区块个数 */
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
		char* result;
		size_t total_bytes = size * nobjs;			//请求取得的总空间
		size_t bytes_left = end_free - start_free;	//内存池剩余空间

		if (bytes_left >= total_bytes) {		//内存池空间完全满足需求量
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size) {		//内存池剩余空间不能完全满足需求，但可以满足至少一个区块
			result = start_free;
			nobjs = (int)(bytes_left / size);	//将nobjs修改为实际获得的区块个数
			total_bytes = size * nobjs;			//实际获得的总空间
			start_free += total_bytes;
			return result;
		}
		else {		//内存剩余区域一个区块都无法满足
			//需要从系统取得空间的大小
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 2);
			
			//以下试着让内存池中剩余的零头还有利用价值
			if (bytes_left > 0) {
				/* 注意：内存池中的空间每次申请是8的倍数，每次取走也是8的倍数，所以只要有剩余也一定是8的倍数 */

				//寻找适当的free list，然后配备给free list
				obj* volatile* pp_free_list = free_lists + FREELIST_INDEX(bytes_left);

				//调整free list，将内存池中剩余的空间纳入
				((obj*)start_free)->free_list_link = *pp_free_list;
				*pp_free_list = (obj *)start_free;
			}

			/* 配置heap的空间，用来补充内存池 */
			start_free = (char *)malloc(bytes_to_get);
			if (0 == start_free)		//heap空间不足，malloc失败
			{
				obj* volatile* pp_free_list;
				//以下搜索适当的free list，即“尚有未用区块，且区块足够大”的free list
				for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					pp_free_list = free_lists + FREELIST_INDEX(i);
					obj* pObj = *pp_free_list;
					if (pObj != 0) {		//free list内尚有满足条件的区块
						//调整free list以释放未用区块
						*pp_free_list = pObj->free_list_link;
						start_free = (char *)pObj;
						end_free = start_free + i;

						//递归调用自己，为了修正nobjs
						return chunk_alloc(size, nobjs);

						//注意：任何残余零头都将被编入free list中备用
					}
				}
				end_free = 0;		//如果到处都没有内存可用了
				//调用第一级配置器，看看out of memory机制能否解决
				//这会抛出异常，或内存不足的情况获得改善
				start_free = (char *)malloc_alloc::allocate(bytes_to_get);
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;

			//递归调用自己，为了修正nobjs
			return chunk_alloc(size, nobjs);
		}
	}

	#define __NODE_ALLOCATER_THREADS false	 //非多线程环境

}// end of namespace detail


#ifdef __USE_MALLOC		//使用一级空间配置器
	typedef detail::malloc_alloc alloc;
#else					//使用二级空间配置器
	typedef detail::__default_alloc_template<__NODE_ALLOCATER_THREADS, 0> alloc;  //非多线程环境，直接将inst指定为0
#endif


	//SGI封装的标准的alloc配置器接口（一般都使用这个标准配置器接口）
	//使用这个接口时配置大小不再是以字节为单位，而是以元素大小为单位
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
	};

}// end of namespace lfp

#endif // !_STL_ALLOCATOR_H_