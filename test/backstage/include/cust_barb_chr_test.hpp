#pragma once
#include "test_header.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"

TPF
void test_customer_ini()
{
	using namespace simulation;
	Customer c1 { Level::BEG, 1.2, 1000 };
	Customer c2 {Level::INT, 1.3, 500 };
	Customer c3 { Level::ADV, 0.9, 114514 };
	Customer c4 { Level::FAST, 0.8, 515 };
}

TPF
void test_customer_accessor(bool display = false)
{
	using namespace simulation;
	using namespace std;
	const Customer c1 { Level::BEG, 1.2, 1000 };
	if (display)
		println("{}", c1.get_id().get_id_number());
	assert(Level::BEG == c1.get_level());
	assert(1.2 == c1.get_time_factor());
	assert(1000 == c1.get_max_waiting_time());
	assert(!c1.haircut_started());
}

TPF
void test_customer_modifier()
{	
	using namespace simulation;
	using namespace std;
	Customer c1 {Level::INT, 1.3, 500 };
	assert(!c1.haircut_started());
	c1.start_cuting_hear();
	assert(c1.haircut_started());
}

TPF 
void test_barber_ini()
{
	using namespace simulation;
	using namespace std;
	Barber b1 { Level::BEG, 1.2 };
	Customer c1 { Level::BEG, 1.1, 1222 };
	Barber b2 { Level::BEG, 0.98, c1.get_id() };
}

TPF
void test_barber_accessor(bool display = false)
{
	using namespace simulation;
	using namespace std;
	Barber b1 { Level::BEG, 1.2 };
	if (display)
		println("barber id {}", b1.get_id().get_id_number());
	assert(b1.get_level() == Level::BEG);
	assert(b1.get_time_factor() == 1.2);
	assert(!b1.busy());
	try
	{
		decltype(auto) _ {b1.get_customer_id()};
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}
	
	try
	{
		Barber b2 { Level::FAST, 1.2 };
		assert(false);
	}
	catch(invalid_argument& e)
	{
		if (display)
			println("invalid argument: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}

	Customer c3 { Level::FAST, 1.01, 11111 };
	Barber b3 { Level::ADV, 1.001, c3.get_id() };
	if (display)
	{
		println("b3 id {}", b3.get_id().get_id_number());
		println("b3 customer id {}", b3.get_customer_id().get_id_number());
	}
	assert(b3.get_level() == Level::ADV);
	assert(b3.get_time_factor() == 1.001);
	assert(b3.busy());
}

TPF
void test_barber_modifier(bool display = false)
{	
	using namespace simulation;
	using namespace std;
	Barber b1 { Level::BEG, 1.2 };
	try
	{
		b1.release_customer();
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}
	Customer c1 { Level::BEG, 1.02, 1234 };
	b1.set_customer_id(c1.get_id());
	assert(c1.get_id() == b1.get_customer_id());
	assert(b1.busy());
	b1.release_customer();
	assert(!b1.busy());

	Customer c3 { Level::FAST, 1.01, 11111 };
	Barber b3 { Level::ADV, 1.001, c3.get_id() };
	b3.release_customer();
	assert(!b3.busy());
}

TPF 
void test_chair_ini()
{
	using namespace simulation;
	using namespace std;
	Chair ch1;
	Customer c2 { Level::BEG, 1.02, 1000 }; 
	Barber b2 { Level::BEG, 1.03, c2.get_id() };
	Chair ch2 { c2.get_id(), b2.get_id() };
}

TPF
void test_chair_accessor(bool display = false)
{
	using namespace simulation;
	using namespace std;
	
	Chair ch1;
	if (display)
		println("{}", ch1.get_id().get_id_number());
	assert(!ch1.busy());
	try
	{
		decltype(auto) _ { ch1.get_customer_id() };
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}

	try
	{
		decltype(auto) _ { ch1.get_barber_id() };
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}


	Customer c2 { Level::BEG, 1.02, 1000 }; 
	Barber b2 { Level::BEG, 1.03, c2.get_id() };
	Chair ch2 { c2.get_id(), b2.get_id() };
	assert(ch2.busy());
	assert(ch2.get_customer_id() == c2.get_id());
	assert(ch2.get_barber_id() == b2.get_id());
	
}

TPF
void test_chair_modifier(bool display = false)
{
	using namespace simulation;
	using namespace std;

	Chair ch1;
	try
	{
		ch1.release();
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}
	Customer c1 { Level::ADV, 1, 12031 };
	Barber b1 { Level::ADV, 1, c1.get_id() };
	ch1.set_customer_barber(c1.get_id(), b1.get_id());
	assert(ch1.busy());
	assert(ch1.get_customer_id() == c1.get_id());
	assert(ch1.get_barber_id() == b1.get_id());
	ch1.release();
	assert(!ch1.busy());
	try
	{
		ch1.release();
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}

	Customer c2 { Level::BEG, 1.02, 1000 }; 
	Barber b2 { Level::BEG, 1.03, c2.get_id() };
	Chair ch2 { c2.get_id(), b2.get_id() };
	ch2.release();
	assert(!ch2.busy());
	try
	{
		ch2.release();
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("logic error: {}", e.what());
	}
	catch(...)
	{
		assert(false);
	}

	Customer c3 { Level::BEG, 1.03, 1000 }; 
	Customer c4 { Level::BEG, 1.03, 1000 }; 
	Customer c5 { Level::BEG, 1.03, 1000 }; 
	Barber b3 { Level::BEG, 1.03, c3.get_id() };
	Barber b4 { Level::BEG, 1.03, c3.get_id() };
	Barber b5 { Level::BEG, 1.03, c3.get_id() };
	Chair ch3 { c3.get_id(), b3.get_id() };
	ch3.set_customer_barber(c4.get_id(), b4.get_id());
	assert(ch3.get_customer_id() == c4.get_id());
	assert(ch3.get_barber_id() == b4.get_id());
	ch3.set_customer_barber(c5.get_id(), b5.get_id());
	assert(ch3.get_customer_id() == c5.get_id());
	assert(ch3.get_barber_id() == b5.get_id());

}
