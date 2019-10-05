<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
set(mySTL_SRCS
	3stl_string.cc
)

add_library(mySTL ${mySTL_SRCS})

install(TARGETS mySTL DESTINATION lib)
<<<<<<< HEAD
=======
=======
#set(mySTL_SRCS
#)

#add_library(mySTL ${mySTL_SRCS})

#install(TARGETS mySTL DESTINATION lib)
>>>>>>> update
>>>>>>> update

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
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
	5stl_list.h
>>>>>>> update
>>>>>>> update
)
install(FILES ${HEADERS} DESTINATION include)