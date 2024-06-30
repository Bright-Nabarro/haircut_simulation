#pragma once
#include "base_utility.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "event_manager.hpp"

namespace simulation
{

class Event
{
	friend EventManager<Event>;
protected:
	using SimulationManager = ObjectManager<Customer, Barber, Chair>;
public:
	Event(SimulationManager& objManager, EventManager<Event>& eventManager, const Tick& tick);
	virtual ~Event() = default;
protected:
	virtual void execve() = 0;
	SimulationManager& m_objManager;
	EventManager<Event>& m_eventManager;
	CustomerWaitingQue<SimulationManager> m_customerQue;
	BarberManager<SimulationManager> m_barberManager;
	ChairManager<SimulationManager> m_chairManager;
	Tick tick();
private:
	Tick m_tick;
};


class CustomerArrivalEvent: public Event
{
public:
	CustomerArrivalEvent(SimulationManager& objManager,
			EventManager<Event>& eventManager, const Tick& tick, const Id<Customer> customerId);
private:
	void execve() override;
	Tick max_wait_time();
	Tick start_haircut_time();
	std::shared_ptr<Customer> pCustomer;
};


class StartHaircutEvent: public Event
{
public:
	StartHaircutEvent(SimulationManager& objManager,
			EventManager<Event>& eventManager, const Tick& tick, const Id<Customer> customerId);
private:
	void execve() override;

};


class CustomerLeaveEvent: public Event
{
public:
	void execve() override;
private:
};


class CompleteHaircutEvent: public Event
{
public:
private:
	void execve() override;
};

}	//namespace simulation
