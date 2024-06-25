#pragma once
#include <cassert>
#include <print>
#include "base_utility.hpp"
#include "exception.hpp"

//测试函数只会调用一次，简化起见在头文件定义，使用强制内联

#define TPF [[gnu::always_inline]] inline 	//后一个inline只是为了消除静态检测警告

TPF
void test_Tick_ini()
{
	using namespace simulation;
	Tick tick1{1, 14, 2};

	try
	{
		Tick tick{24, 1, 2};
		assert(false);
	}
	catch (InvalidTimepoint& e)
	{
		//true branches
	} 
	catch(...)
	{
		assert(false);
	}
	
	try
	{
		Tick tick{2, 199, 2};
		assert(false);
	}
	catch (InvalidTimepoint& e)
	{
		//true branches
	} 
	catch(...)
	{
		assert(false);
	}

	try
	{
		Tick tick{2, 19, 60};
		assert(false);
	}
	catch (InvalidTimepoint& e)
	{
		//true branches
	} 
	catch(...)
	{
		assert(false);
	}
}

TPF
void test_Tick_cpr()
{
	using namespace simulation;

	Tick tick1{11, 41, 5};
	Tick tick2{11, 41, 5};
	assert(tick1 == tick2);

	Tick tick3 { 15, 7, 9 };
	Tick tick4 { tick3 };
	assert(tick3 == tick4);
	
	auto tncpr = [](const Tick& t1, Tick& t2) {
		assert(t1 < t2);
		assert(t2 > t1);
		assert(t1 != t2);
		assert(t1 <= t2);
		assert(t2 >= t1);
	};

	Tick tick5 { 0, 0, 0 };
	Tick tick6 { 0, 0, 1 };
	tncpr(tick5, tick6);

	Tick tick7 {2, 42, 22 };
	Tick tick8 { 2, 45, 12 };
	tncpr(tick7, tick8);

	Tick tick9 { 22, 12, 00 };
	Tick tick10 { 23, 11, 33 };
	tncpr(tick9, tick10);

}

TPF
void test_Tick_inc()
{
	using namespace simulation;
	Tick tick1 { 0, 0, 0 };
	Tick tick2 { 1, 2, 3 };
	tick1.increament(1, 2, 3);
	assert(tick1 == tick2);
	
	Tick tick3 { 11, 12, 13 };
	Tick tick3Cpy { tick3 };
	tick3.increament(0, 0, 0);
	assert(tick3 == tick3Cpy);
	
	Tick tick4 { 16, 2, 4 };
	Tick tick5 { 16, 2, 14 };
	tick4.increament(10);
	assert(tick4 == tick5);

	Tick tick6 { 23, 6, 24 };
	Tick tick7 { 23, 17, 47 };
	tick6.increament(11, 23);
	assert(tick6 == tick7);
}

