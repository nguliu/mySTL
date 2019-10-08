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
  
- 6stl_deque.h：**deque是序列式容器中设计难度最大的容器**，主要原因在于其特殊的数据结构和配套的迭代器设计（从deque的设计中可以看出每种容器只能由自己来设计迭代器）。其内部首先以一块连续的空间作为map，map内的每个node指向一块连续的缓冲区，这些连续的缓冲区才是真正存放数据的位置，初始状态是它维护一个空的缓冲区，当一块缓冲区满时，需要申请一块新的缓冲区，并通过map内的一个node记录其位置，当map满时需要reallocate_map，即更换一块更大的map。由于deque内部是段式连续空间，对外表现为逻辑上的连续空间任务就交给了deque的迭代器（迭代器支持随机访问），迭代器的设计变得复杂，迭代器内部维护4个指针，node指向map中记录当前迭代器所在缓冲区的node，这是因为当迭代器需要跨缓冲区移动时它必须要通过map找到下一块缓冲区在哪，first指向当前元素所在缓冲区的第一个位置，last指向当前元素所在缓冲区最后一个位置的下一位置，cur指向缓冲区中当前迭代器所指元素的位置，通过它提领迭代器所指元素  
  
- 7stl_stack.h: stack实际不是一种容器而是一个配接器，其内部使用list作为底层容器（也可使用deque、vector等），它通过底层容器实现对外操作，因而实现非常简单  
  
- 8stl_queue.h: queue是单向队列，它与stack一样，都是一个配接器，其内部使用list作为底层容器（也可使用deque、vector等），它通过底层容器实现对外操作，因而实现非常简单  
  
- 9stl_heap.h: heap并不属于标准库的内容，这里主要提供push_heap、pop_head、sort_heap、make_heap几个泛型算法，它们接受一对迭代器和一个权值比较规则，对底层容器中迭代器范围内的元素进行相应操作，从而维持底层容器堆的特性  
  
- 9stl_priority_queue.h: 优先级队列的实现，这是一个配接器而不是一个容器，它默认以vector为底层容器，通过heap调用相关算法，维持底层容器中的元素保持堆的特性  
  
- 10stl_slist.h: slist和list最大的区别在于，前者的迭代器属于单向的ForwardIterator，而后者是双向迭代器BidirectionalIterator。因此slist的功能就受到很多限制，但slist消耗的空间更少。和list一样，它们的插入、移除、接合等操作不会造成原有的迭代器失效。
注意：根据STL的习惯，插入操作会将元素插入到迭代器所指位置之前而不是之后，但slist没办法快速找到其前一个节点，只能从头遍历，这便是slist最大的缺点，因此slist不提供push_back操作，只提供insert_after、erase_after、push_front操作  
  
## Environment
- OS: Ubuntu 18.04
- Kernel: 5.0.0-27-generic
- Complier: g++ 7.4.0
- cmake: 3.10.2（如果你的版本过低请自行修改根目录下CMakeLists.txt内的版本号）
## Build and install  
- ./build.sh  
- ./build.sh install  
