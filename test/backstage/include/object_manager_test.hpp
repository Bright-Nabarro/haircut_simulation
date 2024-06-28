#pragma once
#include <memory>
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "test_header.hpp"
#include "object_manager.hpp"

TPF
void test_om_ini()
{
	using namespace simulation;
	using namespace std;

	// 初始化ObjectManager
	ObjectManager<int, double> m1;

	// 创建Id和对象
	Id<int> idIntA{};
	shared_ptr<int> a = make_shared<int>(1);

	m1.register_obj<int>(idIntA, a);
	m1.get_obj<int>(idIntA);
	m1.remove_obj<int>(idIntA);
	m1.contains<int>(idIntA);
}

TPF
void test_om_register()
{
	using namespace simulation;
	using namespace std;

	// 初始化ObjectManager
	ObjectManager<Customer, Barber, Chair> manager;

	// 创建Customer, Barber, 和 Chair对象
	auto customer = make_shared<Customer>(Level::INT, 1.5, 30);
	auto barber = make_shared<Barber>(Level::INT, 1.5);
	auto chair = make_shared<Chair>(customer->get_shared(), barber->get_shared());

	// 获取Id
	const auto& customerId = customer->get_id();
	const auto& barberId = barber->get_id();
	const auto& chairId = chair->get_id();

	// 注册对象
	manager.register_obj(customerId, customer);
	manager.register_obj(barberId, barber);
	manager.register_obj(chairId, chair);

	// 检查是否成功注册
	assert(manager.contains(customerId));
	assert(manager.contains(barberId));
	assert(manager.contains(chairId));
}

TPF
void test_om_get()
{
	using namespace simulation;
	using namespace std;

	// 初始化ObjectManager
	ObjectManager<Customer, Barber, Chair> manager;

	// 创建多个Customer, Barber, 和 Chair对象
	auto customer1 = make_shared<Customer>(Level::INT, 1.5, 30);
	auto barber1 = make_shared<Barber>(Level::INT, 1.5);
	auto chair1 = make_shared<Chair>(customer1->get_shared(), barber1->get_shared());

	auto customer2 = make_shared<Customer>(Level::ADV, 2.0, 45);
	auto barber2 = make_shared<Barber>(Level::ADV, 2.0);
	auto chair2 = make_shared<Chair>(customer2->get_shared(), barber2->get_shared());

	auto customer3 = make_shared<Customer>(Level::BEG, 1.0, 20);
	auto barber3 = make_shared<Barber>(Level::BEG, 1.0);
	auto chair3 = make_shared<Chair>(customer3->get_shared(), barber3->get_shared());

	// 获取Id
	const auto& customer1Id = customer1->get_id();
	const auto& barber1Id = barber1->get_id();
	const auto& chair1Id = chair1->get_id();

	const auto& customer2Id = customer2->get_id();
	const auto& barber2Id = barber2->get_id();
	const auto& chair2Id = chair2->get_id();

	const auto& customer3Id = customer3->get_id();
	const auto& barber3Id = barber3->get_id();
	const auto& chair3Id = chair3->get_id();

	// 注册对象
	manager.register_obj(customer1Id, customer1);
	manager.register_obj(barber1Id, barber1);
	manager.register_obj(chair1Id, chair1);

	manager.register_obj(customer2Id, customer2);
	manager.register_obj(barber2Id, barber2);
	manager.register_obj(chair2Id, chair2);

	manager.register_obj(customer3Id, customer3);
	manager.register_obj(barber3Id, barber3);
	manager.register_obj(chair3Id, chair3);

	// 检查是否成功获取
	assert(manager.get_obj(customer1Id) == customer1);
	assert(manager.get_obj(barber1Id) == barber1);
	assert(manager.get_obj(chair1Id) == chair1);

	assert(manager.get_obj(customer2Id) == customer2);
	assert(manager.get_obj(barber2Id) == barber2);
	assert(manager.get_obj(chair2Id) == chair2);

	assert(manager.get_obj(customer3Id) == customer3);
	assert(manager.get_obj(barber3Id) == barber3);
	assert(manager.get_obj(chair3Id) == chair3);
}

