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
	assert(barber.get_total_worktime() == 0);

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

	barber.increase_worktime(2.5);
    assert(barber.get_total_worktime() == 2.5);

    // 再次增加工作时间
    barber.increase_worktime(1.5);
    assert(barber.get_total_worktime() == 4.0);

    // 测试增加 0 时间
    barber.increase_worktime(0.0);
    assert(barber.get_total_worktime() == 4.0);
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

TPF
void test_chair_manager_ini()
{
	using namespace simulation;

    // 使用ObjectManager进行实例化
    using M1 = ObjectManager<Customer, Barber, Chair, ChairTestA>;
    ChairManager<M1> chairManager(M1{});

    // 初始化ObjectManager
    M1 manager;

    // 创建Chair对象
    auto chair1 = std::make_shared<Chair>();
    auto chair2 = std::make_shared<Chair>();

    // 获取Id
    [[maybe_unused]] const auto& c1 = chair1->get_id();
    [[maybe_unused]] const auto& c2 = chair2->get_id();
}

TPF
void test_chair_manager_get_free()
{
	using namespace simulation;

    // 使用ObjectManager进行实例化
    using M2 = ObjectManager<Customer, Barber, Chair, ChairTestB>;
    M2 manager;

    // 创建Customer和Barber对象
    auto customer1 = std::make_shared<Customer>(Level::INT, 1.5, 30);
    auto customer2 = std::make_shared<Customer>(Level::ADV, 2.0, 45);
    auto barber1 = std::make_shared<Barber>(Level::INT, 1.5);
    auto barber2 = std::make_shared<Barber>(Level::ADV, 2.0);

    // 获取Id
    const auto& customer1Id = customer1->get_id();
    const auto& customer2Id = customer2->get_id();
    const auto& barber1Id = barber1->get_id();
    const auto& barber2Id = barber2->get_id();

    // 注册Customer和Barber对象到ObjectManager中
    manager.register_obj(customer1Id, customer1);
    manager.register_obj(customer2Id, customer2);
    manager.register_obj(barber1Id, barber1);
    manager.register_obj(barber2Id, barber2);

    // 创建Chair对象
    auto chair1 = std::make_shared<Chair>(customer1->get_shared(), barber1->get_shared());
    auto chair2 = std::make_shared<Chair>(customer2->get_shared(), barber2->get_shared());
    auto chair3 = std::make_shared<Chair>();
    auto chair4 = std::make_shared<Chair>();
    auto chair5 = std::make_shared<Chair>(customer1->get_shared(), barber2->get_shared());
    auto chair6 = std::make_shared<Chair>();
    auto chair7 = std::make_shared<Chair>();
    auto chair8 = std::make_shared<Chair>(customer2->get_shared(), barber1->get_shared());

    // 获取Id
    const auto& chair1Id = chair1->get_id();
    const auto& chair2Id = chair2->get_id();
    const auto& chair3Id = chair3->get_id();
    const auto& chair4Id = chair4->get_id();
    const auto& chair5Id = chair5->get_id();
    const auto& chair6Id = chair6->get_id();
    const auto& chair7Id = chair7->get_id();
    const auto& chair8Id = chair8->get_id();

    // 注册Chair对象到ObjectManager中
    manager.register_obj(chair1Id, chair1);
    manager.register_obj(chair2Id, chair2);
    manager.register_obj(chair3Id, chair3);
    manager.register_obj(chair4Id, chair4);
    manager.register_obj(chair5Id, chair5);
    manager.register_obj(chair6Id, chair6);
    manager.register_obj(chair7Id, chair7);
    manager.register_obj(chair8Id, chair8);

    // 初始化ChairManager
    ChairManager<M2> chairManager(manager);

    // 获取空闲的Chair对象
    auto freeChair1 = chairManager.get_free_chair();
    assert(freeChair1 == chair3 || freeChair1 == chair4 || freeChair1 == chair6 || freeChair1 == chair7);

    auto freeChair2 = chairManager.get_free_chair();
    assert(freeChair2 == chair3 || freeChair2 == chair4 || freeChair2 == chair6 || freeChair2 == chair7);
    assert(freeChair2 != freeChair1);

    auto freeChair3 = chairManager.get_free_chair();
    assert(freeChair3 == chair3 || freeChair3 == chair4 || freeChair3 == chair6 || freeChair3 == chair7);
    assert(freeChair3 != freeChair1);
    assert(freeChair3 != freeChair2);

    auto freeChair4 = chairManager.get_free_chair();
    assert(freeChair4 == chair3 || freeChair4 == chair4 || freeChair4 == chair6 || freeChair4 == chair7);
    assert(freeChair4 != freeChair1);
    assert(freeChair4 != freeChair2);
    assert(freeChair4 != freeChair3);

    // 确保没有空闲的Chair对象
    auto freeChair5 = chairManager.get_free_chair();
    assert(freeChair5 == nullptr);
}

