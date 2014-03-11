
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <iostream>

#include <QString>

struct Unprintable
{
};

void someOtherFunc()
{
}

int main()
{



	std::vector<int> simple_vec{ 0, 1, 2, 3 };


	std::vector<std::pair<int, std::string>> vec = { { 1, "foo" },
		                                         { 2, "bar" },
		                                         { 3, "baz" } };


	std::set<float> set{ 1.4, 2.5, 3.7 };


	QString qstring = "some QString";


	std::map<int, std::string> map{ { 1, "a" }, { 2, "b" } };

	someOtherFunc();
	// Those are for testing of error-messages:
	// std::cout << "Unprintable: " <<
	// Aux::toString(std::vector<Unprintable>{}) << '\n';
	// std::cout << "Unprintable: " <<
	// Aux::toString(std::vector<std::vector<std::tuple<Unprintable>>>{}) <<
	// '\n';

	// And those are for comaprission:
	// std::cout << "Unprintable: " << Unprintable{} << '\n';
	// std::cout << "Unprintable: " <<
	// std::vector<std::vector<std::tuple<Unprintable>>>{} << '\n';
}
