#pragma once
#include "test_header.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "object_manager.hpp"

TPF
void test_customer_ini()
{
	using namespace simulation;

	// 测试Customer的初始化
	Customer customer{Level::INT, 1.5, 30};
}

TPF
void test_customer_accessor(bool display = false)
{
	using namespace simulation;

	// 初始化Customer
	Customer customer{Level::ADV, 2.0, 45};

	// 测试访问器函数
	const auto& id = customer.get_id();
	auto shared_id = customer.get_shared();

	assert(customer.get_level() == Level::ADV);
	assert(customer.get_time_factor() == 2.0);
	assert(customer.get_max_waiting_time() == 45);
	assert(customer.haircut_started() == false);
	if (display) {
		std::println("Customer ID: {}", id.get_id_number());
		std::println("Shared Customer ID: {}", shared_id->get_id_number());
		std::println("Customer Level: {}", static_cast<int>(customer.get_level()));
		std::println("Customer Time Factor: {}", customer.get_time_factor());
		std::println("Customer Max Waiting Time: {}", customer.get_max_waiting_time());
		std::println("Haircut Started: {}", customer.haircut_started());
	}
}

TPF
void test_customer_modifier()
{
	using namespace simulation;

	// 初始化Customer
	Customer customer{Level::BEG, 1.0, 20};

	// 测试修改器函数
	assert(customer.haircut_started() == false);
	customer.start_cuting_hear();
	assert(customer.haircut_started() == true);
}

TPF 
void test_barber_ini()
{
	using namespace simulation;

	// 测试Barber的初始化
	Barber barber1{Level::INT, 1.5};
	Barber barber2{Level::ADV, 2.0};

	assert(barber1.get_level() == Level::INT);
	assert(barber1.get_time_factor() == 1.5);
	assert(barber1.busy() == false);

	assert(barber2.get_level() == Level::ADV);
	assert(barber2.get_time_factor() == 2.0);
	assert(barber2.busy() == false);
}

TPF
void test_barber_accessor(bool display = false)
{
	using namespace simulation;

	// 初始化Barber
	Barber barber{Level::BEG, 1.0};
	auto shared_id = barber.get_shared();

	// 测试访问器函数
	assert(barber.get_level() == Level::BEG);
	assert(barber.get_time_factor() == 1.0);
	assert(barber.busy() == false);

	if (display) {
		std::println("Barber ID: {}", barber.get_id().get_id_number());
		std::println("Shared Barber ID: {}", shared_id->get_id_number());
		std::println("Barber Level: {}", static_cast<int>(barber.get_level()));
		std::println("Barber Time Factor: {}", barber.get_time_factor());
		std::println("Barber Busy: {}", barber.busy());
	}
}

TPF
void test_barber_modifier(bool display = false)
{
	using namespace simulation;
	using namespace std;

	// 初始化Barber和Customer
	Barber barber{Level::BEG, 1.0};
	Customer customer{Level::BEG, 1.0, 20};

	auto customer_id = customer.get_shared();
	barber.set_customer_id(customer_id);

	// 测试修改器函数
	assert(barber.busy() == true);
	assert(barber.get_customer_id() == *customer_id);

	if (display) {
		std::println("Barber is busy with customer ID: {}", barber.get_customer_id().get_id_number());
	}

	barber.release_customer();
	assert(barber.busy() == false);

	if (display) {
		std::println("Barber released the customer and is now busy: {}", barber.busy());
	}
}

TPF 
void test_chair_ini()
{
	using namespace simulation;

	// 测试Chair的初始化
	Chair chair1{};
	assert(chair1.busy() == false);

	// 初始化Customer和Barber
	Customer customer{Level::INT, 1.5, 30};
	Barber barber{Level::INT, 1.5};

	auto customer_id = customer.get_shared();
	auto barber_id = barber.get_shared();

	Chair chair2{customer_id, barber_id};
	assert(chair2.busy() == true);
	assert(chair2.get_customer_id() == *customer_id);
	assert(chair2.get_barber_id() == *barber_id);
}

TPF
void test_chair_accessor(bool display = false)
{
	using namespace simulation;

	// 初始化Customer和Barber
	Customer customer{Level::ADV, 2.0, 45};
	Barber barber{Level::ADV, 2.0};

	auto customer_id = customer.get_shared();
	auto barber_id = barber.get_shared();

	// 初始化Chair
	Chair chair{customer_id, barber_id};

	// 测试访问器函数
	assert(chair.busy() == true);
	assert(chair.get_customer_id() == *customer_id);
	assert(chair.get_barber_id() == *barber_id);

	if (display) {
		std::println("Chair ID: {}", chair.get_id().get_id_number());
		std::println("Chair is busy with Customer ID: {}", chair.get_customer_id().get_id_number());
		std::println("Chair is busy with Barber ID: {}", chair.get_barber_id().get_id_number());
	}
}