TPF
void test_chair_manager_set_free()
{
    using namespace simulation;

    // 使用ObjectManager进行实例化
    using M3 = ObjectManager<Customer, Barber, Chair, ChairTestC>;
    M3 manager;

    // 创建Customer和Barber对象
    auto customer1 = std::make_shared<Customer>(Level::INT, 1.5, 30);
    auto customer2 = std::make_shared<Customer>(Level::ADV, 2.0, 45);
    auto barber1 = std::make_shared<Barber>(Level::INT, 1.5);
    auto barber2 = std::make_shared<Barber>(Level::ADV, 2.0);

    // 获取Id
    const auto& customer1Id = customer1->get_id();
    const auto& customer2Id = customer2->get_id();
    const auto& barber1Id = barber1->get_id();
    const auto& barber2Id = barber2->get_id();

    // 注册Customer和Barber对象到ObjectManager中
    manager.register_obj(customer1Id, customer1);
    manager.register_obj(customer2Id, customer2);
    manager.register_obj(barber1Id, barber1);
    manager.register_obj(barber2Id, barber2);

    // 创建Chair对象
    auto chair1 = std::make_shared<Chair>(customer1->get_shared(), barber1->get_shared());
    auto chair2 = std::make_shared<Chair>(customer2->get_shared(), barber2->get_shared());
    auto chair3 = std::make_shared<Chair>();
    auto chair4 = std::make_shared<Chair>();
    auto chair5 = std::make_shared<Chair>(customer1->get_shared(), barber2->get_shared());
    auto chair6 = std::make_shared<Chair>();
    auto chair7 = std::make_shared<Chair>();
    auto chair8 = std::make_shared<Chair>(customer2->get_shared(), barber1->get_shared());
	//1, 2, 5, 8为busy
	
    // 获取Id
    const auto& chair1Id = chair1->get_id();
    const auto& chair2Id = chair2->get_id();
    const auto& chair3Id = chair3->get_id();
    const auto& chair4Id = chair4->get_id();
    const auto& chair5Id = chair5->get_id();
    const auto& chair6Id = chair6->get_id();
    const auto& chair7Id = chair7->get_id();
    const auto& chair8Id = chair8->get_id();

    // 注册Chair对象到ObjectManager中
    manager.register_obj(chair1Id, chair1);
    manager.register_obj(chair2Id, chair2);
    manager.register_obj(chair3Id, chair3);
    manager.register_obj(chair4Id, chair4);
    manager.register_obj(chair5Id, chair5);
    manager.register_obj(chair6Id, chair6);
    manager.register_obj(chair7Id, chair7);
    manager.register_obj(chair8Id, chair8);

    // 初始化ChairManager
    ChairManager<M3> chairManager(manager);

    // 将一些Chair对象添加到空闲集合中
    chairManager.free_chair(chair1Id);
    chairManager.free_chair(chair2Id);
    chairManager.free_chair(chair5Id);
    chairManager.free_chair(chair8Id);


    // 检查是否成功标记为空闲
   
	auto freeChair1 = chairManager.get_free_chair();
	assert(
	    freeChair1 == chair1 ||
	    freeChair1 == chair2 ||
	    freeChair1 == chair3 ||
	    freeChair1 == chair4 ||
	    freeChair1 == chair5 ||
	    freeChair1 == chair6 ||
	    freeChair1 == chair7 ||
	    freeChair1 == chair8
	);
	
	auto freeChair2 = chairManager.get_free_chair();
	assert(
	    freeChair2 == chair1 ||
	    freeChair2 == chair2 ||
	    freeChair2 == chair3 ||
	    freeChair2 == chair4 ||
	    freeChair2 == chair5 ||
	    freeChair2 == chair6 ||
	    freeChair2 == chair7 ||
	    freeChair2 == chair8
	);
	assert(freeChair2 != freeChair1);
	
	auto freeChair3 = chairManager.get_free_chair();
	assert(
	    freeChair3 == chair1 ||
	    freeChair3 == chair2 ||
	    freeChair3 == chair3 ||
	    freeChair3 == chair4 ||
	    freeChair3 == chair5 ||
	    freeChair3 == chair6 ||
	    freeChair3 == chair7 ||
	    freeChair3 == chair8
	);
	assert(freeChair3 != freeChair1 && freeChair3 != freeChair2);
	
	auto freeChair4 = chairManager.get_free_chair();
	assert(
	    freeChair4 == chair1 ||
	    freeChair4 == chair2 ||
	    freeChair4 == chair3 ||
	    freeChair4 == chair4 ||
	    freeChair4 == chair5 ||
	    freeChair4 == chair6 ||
	    freeChair4 == chair7 ||
	    freeChair4 == chair8
	);
	assert(freeChair4 != freeChair1 && freeChair4 != freeChair2 && freeChair4 != freeChair3);
	
	auto freeChair5 = chairManager.get_free_chair();
	assert(
	    freeChair5 == chair1 ||
	    freeChair5 == chair2 ||
	    freeChair5 == chair3 ||
	    freeChair5 == chair4 ||
	    freeChair5 == chair5 ||
	    freeChair5 == chair6 ||
	    freeChair5 == chair7 ||
	    freeChair5 == chair8
	);
	assert(freeChair5 != freeChair1 && freeChair5 != freeChair2 && freeChair5 != freeChair3 && freeChair5 != freeChair4);
	
	auto freeChair6 = chairManager.get_free_chair();
	assert(
	    freeChair6 == chair1 ||
	    freeChair6 == chair2 ||
	    freeChair6 == chair3 ||
	    freeChair6 == chair4 ||
	    freeChair6 == chair5 ||
	    freeChair6 == chair6 ||
	    freeChair6 == chair7 ||
	    freeChair6 == chair8
	);
	assert(freeChair6 != freeChair1 && freeChair6 != freeChair2 && freeChair6 != freeChair3 && freeChair6 != freeChair4 && freeChair6 != freeChair5);
	
	auto freeChair7 = chairManager.get_free_chair();
	assert(
	    freeChair7 == chair1 ||
	    freeChair7 == chair2 ||
	    freeChair7 == chair3 ||
	    freeChair7 == chair4 ||
	    freeChair7 == chair5 ||
	    freeChair7 == chair6 ||
	    freeChair7 == chair7 ||
	    freeChair7 == chair8
	);
	assert(freeChair7 != freeChair1 && freeChair7 != freeChair2 && freeChair7 != freeChair3 && freeChair7 != freeChair4 && freeChair7 != freeChair5 && freeChair7 != freeChair6);
	
	auto freeChair8 = chairManager.get_free_chair();
	assert(
	    freeChair8 == chair1 ||
	    freeChair8 == chair2 ||
	    freeChair8 == chair3 ||
	    freeChair8 == chair4 ||
	    freeChair8 == chair5 ||
	    freeChair8 == chair6 ||
	    freeChair8 == chair7 ||
	    freeChair8 == chair8
	);
	assert(freeChair8 != freeChair1 && freeChair8 != freeChair2 && freeChair8 != freeChair3 && freeChair8 != freeChair4 && freeChair8 != freeChair5 && freeChair8 != freeChair6 && freeChair8 != freeChair7);

	auto freeChair9 = chairManager.get_free_chair();
	assert(freeChair9 == nullptr);
}

