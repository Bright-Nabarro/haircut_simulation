#include "exception.hpp"
#include "test_header.hpp"
#include "factory.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"

TPF
void test_fac_ini()
{
	using namespace simulation;

	struct A
	{
		A(): m_id{} {}
		const auto& get_id() { return m_id; }
		Id<A> m_id;
	};

	ObjectManager<A> om;
	ObjFactory<A> factory { om };
	auto pa { factory.create_obj() };
}

TPF
void test_fac_create()
{
	using namespace simulation;

	// 初始化ObjectManager
	ObjectManager<Customer> manager;

	// 创建ObjFactory
	ObjFactory<Customer> factory(manager);

	// 使用工厂创建Customer对象
	auto customer1 = factory.create_obj(Level::INT, 1.5, 30);
	auto customer2 = factory.create_obj(Level::ADV, 2.0, 45);

	// 检查Customer对象的属性
	assert(customer1->get_level() == Level::INT);
	assert(customer1->get_time_factor() == 1.5);
	assert(customer1->get_max_waiting_time() == 30);
	assert(customer1->haircut_started() == false);

	assert(customer2->get_level() == Level::ADV);
	assert(customer2->get_time_factor() == 2.0);
	assert(customer2->get_max_waiting_time() == 45);
	assert(customer2->haircut_started() == false);

	// 检查Customer对象是否已注册到ObjectManager中
	assert(manager.contains(customer1->get_id()));
	assert(manager.contains(customer2->get_id()));

}

TPF
void test_fac_complex()
{
	using namespace simulation;

    // 初始化单个ObjectManager
	using Mgr = ObjectManager<Customer, Barber, Chair>;
    Mgr manager;

    // 创建ObjFactory
    ObjFactory<Customer, Mgr> customerFactory(manager);
    ObjFactory<Barber, Mgr> barberFactory(manager);
    ObjFactory<Chair, Mgr> chairFactory(manager);

    // 使用工厂创建Customer, Barber, 和 Chair对象
    auto customer1 = customerFactory.create_obj(Level::INT, 1.5, 30);
    auto barber1 = barberFactory.create_obj(Level::INT, 1.5);
    auto chair1 = chairFactory.create_obj(customer1->get_shared(), barber1->get_shared());

    auto customer2 = customerFactory.create_obj(Level::ADV, 2.0, 45);
    auto barber2 = barberFactory.create_obj(Level::ADV, 2.0);
    auto chair2 = chairFactory.create_obj(customer2->get_shared(), barber2->get_shared());

    // 检查Customer对象的属性
    assert(customer1->get_level() == Level::INT);
    assert(customer1->get_time_factor() == 1.5);
    assert(customer1->get_max_waiting_time() == 30);
    assert(customer1->haircut_started() == false);

    assert(customer2->get_level() == Level::ADV);
    assert(customer2->get_time_factor() == 2.0);
    assert(customer2->get_max_waiting_time() == 45);
    assert(customer2->haircut_started() == false);

    // 检查Barber对象的属性
    assert(barber1->get_level() == Level::INT);
    assert(barber1->get_time_factor() == 1.5);
    assert(barber1->busy() == false);

    assert(barber2->get_level() == Level::ADV);
    assert(barber2->get_time_factor() == 2.0);
    assert(barber2->busy() == false);

    // 检查Chair对象的属性
    assert(chair1->get_customer_id() == customer1->get_id());
    assert(chair1->get_barber_id() == barber1->get_id());
    assert(chair1->busy() == true);

    assert(chair2->get_customer_id() == customer2->get_id());
    assert(chair2->get_barber_id() == barber2->get_id());
    assert(chair2->busy() == true);

    // 检查对象是否已注册到对应的ObjectManager中
    assert(manager.contains(customer1->get_id()));
    assert(manager.contains(customer2->get_id()));

    assert(manager.contains(barber1->get_id()));
    assert(manager.contains(barber2->get_id()));

    assert(manager.contains(chair1->get_id()));
    assert(manager.contains(chair2->get_id()));
}

TPF
void test_spec_factory()
{
	using namespace simulation;
	using Mgr = ObjectManager<Customer, Barber, Chair>;
    Mgr manager;

    CustomerFactory customerFactory(manager);
    BarberFactory barberFactory(manager);
    ChairFactory chairFactory(manager);

    // 测试 CustomerFactory 异常
    try {
        auto _ = customerFactory.create_customer(Level::BEG, 1.5, 86400);
		assert(false);
    } catch (const InvalidMaxWaitingTime& e) {
    }	catch(...)
	{
		assert(false);
	}

    try {
        auto _ = customerFactory.create_customer(Level::BEG, -1.0, 3600);
		assert(false);
    } catch (const std::runtime_error& e) {
    }	catch(...)
	{
		assert(false);
	}

    // 测试 BarberFactory 异常
    try {
        auto _ = barberFactory.create_barber(Level::FAST, 1.5);
		assert(false);
    } catch (const InvalidLevel& e) {
    }	catch(...)
	{
		assert(false);
	}

    try {
        auto _ = barberFactory.create_barber(Level::ADV, -1.0);
		assert(false);
    } catch (const std::runtime_error& e) {
    }
	catch(...)
	{
		assert(false);
	}

    // 简单对象创建测试
    auto customer = customerFactory.create_customer(Level::INT, 1.5, 3600);
    auto barber = barberFactory.create_barber(Level::INT, 1.5);
    auto chair = chairFactory.create_chair();

    assert(customer != nullptr);
    assert(barber != nullptr);
    assert(chair != nullptr);

    // 检查对象是否已注册到对应的 ObjectManager 中
    assert(manager.contains(customer->get_id()));
    assert(manager.contains(barber->get_id()));
    assert(manager.contains(chair->get_id()));
}
