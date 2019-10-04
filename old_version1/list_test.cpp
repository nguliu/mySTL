#include "pch.h"
#include <iostream>
#include "list.h"
#include "hashtable.h"

using namespace mySTL;
using namespace std;

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
	cout << "原始状况: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.sort();
	cout << "排序后: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	if ((it = find(ilist.begin(), ilist.end(), 5)) != ilist.end()) {
		ilist.insert(it, 99);
		cout << "在5之前插入99: ilist.size() = " << ilist.size() << endl;
		for (it = ilist.begin(); it != ilist.end(); ++it) {
			cout << *it << " ";
		}
		cout << endl << endl;
	}


	ilist.unique();
	cout << "去除连续重复项: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.reverse();
	cout << "反转后: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	return 0;
}