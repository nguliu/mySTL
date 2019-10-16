#include <mySTL/5stl_list.h>
#include <mySTL/2stl_algobase.h>
//#include <iostream>

using namespace lfp;

/* 注意：list 的迭代器为双向迭代器没有 +n 操作，只能 ++ -- */

int main() {
	list<int> ilist;
	list<int>::iterator it;

	ilist.push_back(9);
	ilist.push_back(1);
	ilist.push_back(5);
	ilist.push_back(8);
	ilist.push_back(3);
	ilist.push_back(7);
	ilist.push_back(5);
	ilist.push_back(4);
	ilist.push_back(8);
	ilist.push_back(6);
	ilist.push_back(2);
	ilist.push_back(5);
	std::cout << "原始状况: ilist.size() = " << ilist.size() << std::endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl << std::endl;


	ilist.sort();
	std::cout << "排序后: ilist.size() = " << ilist.size() << std::endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl << std::endl;


	if ((it = find(ilist.begin(), ilist.end(), 5)) != ilist.end()) {
		ilist.insert(it, 99);
		std::cout << "在5之前插入99: ilist.size() = " << ilist.size() << std::endl;
		for (it = ilist.begin(); it != ilist.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl << std::endl;
	}


	ilist.unique();
	std::cout << "去除连续重复项: ilist.size() = " << ilist.size() << std::endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl << std::endl;


	ilist.reverse();
	std::cout << "反转后: ilist.size() = " << ilist.size() << std::endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl << std::endl;


	return 0;
}
