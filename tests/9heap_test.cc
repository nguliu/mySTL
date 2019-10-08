#include <mySTL/2stl_algobase.h>	//使用大小比较算法
#include <mySTL/3stl_string.h>
#include <mySTL/4stl_vector.h>
#include <mySTL/9stl_heap.h>
#include <stdio.h>

using namespace lfp;

struct MyCompare {
	bool operator()(const string& stra, const string& strb) {
		return stra[stra.size() - 1] < strb[strb.size() - 1];		//以字符串最后一个字母进行排序
	}
};

int main() {
	vector<int> ivec{ 50, 10, 90, 30, 70, 40, 80, 60, 20, 100, 80, 20, 50 };
	
	less<int> lessComp;		//权值比较规则（< 符号）
	make_heap(ivec.begin(), ivec.end(), lessComp);	//以 < 作为比较规则，ivec是大顶堆
	printf("ivec[0] = %d\n", ivec[0]);				//100

	sort_heap(ivec.begin(), ivec.end(), lessComp);	//升序排序
	for (int i : ivec)
		printf("%d ", i);
	printf("\n\n");




	greater<int> greaterComp;		//权值比较规则（> 符号）
	make_heap(ivec.begin(), ivec.end(), greaterComp);	//以 > 作为比较规则，ivec是小顶堆
	printf("ivec[0] = %d\n", ivec[0]);					//10

	auto first = ivec.begin(), last = ivec.end();
	sort_heap(ivec.begin(), ivec.end(), greaterComp);	//降序排序
	for (int i : ivec)
		printf("%d ", i);
	printf("\n\n");




	MyCompare mycomp;
	vector<string> strvec{ "strd", "sc", "bte", "zzzzzzz", "ab", "zx", "bat", "bta", "tab" };

	make_heap(strvec.begin(), strvec.end(), mycomp);
	sort_heap(strvec.begin(), strvec.end(), mycomp);	//按最后一个字母升序排序

	for (auto str : strvec)
		printf("%s ", str.c_str());
	printf("\n\n");



	return 0;
}