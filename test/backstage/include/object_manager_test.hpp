#pragma once
#include <memory>
#include "test_header.hpp"
#include "object_manager.hpp"

TPF
void test_ini()
{
	using namespace simulation;
	using namespace std;
	ObjectManager<int, double> m1;
	Id<int> idIntA;
	shared_ptr<int> a = make_shared<int>(1);
	m1.register_obj<int>(idIntA, a);
}
