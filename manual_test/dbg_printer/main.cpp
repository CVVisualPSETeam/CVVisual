#include "../../src/dbg/dbg.hpp"

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
	TRACEPOINT;
}

int main()
{

	cvv::dbg::setLoggingState(true);

	DEBUG("test debug");

	std::vector<int> simple_vec{ 0, 1, 2, 3 };

	TRACEPOINT;

	std::vector<std::pair<int, std::string>> vec = { { 1, "foo" },
		                                         { 2, "bar" },
		                                         { 3, "baz" } };

	TRACEPOINT;

	std::set<float> set{ 1.4, 2.5, 3.7 };

	TRACEPOINT;

	QString qstring = "some QString";
	DEBUG(qstring);

	TRACEPOINT;

	std::map<int, std::string> map{ { 1, "a" }, { 2, "b" } };
	DEBUG(map);
	TRACEPOINT;

	someOtherFunc();
	TRACEPOINT;
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
