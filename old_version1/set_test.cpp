#include "pch.h"
#include <iostream>

#include "set.h"

using namespace mySTL;

int main() {
	int ia[5] = { 0, 1, 2, 3, 4 };
	set<int> iset(ia, ia + 5);

	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl;
	iset.insert(3);
	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl << std::endl;

	iset.insert(5);
	set<int>::iterator it = iset.begin();
	//*it = 0;		//·Ç·¨
	for (; it != iset.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl << std::endl;

	it = iset.find(3);
			if (it != iset.end())
				std::cout << "find " << *it << std::endl;
			else
				std::cout << "not find 3" << std::endl;
	it = iset.find(6);
			if (it != iset.end())
				std::cout << "find " << *it << std::endl;
			else
				std::cout << "not find 6" << std::endl;

	for (int i : iset)
		std::cout << i << " ";
	std::cout << std::endl << std::endl;

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
