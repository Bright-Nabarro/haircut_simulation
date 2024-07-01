#pragma once
#include "test_header.hpp"
#include "event.hpp"

//这里只测试实例化，具体的流程直接在业务中调试
TPF
void test_event_ini()
{
	using namespace simulation;

    // 模拟管理对象
    ObjectManager<Customer, Barber, Chair> objManager;
    EventManager<Event> eventManager;

	try
	{
    	// 测试CustomerArrivalEvent
    	Id<Customer> customerId;
    	CustomerArrivalEvent customerArrivalEvent(objManager, eventManager, Tick(0, 0, 0), 10.0, nullptr, customerId);

    	// 测试StartHaircutEvent
    	StartHaircutEvent startHaircutEvent(objManager, eventManager, Tick(0, 0, 0), 10.0, nullptr, Level::BEG);

    	// 测试CustomerLeaveEvent
    	CustomerLeaveEvent customerLeaveEvent(objManager, eventManager, Tick(0, 0, 0), 10.0, nullptr, customerId);

    	// 测试CompleteHaircutEvent
    	Id<Barber> barberId;
    	Id<Chair> chairId;
    	CompleteHaircutEvent completeHaircutEvent(objManager, eventManager, Tick(0, 0, 0), 10.0, nullptr, customerId, barberId, chairId);
	}
	catch(...)
	{
	}
}
