#include <mySTL/2stl_algobase.h>
#include <mySTL/16stl_hashtable.h>
#include <iostream>

using namespace lfp;
using namespace lfp::detail;

int main() {
	hashtable<int,				// value
			  int,				// key
			  std::hash<int>,	// hash function
			  identity<int>,	// extract key
			  equal_to<int>,	// equal key
			  alloc>
	iht(50, std::hash<int>(), equal_to<int>());
	

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	std::cout << "iht.max_bucket_count() = " << iht.max_bucket_count() << std::endl << std::endl;

	iht.insert_unique(50);
	iht.insert_unique(23);
	iht.insert_unique(18);
	iht.insert_unique(2);
	iht.insert_unique(52);
	iht.insert_unique(5);
	iht.insert_unique(18);		//insert 6 elemts

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buckets[" << i << "] has " << n << " elemts.\n";
	}
	std::cout << std::endl;

	

	for (int i = 0; i < 53; ++i)
		iht.insert_equal(i);

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buckets[" << i << "] has " << n << " elemts.\n";
	}

	std::cout << "\ncount of 18 " << iht.count(18) << " times  " << *iht.find(18) << std::endl << std::endl;

	
	iht.insert_unique(90);		//看看是否会发生表格重建

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buckets[" << i << "] has " << n << " elemts.\n";
	}
	std::cout << std::endl;


	iht.erase(iht.begin(), iht.find(90));

	std::cout << "iht.size() = " << iht.size() << std::endl;
	std::cout << "iht.bucket_count() = " << iht.bucket_count() << std::endl;
	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elemts_in_buckets(i);
		if (n != 0)
			std::cout << "buckets[" << i << "] has " << n << " elemts.\n";
	}
	std::cout << std::endl;

	return 0;
}