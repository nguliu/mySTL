#include <mySTL/2stl_algobase.h>
#include <mySTL/10stl_slist.h>
#include <iostream>

using namespace lfp;


int main()
{
	slist<int> islist;
	islist.push_front(9);
	islist.push_front(1);
	islist.push_front(2);
	islist.push_front(3);
	islist.push_front(4);
	std::cout << "slist.size() = " << islist.size() << std::endl;

	slist<int>::iterator first = islist.begin();
	slist<int>::iterator last = islist.end();
	while (first != last) {
		std::cout << *first++ << " ";
	}
	std::cout << std::endl << std::endl;

	slist<int>::iterator it = find(islist.begin(), islist.end(), 3);
	if (it != islist.end())
		islist.insert_after(it, 99);

	first = islist.begin();
	while (first != last) {
		std::cout << *first++ << " ";
	}
	std::cout << std::endl << std::endl;

	return 0;
}
