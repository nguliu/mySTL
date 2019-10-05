#include "pch.h"
#include <iostream>
#include "hashtable.h"

using namespace mySTL;


int main() {
	hashtable<int, int, std::hash<int>, identity<int>, equal_to<int>, alloc>	iht(50, std::hash<int>(), equal_to<int>());
	

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	std::cout << "iht.max_bucket_count() = " << iht.max_bucket_count() << std::endl << std::endl;

	iht.insert_unique(59);
	iht.insert_unique(63);
	iht.insert_unique(108);
	iht.insert_unique(2);
	iht.insert_unique(53);
	iht.insert_unique(55);
	iht.insert_unique(108);		//insert 6 elemts

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buclets[" << i << "] has " << n << " elemts.\n";
	}
	std::cout << std::endl;

	

	for (int i = 0; i < 50; ++i)
		iht.insert_equal(i);

	
	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buclets[" << i << "] has " << n << " elemts.\n";
	}
	std::cout << "\ncount of 2 " << iht.count(2) << "times  " << *iht.find(108) << std::endl;
	std::cout << std::endl << std::endl;


	iht.erase(iht.begin(), iht.end());
	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;


	return 0;
}