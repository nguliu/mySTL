#include <mySTL/2stl_algobase.h>
#include <stdio.h>


struct Compare {
	template<class T>
	bool operator()(const T& a, const T& b) {
		return a < b;
	}
};

int main() {
	int ia[8] = { 1, 3, 5, 7, 8, 9, 10, 11 };
	Compare comp;

	int* p1 = lfp::lower_bound(ia, ia + 8, 2);
	int* p2 = lfp::lower_bound(ia, ia + 8, 5, comp);
	int* p3 = lfp::upper_bound(ia, ia + 8, 9);
	int* p4 = lfp::upper_bound(ia, ia + 8, 10, comp);

	printf("%d\n\n", *p1);
	printf("%d\n\n", *p2);
	printf("%d\n\n", *p3);
	printf("%d\n\n", *p4);

	return 0;
}