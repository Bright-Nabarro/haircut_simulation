#pragma once
#include <functional>
#include <map>
#include "base_utility.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "event_manager.hpp"
#include "tracking.hpp"

namespace simulation
{

class Event;

class EventHandlerComponents
{
	using SimulationManager = ObjectManager<Customer, Barber, Chair>;

public:
	auto get_obj_manager() 			-> SimulationManager&;
	auto get_event_manager() 		-> EventManager<Event>&;
	auto get_customer_que() 		-> CustomerWaitingQue<SimulationManager>&;
	auto get_barber_manager() 		-> BarberManager<SimulationManager>&;
	auto get_chair_manager() 		-> ChairManager<SimulationManager>&;
	auto get_tick() const 			-> Tick;
	auto get_base_time() const 		-> double;
	auto get_fee_schedule() const 	-> const std::map<Level, double>&;
	auto get_stics() 				-> SimStatistics&;
	template<typename... Args>
	void invoke_output(Args... args) const
	{
		m_output(args...);
	}

private:
	EventHandlerComponents(
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics);

	SimulationManager m_objManager {};
	EventManager<Event>& m_eventManager;
	CustomerWaitingQue<SimulationManager>& m_customerQue;
	BarberManager<SimulationManager>& m_barberManager;
	ChairManager<SimulationManager>& m_chairManager;
	const Tick& m_tick;
	double m_baseTime;
	const std::map<Level, double>& m_feeSchedule;
	std::function<void(std::string_view)> m_output;
	SimStatistics& m_stics;
};

class Event
{
	friend EventManager<Event>;

public:
	virtual ~Event() = default;
	virtual void execve() = 0;
	auto get_tick() const -> Tick
	{ return m_cpts.get_tick(); }

protected:
	Event(EventHandlerComponents& paramenters);
	EventHandlerComponents m_cpts;
};


class CustomerArrivalEvent: public Event
{
public:
	CustomerArrivalEvent(
		EventHandlerComponents& cpts,
		const Id<Customer>& customerId
	);
private:
	void execve() override;
	Tick start_haircut_time(const std::vector<double>& factorList) const;
	std::shared_ptr<Customer> m_pCustomer;
};


class StartHaircutEvent: public Event
{
public:
	StartHaircutEvent(
			EventManager<Event>& eventManager,
			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics,
			Level level);
private:
	void execve() override;
	Level m_level;
};


class CustomerLeaveEvent: public Event
{
public:
	CustomerLeaveEvent(
			EventManager<Event>& eventManager,
			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics,
			const Id<Customer>& customerId);

private:
	void execve() override;
	std::shared_ptr<Customer> m_pCustomer;
};


class CompleteHaircutEvent: public Event
{
public:
	CompleteHaircutEvent(
			EventManager<Event>& eventManager,
			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics,
			const Id<Chair>& chairId);

private:
	std::shared_ptr<Chair> m_pChair;
	const Id<Customer>& m_customerId;
	const Id<Barber>& m_barberId;
	void execve() override;
};

}	//namespace simulation
