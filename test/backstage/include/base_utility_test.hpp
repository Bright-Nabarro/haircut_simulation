#pragma once
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "base_utility.hpp"
#include "exception.hpp"
#include "test_header.hpp"


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

struct S1
{
};

struct S2
{
};

TPF
void test_Id_ini()
{
	using namespace simulation;
	Id<S1> ids1_1;
	Id<S1> ids1_2;
	Id<S2> ids2_1;
	Id<S2> ids2_2;
	Id<int> i1;
	Id<void> i2;
}

TPF
void test_Id_get_number(bool display = false)
{
	struct A
	{
	};
	
	struct B
	{
	};

	using namespace simulation;
	Id<A> idA1;
	Id<A> idA2;
	Id<B> idB1;
	Id<B> idB2;
	if (display)
	{
		std::println("idA1 number: {}", idA1.get_id_number());
		std::println("idA2 number: {}", idA2.get_id_number());
		std::println("idB1 number: {}", idB1.get_id_number());
		std::println("idB2 number: {}", idB2.get_id_number());
	}
	assert(idA1.get_id_number() == 0);
	assert(idA2.get_id_number() == 1);
	assert(idB1.get_id_number() == 0);
	assert(idB2.get_id_number() == 1);

	{
		Id<A> idA3;
		assert(idA3.get_id_number() == 2);
	}

	Id<A> idA4;
	assert(idA4.get_id_number() == 3);
}


TPF
void test_Id_cpr()
{
	struct A
	{
	};
	
	struct B
	{
	};

	using namespace simulation;
	Id<A> idA1;
	Id<A> idA2;
	Id<B> idB1;
	
	const auto& idA1Ref { idA1 };
	assert(idA1Ref == idA1);
	//idA1 != idB1;
	assert(idA1 != idA2);
	assert(idA1Ref != idA2);
	assert(idA1 < idA2);
	assert(idA1 <= idA2);
	assert(idA2 > idA1);
	assert(idA2 >= idA1);
}

TPF
void test_Id_ownership(bool display = false)
{
	using namespace simulation;
	using namespace std;
	struct A{};
	Id<A> idA1;
	auto idR1 { cref(idA1) };
	assert(idR1.get() == idA1);
	assert(idR1 == idA1);
	Id<A> idA2;
	idR1 = cref(idA2);
	assert(idR1 != idA1);
	assert(idR1 == idA2);

	optional<reference_wrapper<Id<A>>> optIdR1;
	optIdR1.emplace(idA1);
	assert(optIdR1.value() == idA1);

	struct B{};
	Id<B> idB1{};
	Id<B> idB2{};
	Id<B> idB3{};
	unordered_set<reference_wrapper<const Id<B>>> bset;
	bset.insert(ref(idB1));
	bset.insert(ref(idB2));
	bset.insert(ref(idB3));
	for(const auto& x : bset)
	{
		if(display)
			println("{}", x.get().get_id_number());
	}

	struct C{};
	struct hashRef
	{
		auto operator()(const reference_wrapper<const Id<C>>& rhs) const
		{
			return hash<size_t>{}(rhs.get().get_id_number());
		}
	};
	struct equalRef
	{
		bool operator()(const reference_wrapper<const Id<C>>& lhs,
						const reference_wrapper<const Id<C>>& rhs) const
		{
			return lhs.get().get_id_number() == rhs.get().get_id_number();
		}
	};
	vector<Id<C>> vecIdC;
	unordered_set<reference_wrapper<const Id<C>>> cset;
	for (size_t i{0}; i < 3; i++)
	{
		vecIdC.emplace_back();
	}
	for (const auto& x : vecIdC)
	{
		cset.insert(ref(x));
	}
}

TPF
void test_hash_equal_Id(bool display = false)
{
	using namespace simulation;
	using namespace std;

	struct A {};
	Id<A> idA1;
	Id<A> idA2;
	Id<A> idA3;
	Id<A> idA4;
	if (display)
	{
		std::println("{}", std::hash<decltype(idA1)>{}(idA1)); 
		std::println("{}", std::hash<decltype(idA1)>{}(idA2)); 
		std::println("{}", std::hash<decltype(idA1)>{}(idA3)); 
		std::println("{}", std::hash<decltype(idA1)>{}(idA4)); 
	}
	
	auto IdAR1 { cref(idA1) };
	auto IdAR2 { cref(idA1) };
	assert(IdAR1 == IdAR2);

	struct B {};

	vector<Id<B>> idBList;
	unordered_set<reference_wrapper<const Id<B>>> idBSet;

	unordered_set<Id<B>*> pSet;
	
	for (size_t i{0}; i < 100; i++)
	{
		idBList.emplace_back();
	}
	size_t listSum{0}, setSum{0};
	for (const auto& x : idBList)
	{
		idBSet.insert(cref(x));
		listSum += x.get_id_number();
	}
	
	for (const auto& x : idBSet)
	{
		if (display)
			println("set idB {}", x.get().get_id_number());
		setSum += x.get().get_id_number();
	}
	for (const auto& x : pSet)
	{
		if (display)
			println("setp idB {}", x->get_id_number());
	}

	if (display)
		println("set size : {}", idBSet.size());

	assert(listSum == setSum);
	
}
