#include "pch.h"
#include <iostream>
#include "list.h"
#include "hashtable.h"

using namespace mySTL;
using namespace std;

<<<<<<< HEAD
/* 注意：list 的迭代器为双向迭代器没有 +n 操作，只能 ++ -- */
=======
<<<<<<< HEAD
/* 注意：list 的迭代器为双向迭代器没有 +n 操作，只能 ++ -- */
=======
/* 娉ㄦ剰锛歭ist 鐨勮凯浠ｅ櫒涓哄弻鍚戣凯浠ｅ櫒娌℃湁 +n 鎿嶄綔锛屽彧鑳 ++ -- */
>>>>>>> update
>>>>>>> update

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
<<<<<<< HEAD
	cout << "原始状况: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "原始状况: ilist.size() = " << ilist.size() << endl;
=======
	cout << "鍘熷鐘跺喌: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.sort();
<<<<<<< HEAD
	cout << "排序后: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "排序后: ilist.size() = " << ilist.size() << endl;
=======
	cout << "鎺掑簭鍚: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	if ((it = find(ilist.begin(), ilist.end(), 5)) != ilist.end()) {
		ilist.insert(it, 99);
<<<<<<< HEAD
		cout << "在5之前插入99: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
		cout << "在5之前插入99: ilist.size() = " << ilist.size() << endl;
=======
		cout << "鍦5涔嬪墠鎻掑叆99: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
		for (it = ilist.begin(); it != ilist.end(); ++it) {
			cout << *it << " ";
		}
		cout << endl << endl;
	}


	ilist.unique();
<<<<<<< HEAD
	cout << "去除连续重复项: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "去除连续重复项: ilist.size() = " << ilist.size() << endl;
=======
	cout << "鍘婚櫎杩炵画閲嶅椤: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.reverse();
<<<<<<< HEAD
	cout << "反转后: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "反转后: ilist.size() = " << ilist.size() << endl;
=======
	cout << "鍙嶈浆鍚: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	return 0;
<<<<<<< HEAD
}
=======
<<<<<<< HEAD
}
=======
}
>>>>>>> update
>>>>>>> update
