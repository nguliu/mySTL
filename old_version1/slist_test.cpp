#include "pch.h"
#include <iostream>
#include "slist.h"

using namespace mySTL;
using namespace std;


int main()
{
	slist<int> islist;
	islist.push_front(9);
	islist.push_front(1);
	islist.push_front(2);
	islist.push_front(3);
	islist.push_front(4);
	cout << "slist.size() = " << islist.size() << endl;

	slist<int>::iterator beg = islist.begin();
	slist<int>::iterator end = islist.end();
	while (beg != end) {
		cout << *beg++ << " ";
	}
	cout << endl << endl;

	slist<int>::iterator it = find(islist.begin(), islist.end(), 4);
	if (it != islist.end())
		islist.insert(it, 99);

	beg = islist.begin();
	while (beg != end) {
		cout << *beg++ << " ";
	}
	cout << endl << endl;


	return 0;
}