#include <mySTL/3stl_string.h>
#include <iostream>

using namespace lfp;

int main()
{
	//constructor
	{
		string str1;
		string str2(str1);
		string str3("abcdefg");
		string str4(str3.begin()+1, str3.end() - 1);
		string str5(10, 'a');
		std::cout << "str1: " << str1.c_str() << std::endl;
		std::cout << "str2: " << str2.c_str() << std::endl;
		std::cout << "str3: " << str3.c_str() << std::endl;
		std::cout << "str4: " << str4.c_str() << std::endl;
		std::cout << "str5: " << str5.c_str() << std::endl << std::endl;
	}

	// length, size, c_str()
	{
		string str1("this is string1");
		std::cout << "str1 length: " << str1.length() << std::endl;
		std::cout << "str1 size: " << str1.size() << std::endl;
		std::cout << "str1: " << str1.c_str() << std::endl << std::endl;
	}


	// operator =
	{
		string str1("https://blog.csdn.net/qq_40843865");
		string str2, str3;
		str2 = str1;
		str3 = "https://github.com/breking";
		printf("str2: %s \nstr3: %s\n\n", str2.c_str(), str3.c_str());
	}

	// operator +=
	{
		string str1 = "CSDN: ";
		string str2 = "https://blog.csdn.net/qq_40843865";
		str1 += str2;
		printf("%s\n", str1.c_str());
		string str3("GitHub: ");
		str3 += "https://github.com/breking";
		printf("%s\n\n", str3.c_str());
	}

	// append
	{
		string str1 = "CSDN: ";
		string str2 = "https://blog.csdn.net/qq_40843865";
		str1.append(str2);
		printf("%s\n", str1.c_str());
		string str3("GitHub: ");
		str3.append("https://github.com/breking");
		printf("%s\n\n", str3.c_str());
	}

	// assign
	{
		string str1;
		string str2("CSDN: https://blog.csdn.net/qq_40843865");
		str1.assign(str2);
		printf("str1: %s\n\n", str1.c_str());
	}

	// at
	{
		string str("Test String");
		const char& a = str.at(5);
		printf("a: %c, str: %s\n\n", a, str.c_str());
	}

	{ // clear
		string str("Test String");
		printf("str: %s\n", str.c_str());
		str.clear();
		printf("str: %s\n\n", str.c_str());
	}

	{ // compare
		string str1("Test"), str2("String"), str3("Apple"), str4("String");
		printf("str2:str1: %d; st2:str3: %d; str2:str4: %d\n",
			str2.compare(str1), str2.compare(str3), str2.compare(str4));
		printf("str2:str1: %d; st2:str3: %d; str2:str4: %d\n\n",
			str2.compare("Test"), str2.compare("Apple"), str2.compare("String"));
	}


	// operator +
	{
		string firstlevel("com");
		string secondlevel("cplusplus");
		string scheme("http://");
		string hostname;
		string url;

		hostname = "www." + secondlevel + '.' + firstlevel;
		url = scheme + hostname;
		printf("url: %s\n\n", url.c_str());
	}

	// operator: ==, !=, <, <=, >, >=
	{
		string foo = "alpha";
		string bar = "beta";

		if (foo == bar) printf("foo == bar\n");
		if (foo != bar) printf("foo != bar\n");
		if (foo < bar) printf("foo < bar\n");
		if (foo > bar) printf("foo > bar\n");
		if (foo <= bar) printf("foo <= bar\n");
		if (foo >= bar) printf("foo >= bar\n\n");
	}

	return 0;
}