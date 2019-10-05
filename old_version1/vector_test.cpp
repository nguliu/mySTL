#include "pch.h"
#include <iostream>
#include "vector.h"

using namespace mySTL;
using namespace std;


int main() {

	vector<int> vec = { 8, 1, 2, 4, 3, 0, 9, 3, 5 };

	vector<int> ivec2(3, 9);
	vector<int> ivec(ivec2);

	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;
	ivec.push_back(1);
	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;
	ivec.push_back(2);
	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;
	ivec.push_back(3);
	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;
	ivec.push_back(4);
	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;
	ivec.push_back(5);

	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl;
	for (int i = 0; i < ivec.size(); ++i) {
		cout << ivec[i] << " ";
	}
	cout << endl << endl;

	ivec.pop_back(); ivec.pop_back();
	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl << endl;

	vector<int>::iterator it = find(ivec.begin(), ivec.end(), 2);
	if (it != ivec.end()) {
		ivec.insert(it, 3, 100);		//在2之前插入3个100
	}

	cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl;
	for (int i = 0; i < ivec.size(); ++i) {
		cout << ivec[i] << " ";
	}
	cout << endl << endl;

	if ((it = find(ivec.begin(), ivec.end(), 9)) != ivec.end()) {
		ivec.erase(it, it + 3);
		cout << "ivec.size() = " << ivec.size() << "  ivec.capacity() = " << ivec.capacity() << endl;
		for (int i = 0; i < ivec.size(); ++i) {
			cout << ivec[i] << " ";
		}
		cout << endl << endl;
	}

	return 0;
}