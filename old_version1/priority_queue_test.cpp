#include "pch.h"
#include <iostream>
#include "priority_queue.h"

using namespace mySTL;
using namespace std;


int main() {
	priority_queue<int> ipq = { 50, 10, 90, 30, 70, 40, 80, 60, 20 };

	while (ipq.size() > 0) {
		cout << "ipq.size() = " << ipq.size() << "  ipq.front() = " << ipq.front() << endl;
		ipq.pop();
	}

	return 0;
}