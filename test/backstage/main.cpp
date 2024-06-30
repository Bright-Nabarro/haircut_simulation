#include <print>
#include "base_utility_test.hpp"
#include "cust_barb_chr_test.hpp"
#include "object_manager_test.hpp"
#include "factory_test.hpp"
#include "event_manager_test.hpp"

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
	//CustomerWaitingQue
	test_cwq_ini();
	test_cwq_push();
	test_cwq_pop();
	//Barber
	test_barber_ini();
	test_barber_accessor();
	test_barber_modifier();
	//BarberManager
	test_barber_manager_ini();
	test_barber_manager_get_free_barber();
	test_barber_set_free();
	//Chair
	test_chair_ini();
	test_chair_accessor();
	test_chair_modifier();
	//ChairManager
	test_chair_manager_ini();
	test_chair_manager_get_free();
	test_chair_manager_set_free();
}

static void object_manager_test()
{
	test_om_ini();
	test_om_register();
	test_om_get();
	test_om_remove();
	test_om_complex();
	test_om_iterator();
}

static void factory_test()
{
	test_fac_ini();
	test_fac_create();
	test_fac_complex();
}

static void event_manager_test()
{
	test_em_ini();
	test_em_push();
	test_em_execve();
}

int main()
{
	base_utility_test();
	cust_barb_chr_test();
	object_manager_test();
	factory_test();
	event_manager_test();
	std::println("((>ω<))↗ok");
}