TPF
void test_om_remove()
{
	using namespace simulation;
	using namespace std;

	// 初始化ObjectManager
	ObjectManager<Customer, Barber, Chair> manager;

	// 创建多个Customer, Barber, 和 Chair对象
	auto customer1 = make_shared<Customer>(Level::INT, 1.5, 30);
	auto barber1 = make_shared<Barber>(Level::INT, 1.5);
	auto chair1 = make_shared<Chair>(customer1->get_shared(), barber1->get_shared());

	auto customer2 = make_shared<Customer>(Level::ADV, 2.0, 45);
	auto barber2 = make_shared<Barber>(Level::ADV, 2.0);
	auto chair2 = make_shared<Chair>(customer2->get_shared(), barber2->get_shared());

	auto customer3 = make_shared<Customer>(Level::BEG, 1.0, 20);
	auto barber3 = make_shared<Barber>(Level::BEG, 1.0);
	auto chair3 = make_shared<Chair>(customer3->get_shared(), barber3->get_shared());

	// 获取Id
	const auto& customer1Id = customer1->get_id();
	const auto& barber1Id = barber1->get_id();
	const auto& chair1Id = chair1->get_id();

	const auto& customer2Id = customer2->get_id();
	const auto& barber2Id = barber2->get_id();
	const auto& chair2Id = chair2->get_id();

	const auto& customer3Id = customer3->get_id();
	const auto& barber3Id = barber3->get_id();
	const auto& chair3Id = chair3->get_id();

	// 注册对象
	manager.register_obj(customer1Id, customer1);
	manager.register_obj(barber1Id, barber1);
	manager.register_obj(chair1Id, chair1);

	manager.register_obj(customer2Id, customer2);
	manager.register_obj(barber2Id, barber2);
	manager.register_obj(chair2Id, chair2);

	manager.register_obj(customer3Id, customer3);
	manager.register_obj(barber3Id, barber3);
	manager.register_obj(chair3Id, chair3);

	// 移除对象并检查是否成功移除
	manager.remove_obj(customer1Id);
	manager.remove_obj(barber1Id);
	manager.remove_obj(chair1Id);

	manager.remove_obj(customer2Id);
	manager.remove_obj(barber2Id);
	manager.remove_obj(chair2Id);

	manager.remove_obj(customer3Id);
	manager.remove_obj(barber3Id);
	manager.remove_obj(chair3Id);

	// 检查是否成功移除
	assert(!manager.contains(customer1Id));
	assert(!manager.contains(barber1Id));
	assert(!manager.contains(chair1Id));

	assert(!manager.contains(customer2Id));
	assert(!manager.contains(barber2Id));
	assert(!manager.contains(chair2Id));

	assert(!manager.contains(customer3Id));
	assert(!manager.contains(barber3Id));
	assert(!manager.contains(chair3Id));
}

TPF
void test_om_complex()
{
	using namespace simulation;
	using namespace std;

	// 初始化ObjectManager
	ObjectManager<Customer, Barber, Chair> manager;

	// 创建和注册大量的Customer, Barber, 和 Chair对象
	const size_t num_objects = 1000;
	vector<shared_ptr<Customer>> customers;
	vector<shared_ptr<Barber>> barbers;
	vector<shared_ptr<Chair>> chairs;

	for (size_t i = 0; i < num_objects; ++i) {
		auto customer = make_shared<Customer>(Level::INT, 1.5, 30);
		auto barber = make_shared<Barber>(Level::INT, 1.5);
		auto chair = make_shared<Chair>(customer->get_shared(), barber->get_shared());

		customers.push_back(customer);
		barbers.push_back(barber);
		chairs.push_back(chair);

		manager.register_obj(customer->get_id(), customer);
		manager.register_obj(barber->get_id(), barber);
		manager.register_obj(chair->get_id(), chair);
	}

	// 检查是否成功注册
	for (size_t i = 0; i < num_objects; ++i) {
		assert(manager.contains(customers[i]->get_id()));
		assert(manager.contains(barbers[i]->get_id()));
		assert(manager.contains(chairs[i]->get_id()));
	}

	// 测试获取功能
	for (size_t i = 0; i < num_objects; ++i) {
		assert(manager.get_obj(customers[i]->get_id()) == customers[i]);
		assert(manager.get_obj(barbers[i]->get_id()) == barbers[i]);
		assert(manager.get_obj(chairs[i]->get_id()) == chairs[i]);
	}

	// 测试移除功能
	for (size_t i = 0; i < num_objects; ++i) {
		manager.remove_obj(customers[i]->get_id());
		manager.remove_obj(barbers[i]->get_id());
		manager.remove_obj(chairs[i]->get_id());

		assert(!manager.contains(customers[i]->get_id()));
		assert(!manager.contains(barbers[i]->get_id()));
		assert(!manager.contains(chairs[i]->get_id()));
	}

	// 检查是否成功移除
	for (size_t i = 0; i < num_objects; ++i) {
		try {
			manager.get_obj(customers[i]->get_id());
			assert(false && "Expected logic_error not thrown");
		} catch (const logic_error&) {
			assert(true);
		}

		try {
			manager.get_obj(barbers[i]->get_id());
			assert(false && "Expected logic_error not thrown");
		} catch (const logic_error&) {
			assert(true);
		}

		try {
			manager.get_obj(chairs[i]->get_id());
			assert(false && "Expected logic_error not thrown");
		} catch (const logic_error&) {
			assert(true);
		}
	}
}

