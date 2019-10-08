#set(mySTL_SRCS
#)

#add_library(mySTL ${mySTL_SRCS})

#install(TARGETS mySTL DESTINATION lib)



#设置哪些头文件需要安装（即对外可见的文件）
set(HEADERS
	0jjalloc.h
	0jjauto_ptr.h
	1stl_alloctor.h
	1stl_construct.h
	1stl_iterator.h
	1stl_type_traits.h
	1stl_uninitialized.h
	2stl_algobase.h
	3stl_string.h
	4stl_vector.h
	5stl_list.h
	6stl_deque.h
	7stl_stack.h
	8stl_queue.h
	9stl_heap.h
	9stl_priority_queue.h
	10stl_slist.h
)
install(FILES ${HEADERS} DESTINATION include)
