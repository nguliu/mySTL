#include "pch.h"
#include <iostream>
#include "heap.h"

using namespace mySTL;
using namespace std;

int main() {
	heap<int> iheap{ 50, 10, 90, 30, 70, 40, 80, 60, 20, 100 };


	iheap.make_heap();
	for (int i : iheap.C)
		cout << i << " ";
	cout << endl << endl;


	iheap.push(66);
	for (int i : iheap.C)
		cout << i << " ";
	cout << endl << endl;


	iheap.pop();
	for (int i : iheap.C)
		cout << i << " ";
	cout << endl << endl;


	iheap.sort();
	for (int i : iheap.C)
		cout << i << " ";
	cout << endl << endl;
	
	return 0;
}