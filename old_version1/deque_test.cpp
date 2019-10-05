#include "pch.h"
#include <iostream>
#include "deque.h"

using namespace mySTL;
using namespace std;


int main() {
	deque<int, alloc, 8> que(20, 9);		//构造一个拥有20个int型元素初值为9、缓冲区为8个元素的deque
	//为每个元素设定新值
	for (int i = 0; i < que.size(); ++i) {
		que[i] = i;
	}

	cout << "1: que.size() = " << que.size() << endl;
	for (int i : que)
		cout << i << " ";
	cout << endl << endl;


	//在尾端压入三个元素，此时三个缓冲区刚好填满（左闭右开原则会留一个空的位置）
	for (int i = 0; i < 3; ++i) {
		que.push_back(20 + i);
	}
	cout << "2: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;

	//在尾端增加一个元素 23，此时需要生成新的缓冲区
	que.push_back(23);
	cout << "3: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;


	//在前端压入三个元素
	for (int i = 0; i < 3; ++i) {
		que.push_front(100 + i);
	}
	cout << "4: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;


	que.erase(que.begin() + 4, que.begin() + 10);
	cout << "5: que.size() = " << que.size() << endl;
	for (deque<int, alloc, 8>::iterator it = que.begin(); it < que.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	que.insert(que.begin() + 10, 8888);
	cout << "6: que.size() = " << que.size() << endl;
	for (deque<int, alloc, 8>::iterator it = que.begin(); it < que.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;

	return 0;
}