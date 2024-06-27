#include <print>
#include "base_utility_test.hpp"
#include "cust_barb_chr_test.hpp"
#include "object_manager_test.hpp"

static void base_utility_test()
{
	//Tick class test
	test_Tick_ini();
	test_Tick_cpr();
	test_Tick_inc();

	//Id class test
	test_Id_ini();
	test_Id_get_number();
	test_Id_ownership();
	test_hash_equal_Id();
}

static void cust_barb_chr_test()
{
	//Customer
	test_customer_ini();
	test_customer_accessor();
	test_customer_modifier();
	//Barber
	test_barber_ini();
	test_barber_accessor();
	test_barber_modifier();
	//Chair
	test_chair_ini();
	test_chair_accessor();
	test_chair_modifier();
}

static void object_manager_test()
{
	test_ini();
}

int main()
{
	base_utility_test();
	cust_barb_chr_test();
	object_manager_test();
	std::println("((>ω<))↗ok");
}
