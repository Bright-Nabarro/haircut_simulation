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
using SimulationManager = ObjectManager<Customer, Barber, Chair>;

public:
	Event(const Tick& tick, SimulationManager& manager);
	virtual ~Event() = default;
	virtual void execve() const = 0;
protected:
	Tick m_tick;
	SimulationManager& m_manager;
};


class CustomerArrivalEvent: public Event
{
public:
	void execve() const override;
private:
	std::shared_ptr<Id<Customer>> pCustomerId;
};


class StartHaircutEvent: public Event
{
public:
	void execve() const override;
private:
};


class CustomerLeaveEvent: public Event
{
public:
	void execve() const override;
private:
};


class CompleteHaircutEvent: public Event
{
public:
	void execve() const override;
private:
};

}	//namespace simulation
