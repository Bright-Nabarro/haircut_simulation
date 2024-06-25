#include <print>
#include "base_utility_test.hpp"

static void base_utility_test()
{
	test_Tick_ini();
	test_Tick_cpr();
	test_Tick_inc();
}

int main()
{
	base_utility_test();
	std::println("((>ω<))↗ok");
}
