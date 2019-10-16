#include <mySTL/3stl_string.h>
#include <mySTL/15stl_multimap.h>
#include <iostream>

using namespace lfp;

int main() {
	multimap<string, int> simap;

	simap[string("stringY")] = 'Y';
	simap[string("stringD")] = 'D';
	simap[string("stringM")] = 'M';
	simap[string("stringA")] = 'A';

	simap.insert(std::pair<string, int>("stringM", 100));



	multimap<string, int>::iterator it = simap.begin();
	std::cout << "simap.size() = " << simap.size() << std::endl;
	for (; it != simap.end(); ++it)
		std::cout << it->first.c_str() << "  " << it->second << std::endl;
	std::cout << std::endl;



	simap.insert(std::pair<string, int>("stringX", 666));
	simap.insert(std::pair<string, int>("stringX", 777));
	simap.insert(std::pair<string, int>("stringX", 888));
	simap.insert(std::pair<string, int>("stringX", 999));

	std::cout << "simap.size() = " << simap.size() << std::endl;
	for (std::pair<string, int> p : simap)
		std::cout << p.first.c_str() << "  " << p.second << std::endl;
	std::cout << std::endl;



	simap.erase(simap.begin(), simap.find("stringX"));

	std::cout << "simap.size() = " << simap.size() << std::endl;
	for (std::pair<string, int> p : simap)
		std::cout << p.first.c_str() << "  " << p.second << std::endl;
	std::cout << std::endl;


	return 0;
}