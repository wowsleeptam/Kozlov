#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char ** argv)
{
	std::string data = "00:11:10pm";
	std::regex pattern("((\\d{4})(\\.)((0[1-9])|(1[0-2]))(\\.)(([0-2][0-9])|(3[0-1])))|((([0][0-9])|(1[0-2]))(:)([0-5]\\d)(:)([0-5]\\d)(pm|am))");
// \d - цифра
	std::sregex_iterator begin(data.cbegin(), data.cend(), pattern);
	std::sregex_iterator end;

	std::for_each(begin, end, [](const std::smatch & m)
	{
		std::cout <<m[0] << std::endl;
	});

}