TPF
void test_chair_modifier(bool display = false)
{
	using namespace simulation;
	using namespace std;

	// 初始化Customer和Barber
	Customer customer1{Level::BEG, 1.0, 20};
	Barber barber1{Level::BEG, 1.0};

	auto customer1_id = customer1.get_shared();
	auto barber1_id = barber1.get_shared();

	Customer customer2{Level::INT, 1.5, 30};
	Barber barber2{Level::INT, 1.5};

	auto customer2_id = customer2.get_shared();
	auto barber2_id = barber2.get_shared();

	// 初始化Chair
	Chair chair{};
	assert(chair.busy() == false);

	// 设置Customer和Barber
	chair.set_customer_barber(customer1_id, barber1_id);
	assert(chair.busy() == true);
	assert(chair.get_customer_id() == *customer1_id);
	assert(chair.get_barber_id() == *barber1_id);

	if (display) {
		std::println("Chair is busy with Customer ID: {}", chair.get_customer_id().get_id_number());
		std::println("Chair is busy with Barber ID: {}", chair.get_barber_id().get_id_number());
	}

	// 释放Customer和Barber
	chair.release();
	assert(chair.busy() == false);

	if (display) {
		std::println("Chair released the Customer and Barber. Now busy: {}", chair.busy());
	}

	// 重新设置Customer和Barber
	chair.set_customer_barber(customer2_id, barber2_id);
	assert(chair.busy() == true);
	assert(chair.get_customer_id() == *customer2_id);
	assert(chair.get_barber_id() == *barber2_id);

	if (display) {
		std::println("Chair is busy with new Customer ID: {}", chair.get_customer_id().get_id_number());
		std::println("Chair is busy with new Barber ID: {}", chair.get_barber_id().get_id_number());
	}
}

TPF
void test_barber_manager_ini()
{
	using namespace simulation;
	using SimulationManager = ObjectManager<Customer, Barber, Chair, A>;
	SimulationManager objManager;
	BarberManager<SimulationManager> barberManager { objManager };
	barberManager.get_free_barber(Level::BEG);
	try
	{
		barberManager.free_barber(Id<Barber>{});
	}
	catch(...)
	{
	}
}

TPF 
void test_barber_manager_get_free_barber()
{
	using namespace simulation;
	using M2 = ObjectManager<Customer, Barber, Chair, B>;
	M2 objManager;
	 // 创建Customer和Barber对象
    auto customer1 = std::make_shared<Customer>(Level::INT, 1.5, 30);
    auto customer2 = std::make_shared<Customer>(Level::ADV, 2.0, 45);

    auto barber1 = std::make_shared<Barber>(Level::INT, 1.5, customer1->get_shared());
    auto barber2 = std::make_shared<Barber>(Level::ADV, 2.0, customer2->get_shared());
    auto barber3 = std::make_shared<Barber>(Level::BEG, 1.0);

    // 获取Id
    const auto& customer1Id = customer1->get_id();
    const auto& customer2Id = customer2->get_id();
    const auto& barber1Id = barber1->get_id();
    const auto& barber2Id = barber2->get_id();
    const auto& barber3Id = barber3->get_id();

    // 注册Customer和Barber对象到ObjectManager中
    objManager.register_obj(customer1Id, customer1);
    objManager.register_obj(customer2Id, customer2);
    objManager.register_obj(barber1Id, barber1);
    objManager.register_obj(barber2Id, barber2);
    objManager.register_obj(barber3Id, barber3);

	BarberManager<M2> barberManager { objManager };
	auto pBarber1 { barberManager.get_free_barber(Level::INT) };
	assert(pBarber1 == nullptr);
	auto pBarber2 { barberManager.get_free_barber(Level::ADV) };
	assert(pBarber2 == nullptr);
	auto pBarber3 { barberManager.get_free_barber(Level::BEG) };
	assert(pBarber3 != nullptr);
	assert(pBarber3 == barber3);
}

TPF
void test_barber_set_free()
{
	using namespace simulation;
	using M3 = ObjectManager<Customer, Barber, Chair, C>;
	M3 objManager;
	 // 创建Customer和Barber对象
    auto customer1 = std::make_shared<Customer>(Level::INT, 1.5, 30);
    auto customer2 = std::make_shared<Customer>(Level::ADV, 2.0, 45);

    auto barber1 = std::make_shared<Barber>(Level::INT, 1.5, customer1->get_shared());
    auto barber2 = std::make_shared<Barber>(Level::ADV, 2.0, customer2->get_shared());
    auto barber3 = std::make_shared<Barber>(Level::BEG, 1.0);

    // 获取Id
    const auto& customer1Id = customer1->get_id();
    const auto& customer2Id = customer2->get_id();
    const auto& barber1Id = barber1->get_id();
    const auto& barber2Id = barber2->get_id();
    const auto& barber3Id = barber3->get_id();

    // 注册Customer和Barber对象到ObjectManager中
    objManager.register_obj(customer1Id, customer1);
    objManager.register_obj(customer2Id, customer2);
    objManager.register_obj(barber1Id, barber1);
    objManager.register_obj(barber2Id, barber2);
    objManager.register_obj(barber3Id, barber3);

	BarberManager<M3> barberManager { objManager };
	barberManager.free_barber(barber1Id);
	barberManager.free_barber(barber2Id);
	auto pBarber1 { barberManager.get_free_barber(Level::INT) };
	assert(pBarber1 != nullptr);
	assert(pBarber1 == barber1);
	auto pBarber2 { barberManager.get_free_barber(Level::ADV) };
	assert(pBarber2 != nullptr);
	assert(pBarber2 == barber2);
	auto pBarber3 { barberManager.get_free_barber(Level::BEG) };
	assert(pBarber3 != nullptr);
	assert(pBarber3 == barber3);
}