TPF
void test_cwq_ini()
{
	using namespace simulation;
	using namespace std;
	using M1 = ObjectManager<Customer, Barber, Chair, CustTestA>;
    M1 manager;

    // 初始化CustomerWaitingQue
    CustomerWaitingQue<M1> customerWaitingQue(manager);

    // 创建一个Customer对象
    auto customer = make_shared<Customer>(Level::BEG, 1.0, 30);
    const Id<Customer>& customerId = customer->get_id();
	manager.register_obj(customerId, customer);

    // 将Customer对象推入队列
    customerWaitingQue.push(customerId);

    // 从队列中弹出Customer对象
    const Id<Customer>& _ = customerWaitingQue.pop(Level::BEG);
}

TPF
void test_cwq_push()
{
	using namespace simulation;
	using namespace std;

    // 使用ObjectManager进行实例化
    using M2 = ObjectManager<Customer, Barber, Chair, CustTestB>;
    M2 manager;

    // 创建Customer对象并注册到manager中
    auto customer1 = std::make_shared<Customer>(Level::BEG, 1.0, 30);
    auto customer2 = std::make_shared<Customer>(Level::INT, 1.5, 45);
    auto customer3 = std::make_shared<Customer>(Level::ADV, 2.0, 60);
    const Id<Customer>& customerId1 = customer1->get_id();
    const Id<Customer>& customerId2 = customer2->get_id();
    const Id<Customer>& customerId3 = customer3->get_id();
    manager.register_obj(customerId1, customer1);
    manager.register_obj(customerId2, customer2);
    manager.register_obj(customerId3, customer3);

    // 初始化CustomerWaitingQue
    CustomerWaitingQue<M2> customerWaitingQue(manager);

    // 将Customer对象推入队列
    customerWaitingQue.push(customerId1);
    customerWaitingQue.push(customerId2);
    customerWaitingQue.push(customerId3);

    // 检查队列大小是否更新
    assert(customerWaitingQue.get_que_size(Level::BEG) == 1);
    assert(customerWaitingQue.get_que_size(Level::INT) == 1);
    assert(customerWaitingQue.get_que_size(Level::ADV) == 1);

    // 从队列中弹出并检查顺序
    const Id<Customer>& poppedCustomerId1 = customerWaitingQue.pop(Level::BEG);
    const Id<Customer>& poppedCustomerId2 = customerWaitingQue.pop(Level::INT);
    const Id<Customer>& poppedCustomerId3 = customerWaitingQue.pop(Level::ADV);
    assert(&poppedCustomerId1 == &customerId1);
    assert(&poppedCustomerId2 == &customerId2);
    assert(&poppedCustomerId3 == &customerId3);

    // 检查弹出后队列大小
    assert(customerWaitingQue.get_que_size(Level::BEG) == 0);
    assert(customerWaitingQue.get_que_size(Level::INT) == 0);
    assert(customerWaitingQue.get_que_size(Level::ADV) == 0);
}

