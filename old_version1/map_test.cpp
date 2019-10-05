#include "pch.h"
#include <iostream>
#include <string>
#include <map>
#include "map.h"

using namespace mySTL;

int main() {
	map<std::string, int> simap;
	simap[std::string("stringY")] = 'Y';
	simap[std::string("stringD")] = 'D';
	simap[std::string("stringM")] = 'M';
	simap[std::string("stringA")] = 'A';
	std::pair<std::string, int> value(std::string("stringM"), 100);
	simap.insert(value);
	value = std::pair<std::string, int>(std::string("stringF"), 'F');
	simap.insert(value);


	map<std::string, int>::iterator it = simap.begin();
	std::cout << "simap.size() = " << simap.size() << std::endl;
	for (; it != simap.end(); ++it)
		std::cout << it->first << "  " << it->second << std::endl;
	std::cout << std::endl;



	simap[std::string("stringM")] = 666;
	it = simap.find("stringY");
	if (it != simap.end())
		it->second = 777;

	for (std::pair<std::string, int> p : simap)
		std::cout << p.first << "  " << p.second << std::endl;
	std::cout << std::endl;


	return 0;
}