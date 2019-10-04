#include "pch.h"
#include <iostream>
#include "stack.h"

using namespace mySTL;
using namespace std;


int main() {
	stack<int> s;
	s.push(1);
	s.push(3);
	s.push(5);
	s.push(7);

	cout << "stack.size() = " << s.size() << endl << endl;

	cout << "stack.top() = " << s.top() << endl; s.pop();
	cout << "stack.top() = " << s.top() << endl; s.pop();
	cout << "stack.top() = " << s.top() << endl; s.pop();
	cout << "stack.top() = " << s.top() << endl; s.pop();

	cout << "\nstack.size() = " << s.size() << endl;

	return 0;
}