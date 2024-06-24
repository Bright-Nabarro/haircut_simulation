#include "base_utility.hpp"
using namespace simulation;

#include "base_utility_test.hpp"
void test_Tick_ini()
{
	Tick tick{1, 14, 2};
}

void test_Tick_cpr()
{
	Tick tick1{11, 41, 5};
	Tick tick2{11, 41, 5};
	assert(tick1 == tick2);
}
