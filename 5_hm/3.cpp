#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char ** argv)
{
	std::string data = "143.123.123.424@sdafasdf.asdf.asdf.asd.fa.sdf.fsd.ds.dd";
		std::regex pattern("([\\w._-]+)(@)((([\\w+]+)(\\.))+)([a-z]{2,9})");

	std::sregex_iterator begin(data.cbegin(), data.cend(), pattern);
	std::sregex_iterator end;

	std::for_each(begin, end, [](const std::smatch & m)
	{
		std::string l = m[0];
		std::regex forbbiden_pattern("(^(([[1]?[0-9]?[0-9])|([2][0-4][0-9])|(25[0-5]))\\.)((([[1]?[0-9]?[0-9])|([2][0-4][0-9])|(25[0-5]))\\.){2}(([[1]?[0-9]?[0-9])|([2][0-4][0-9])|(25[0-5]))");
		std::regex pattern_2("(@)[\\w-+_.]+");
		std::cmatch result;
		// Не принимает ip адрес
		if((std::regex_search(l.c_str(), result, pattern_2)) && (std::regex_search(l.c_str(), forbbiden_pattern) == 0))
			std::cout << result[0] << std::endl;
	});

}
