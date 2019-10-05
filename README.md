## mySTL

简易实现了常用的C++STL容器和算法  
其中的技术点主要包括：常用STL算法实现、空间配置器、内存池、__type_traits、iterator、string、vector、list、deque、stack、queue、heap、priority_queue、slist、rb-tree、map、set、hashtable、hash_map、hash_set  
  
  
## 主要内容如下：  
- 0jjalloc.h: 很简易的空间配置器，通过它可以了解空间配置器原理和new运算符、delete运算符背后的机制  
  
- 0jjauto_ptr.h: auto_ptr的简易实现，可为iterator提供参考  
  
- 1stl_alloctor.h: 对于new运算符内含两个阶段的操作：①调用::operator配置内存，②调用构造函数构造内容，这里实现①配置内存。实现了第一级空间配置器、第二级空间配置器和内存池，其中第一级空间配置器直接使用malloc/free/realloc等函数，第二级空间配置器维护16个8~128byte的自由链表（组织形式类似hash开链）和一个内存池，每次获取/释放内存时都是从自由链表获取/归还到自由链表，若自由链表空间不足时就从内存池获取，内存池空间不足时再从系统获取  
  
- 1stl_construct.h: 对于new运算符内含两个阶段的操作：①调用::operator配置内存，②调用构造函数构造内容，这里实现②内容构造。这里主要实现了全局函数construct和destroy，construct在已申请的内存空间上构造内容，destroy析构特定内存空间上的内容，他们都利用__type_traits<>求取最恰当的措施进行适当的优化  
  
- 1stl_iterator.h: 其中定义了一些迭代器相应属性，通过榨汁机iterator_traits很容易萃取出迭代器的特性，distance_type、iterator_category、value_type是对iterator_traits的封装，用于提取常用的迭代器属性  
  
- 1stl_type_traits.h: 实现了type_traits编程技法  
  
- 1stl_uninitialized.h: 实现内存基本处理工具uninitialized_copy、uninitialized_fill、uninitialized_fill_n  
  
- 2stl_algobase.h: 实现了常用的STL算法，其中利用__type_traits对算法进行了优化  
  
- 3stl_string.h: string实际是对char* 的封装，它使用原生的指针作为迭代器  
  
- 4stl_vector.h: vector具有array的有点同时又有许多高效的优化，主要优点有顺序存储、随机访问、动态扩容、预分配等，它的插入操作和删除操作容易导致迭代器的失效，它使用原生指针作为迭代器  
  
- 5stl_list.h: list作为一个双向链表对外表现，其内部实际是一个双向循环链表，初始状态时有一个空的节点组成双向循环链表。它的优点是插入、删除、接合等操作时不会使其他迭代器失效（甚至进行操作的那个迭代器也不会失效），由于涉及到大量的指针操作，其涉及难度大于vector。为了能融入STL算法，它必须使用定制的迭代器而不能使用原生的指针。由于STL的sort算法只接受随机迭代器，而list的迭代器是双向迭代器不支持随机访问，所有list需要自己实现排序算法，这里我采用了快速排序  
