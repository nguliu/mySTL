#include <mySTL/2stl_algobase.h>
#include <mySTL/4stl_vector.h>
#include <mySTL/9stl_priority_queue.h>
#include <iostream>

using namespace lfp;

int main() {
	//默认大根堆
	priority_queue<int> ipq = { 50, 10, 90, 30, 70, 40, 80, 60, 20 };
	while (ipq.size() > 0) {
		std::cout << "ipq.size() = " << ipq.size() << "  ipq.top() = " << ipq.top() << std::endl;
		ipq.pop();
	}
	std::cout << std::endl << std::endl;

	
	//小根堆
	priority_queue<int, vector<int>, greater<int>> ipq2 = { 50, 10, 90, 30, 70, 40, 80, 60, 20 };
	while (ipq2.size() > 0) {
		std::cout << "ipq2.size() = " << ipq2.size() << "  ipq2.top() = " << ipq2.top() << std::endl;
		ipq2.pop();
	}
	std::cout << std::endl << std::endl;

	return 0;
}