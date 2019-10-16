#include <mySTL/3stl_string.h>
#include <mySTL/14stl_map.h>
#include <iostream>

using namespace lfp;

int main() {
	map<string, int> simap;

	simap[string("stringY")] = 'Y';
	simap[string("stringD")] = 'D';
	simap[string("stringM")] = 'M';
	simap[string("stringA")] = 'A';

	std::pair<string, int> value("stringM", 100);	//insert failed
	simap.insert(value);
	
	value = std::pair<string, int>(string("stringF"), 'F');
	simap.insert(value);


	map<string, int>::iterator it = simap.begin();
	std::cout << "simap.size() = " << simap.size() << std::endl;
	for (; it != simap.end(); ++it)
		std::cout << it->first.c_str() << "  " << it->second << std::endl;
	std::cout << std::endl;



	simap[string("stringM")] = 666;
	it = simap.find("stringY");
	if (it != simap.end())
		it->second = 777;

	for (std::pair<string, int> p : simap)
		std::cout << p.first.c_str() << "  " << p.second << std::endl;
	std::cout << std::endl;



	simap.erase(simap.begin(), simap.find("stringF"));
	for (std::pair<string, int> p : simap)
		std::cout << p.first.c_str() << "  " << p.second << std::endl;
	std::cout << std::endl;


	return 0;
}