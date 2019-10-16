#include <mySTL/2stl_algobase.h>	//identity less
#include <mySTL/3stl_string.h>
#include <mySTL/11stl_rbtree.h>

#include <utility>			//包含 pair
#include <iostream>
#include <stdio.h>


using namespace lfp;
using namespace lfp::detail;

int main()
{
	rb_tree<int, int, identity<int>, less<int>> iitree;	//identity返回自身

	std::cout << "iitree.size() = " << iitree.size() << std::endl << std::endl;

	iitree.insert_unique(10);	//__rb_tree_rebalance
	iitree.insert_unique(7);		//__rb_tree_rebalance
	iitree.insert_unique(8);		//__rb_tree_rebalance
										//__rb_tree_rotate_left
										//__rb_tree_rotate_right
	iitree.insert_unique(15);	//__rb_tree_rebalance
	iitree.insert_unique(5);		//__rb_tree_rebalance
	iitree.insert_unique(6);		//__rb_tree_rebalance
										//__rb_tree_rotate_left
										//__rb_tree_rotate_right
	iitree.insert_unique(11);	//__rb_tree_rebalance
									//__rb_tree_rotate_right
									//__rb_tree_rotate_left
	iitree.insert_unique(13);	//__rb_tree_rebalance
	iitree.insert_unique(12);	//__rb_tree_rebalance
									//__rb_tree_rotate_right
	iitree.insert_unique(15);	//insert failed
	iitree.insert_unique(11);	//insert failed




	rb_tree<int, int, identity<int>, less<int>> iitree2 = iitree;	//调用拷贝构造函数
	iitree.clear();
	std::cout << "iitree.size() = " << iitree.size() << std::endl;
	iitree = iitree2;				//调用 operator=
	std::cout << "iitree.size() = " << iitree.size() << std::endl << std::endl;




	auto it1 = iitree.begin();
	for (; it1 != iitree.end(); ++it1)
		std::cout << *it1 << "(" << (it1.node->color == __rb_tree_red ? "r" : "b") << ") ";
	std::cout << std::endl << std::endl;

	it1 = iitree.find(10);
	auto it2 = iitree.find(100);	  //not find
	if (it1 != iitree.end()) {
		std::cout << "find " << *it1 << "  color:" << (it1.node->color == __rb_tree_red ? "red" : "black") << std::endl;
	}
	if (it2 == iitree.end()) {
		std::cout << "not find 100" << std::endl << std::endl;
	}




	std::cout << "iitree.size() = " << iitree.size() << "  count(11): " << iitree.count(11) << std::endl;
	iitree.insert_equal(11);
	iitree.insert_equal(11);
	std::cout << "iitree.size() = " << iitree.size() << "  count(11): " << iitree.count(11) << std::endl;

	it1 = iitree.begin();
	for (; it1 != iitree.end(); ++it1)
		std::cout << *it1 << "(" << (it1.node->color == __rb_tree_red ? "r" : "b") << ") ";
	std::cout << std::endl << std::endl;



	iitree.erase(iitree.begin(), iitree.end());

	it1 = iitree.begin();
	std::cout << "iitree.size() = " << iitree.size() << std::endl;
	for (; it1 != iitree.end(); ++it1)
		std::cout << *it1 << "(" << (it1.node->color == __rb_tree_red ? "r" : "b") << ") ";
	std::cout << std::endl << std::endl;



	
	typedef std::pair<int, string> Pair;
	rb_tree<string, Pair, select2nd<Pair>, less<string>> istr;	//投射函数select2nd返回第二参数作为key
	
	istr.insert_unique(Pair(2, "bat"));	//注意：这里投射函数是投射第二参数，即key是第二参数
	istr.insert_unique(Pair(2, "bta"));
	istr.insert_unique(Pair(2, "abt"));
	istr.insert_unique(Pair(2, "aaa"));
	istr.insert_unique(Pair(6, "c++"));
	istr.insert_unique(Pair(7, "is"));
	istr.insert_unique(Pair(10, "good"));
	std::cout << "istr.size() = " << istr.size() << std::endl;

	for (auto it = istr.begin(); it != istr.end(); ++it) {
		printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == __rb_tree_red ? "r" : "b"));
	}
	printf("\n\n");


	auto it = istr.find("bat");
	if (it != istr.end()) {
		istr.erase(it);
	}

	istr.erase("aaa");

	std::cout << "istr.size() = " << istr.size() << std::endl;
	for (auto it = istr.begin(); it != istr.end(); ++it) {
		printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == __rb_tree_red ? "r" : "b"));
	}
	printf("\n\n");


	return 0;
}