TPF
void test_cwq_pop()
{
    using namespace simulation;

    // 使用ObjectManager进行实例化
    using M2 = ObjectManager<Customer, Barber, Chair, CustTestB>;
    M2 manager;

    // 创建多个Customer对象并注册到manager中
    auto customer1 = std::make_shared<Customer>(Level::BEG, 1.0, 30);
    auto customer2 = std::make_shared<Customer>(Level::INT, 1.5, 45);
    auto customer3 = std::make_shared<Customer>(Level::ADV, 2.0, 60);
    auto customer4 = std::make_shared<Customer>(Level::BEG, 1.1, 35);
    auto customer5 = std::make_shared<Customer>(Level::INT, 1.6, 50);
    auto customer6 = std::make_shared<Customer>(Level::ADV, 2.1, 65);
    
    const Id<Customer>& customerId1 = customer1->get_id();
    const Id<Customer>& customerId2 = customer2->get_id();
    const Id<Customer>& customerId3 = customer3->get_id();
    const Id<Customer>& customerId4 = customer4->get_id();
    const Id<Customer>& customerId5 = customer5->get_id();
    const Id<Customer>& customerId6 = customer6->get_id();
    
    manager.register_obj(customerId1, customer1);
    manager.register_obj(customerId2, customer2);
    manager.register_obj(customerId3, customer3);
    manager.register_obj(customerId4, customer4);
    manager.register_obj(customerId5, customer5);
    manager.register_obj(customerId6, customer6);

    // 初始化CustomerWaitingQue
    CustomerWaitingQue<M2> customerWaitingQue(manager);

    // 将Customer对象推入队列
    customerWaitingQue.push(customerId1);
    customerWaitingQue.push(customerId2);
    customerWaitingQue.push(customerId3);
    customerWaitingQue.push(customerId4);
    customerWaitingQue.push(customerId5);
    customerWaitingQue.push(customerId6);

    // 检查队列大小是否更新
    assert(customerWaitingQue.get_que_size(Level::BEG) == 2);
    assert(customerWaitingQue.get_que_size(Level::INT) == 2);
    assert(customerWaitingQue.get_que_size(Level::ADV) == 2);

    // 从队列中弹出并检查顺序
    const Id<Customer>& poppedCustomerId1 = customerWaitingQue.pop(Level::BEG);
    const Id<Customer>& poppedCustomerId2 = customerWaitingQue.pop(Level::INT);
    const Id<Customer>& poppedCustomerId3 = customerWaitingQue.pop(Level::ADV);
    assert(&poppedCustomerId1 == &customerId1);
    assert(&poppedCustomerId2 == &customerId2);
    assert(&poppedCustomerId3 == &customerId3);

    // 检查弹出后队列大小
    assert(customerWaitingQue.get_que_size(Level::BEG) == 1);
    assert(customerWaitingQue.get_que_size(Level::INT) == 1);
    assert(customerWaitingQue.get_que_size(Level::ADV) == 1);

    // 继续从队列中弹出剩余的Customer对象
    const Id<Customer>& poppedCustomerId4 = customerWaitingQue.pop(Level::BEG);
    const Id<Customer>& poppedCustomerId5 = customerWaitingQue.pop(Level::INT);
    const Id<Customer>& poppedCustomerId6 = customerWaitingQue.pop(Level::ADV);
    assert(&poppedCustomerId4 == &customerId4);
    assert(&poppedCustomerId5 == &customerId5);
    assert(&poppedCustomerId6 == &customerId6);

    // 检查所有队列是否为空
    assert(customerWaitingQue.get_que_size(Level::BEG) == 0);
    assert(customerWaitingQue.get_que_size(Level::INT) == 0);
    assert(customerWaitingQue.get_que_size(Level::ADV) == 0);
}
