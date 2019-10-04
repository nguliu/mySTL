## mySTL

简易实现了常用的C++STL容器和算法，其中的技术点主要包括：常用算法实现、空间配置器、内存池、__type_traits、iterator、string、vector、


## 内容如下：
- 0jjalloc.h: 很简易的空间配置器，通过它可以了解空间配置器原理和new运算符、delete运算符背后的机制
- 0jjauto_ptr.h: auto_ptr的简易实现，可为iterator提供参考
- 1stl_alloctor.h: 对于new运算符内含两个阶段的操作：①调用::operator配置内存，②调用构造函数构造内容，这里实现①配置内存。实现了第一级空间配置器、第二级空间配置器和内存池，其中第一级空间配置器直接使用malloc/free/realloc等函数，第二级空间配置器维护16个8~128byte的自由链表（组织形式类似hash开链）和一个内存池，每次获取/释放内存时都是从自由链表获取/归还到自由链表，若自由链表空间不足时就从内存池获取，内存池空间不足时再从系统获取
- 1stl_construct.h: 对于new运算符内含两个阶段的操作：①调用::operator配置内存，②调用构造函数构造内容，这里实现②内容构造。这里主要实现了全局函数construct和destroy，construct在已申请的内存空间上构造内容，destroy析构特定内存空间上的内容，他们都利用__type_traits<>求取最恰当的措施进行适当的优化
- 1stl_iterator.h: 其中定义了一些迭代器相应属性，通过榨汁机iterator_traits很容易萃取出迭代器的特性，distance_type、iterator_category、value_type是对iterator_traits的封装，用于提取常用的迭代器属性
- 1stl_type_traits.h: 实现了type_traits编程技法
- 1stl_uninitialized.h: 实现内存基本处理工具uninitialized_copy、uninitialized_fill、uninitialized_fill_n
- 2stl_algobase.h: 实现了常用的STL算法，其中利用__type_traits对算法进行了优化
- 3stl_string.{h,cc}: string的实现
- 4stl_vector.h: vector实现
