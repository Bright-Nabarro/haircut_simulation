#pragma once
#include "base_utility.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"

namespace simulation
{

class Event
{
protected:
	using SimulationManager = ObjectManager<Customer, Barber, Chair>;
public:
	Event(const Tick& tick, SimulationManager& manager);
	virtual ~Event() = default;
protected:
	virtual void execve() = 0;
	SimulationManager& m_manager;
	BarberManager<SimulationManager> m_barberManager;
	ChairManager<SimulationManager> m_chairManager;
private:
	Tick tick();
	Tick m_tick;
};


class CustomerArrivalEvent: public Event
{
public:
	CustomerArrivalEvent(const Tick& tick, SimulationManager& manager, const Id<Customer> customerId);
	void execve() override;
private:
	std::shared_ptr<Customer> pCustomer;
};


class StartHaircutEvent: public Event
{
public:
	StartHaircutEvent(const Tick& tick, SimulationManager& manager);
	void execve() override;
private:
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
	void execve() override;
private:
};

}	//namespace simulation
