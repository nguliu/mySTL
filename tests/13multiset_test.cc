#include <mySTL/13stl_multiset.h>
#include <iostream>

using namespace lfp;

int main() {
	int ia[10] = { 0, 1, 2, 2, 3, 3, 3, 4, 4, 4 };
	multiset<int> iset(ia, ia + 10);

	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl << std::endl;


	multiset<int>::iterator it = iset.begin();

	for (; it != iset.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl << std::endl;



	iset.erase(iset.begin(), iset.find(4));	 // [)
	std::cout << "size() = " << iset.size() << std::endl;
	it = iset.begin();

	while (it != iset.end()) {
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl << std::endl;


	return 0;

}

