#include <mySTL/12stl_set.h>
#include <iostream>

using namespace lfp;

int main() {
	int ia[5] = { 0, 1, 2, 3, 4 };
	set<int> iset(ia, ia + 5);

	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl;
	iset.insert(3);
	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl << std::endl;


	iset.insert(5);
	set<int>::iterator it = iset.begin();
	//*it = 0;		//非法
	std::cout << "size() = " << iset.size() << std::endl;
	for (; it != iset.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl << std::endl;



	iset.erase(iset.begin(), iset.find(3));	 // [)
	std::cout << "size() = " << iset.size() << std::endl;
	it = iset.begin();
	auto end = iset.end();
	while (it != end) {
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl << std::endl;



	
	iset.erase(iset.begin(), iset.end());
	std::cout << "size() = " << iset.size() << std::endl;
	for (int i : iset)
		std::cout << i << " ";
	std::cout << std::endl << std::endl;

	return 0;

}

