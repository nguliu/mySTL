#include "pch.h"
#include <iostream>
#include <string>
#include "rb_tree.h"


using namespace mySTL;

int main()
{
	rb_tree<int, int, identity<int>> itree;
	std::cout << "itree.size() = " << itree.size() << std::endl << std::endl;

	itree.insert_unique(10);	//__rb_tree_rebalance
	itree.insert_unique(7);		//__rb_tree_rebalance
	itree.insert_unique(8);		//__rb_tree_rebalance
									//__rb_tree_rotate_left
									//__rb_tree_rotate_right
	itree.insert_unique(15);	//__rb_tree_rebalance
	itree.insert_unique(5);		//__rb_tree_rebalance
	itree.insert_unique(6);		//__rb_tree_rebalance
									//__rb_tree_rotate_left
									//__rb_tree_rotate_right
	itree.insert_unique(11);	//__rb_tree_rebalance
									//__rb_tree_rotate_right
									//__rb_tree_rotate_left
	itree.insert_unique(13);	//__rb_tree_rebalance
	itree.insert_unique(12);	//__rb_tree_rebalance
									//__rb_tree_rotate_right

	rb_tree<int, int, identity<int>> itree2 = itree;	//调用拷贝构造函数		
	itree = itree2;		//调用 operator=

	auto it1 = itree.begin();
	auto it2 = itree.end();
	std::cout << "itree.size() = " << itree.size() << std::endl << std::endl;
	for (; it1 != it2; ++it1)
		std::cout << *it1 << "(" << (it1.node->color ? "b" : "r") << ") ";
	std::cout << std::endl << std::endl;

	it1 = itree.find(10);
	if (it1 != it2) {
		std::cout << "find " << *it1 << "  color:" << (it1.node->color ? "black" : "red") << std::endl << std::endl;
	}

	std::cout << "itree.size() = " << itree.size() << "  count(11): " << itree.count(11) << std::endl << std::endl;
	itree.insert_equal(11);
	itree.insert_equal(11);
	itree.insert_equal(11);
	std::cout << "itree.size() = " << itree.size() << "  count(11): " << itree.count(11) << std::endl << std::endl;

	itree.clear();

	return 0;
}