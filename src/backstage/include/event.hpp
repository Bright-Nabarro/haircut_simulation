#pragma once
#include <functional>
#include <map>
#include "base_utility.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "event_manager.hpp"

namespace simulation
{

class SimStatistics;

class Event
{
	friend EventManager<Event>;
protected:
	using SimulationManager = ObjectManager<Customer, Barber, Chair>;
public:
	Event(	SimulationManager& objManager,
			EventManager<Event>& eventManager,
			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics);
	virtual ~Event() = default;
protected:
	virtual void execve() = 0;
	SimulationManager& m_objManager;
	EventManager<Event>& m_eventManager;
	CustomerWaitingQue<SimulationManager>& m_customerQue;
	BarberManager<SimulationManager>& m_barberManager;
	ChairManager<SimulationManager>& m_chairManager;
public:
	Tick tick() const;
protected:
	constexpr double base_time() const;
private:
	Tick m_tick;
	double m_baseTime;
protected:
	const std::map<Level, double>& m_feeSchedule;
	std::function<void(std::string_view)> m_output;
	SimStatistics& m_stics;
};


class CustomerArrivalEvent: public Event
{
public:
	CustomerArrivalEvent(
			SimulationManager& objManager,
			EventManager<Event>& eventManager,
 			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics,
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
			SimulationManager& objManager,
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
			SimulationManager& objManager,
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
	CompleteHaircutEvent(SimulationManager& objManager,
			EventManager<Event>& eventManager,
			CustomerWaitingQue<SimulationManager>& customerQue,
			BarberManager<SimulationManager>& barberManager,
			ChairManager<SimulationManager>& chairManager,
			const Tick& tick,
			double baseTime,
			const std::map<Level, double>& feeSchedule,
			std::function<void(std::string_view)> output,
			SimStatistics& stics,
			const Id<Customer>& customerId,
			const Id<Barber>& barberId,
			const Id<Chair>& chairId);

private:
	const Id<Customer>& m_customerId;
	const Id<Barber>& m_barberId;
	const Id<Chair>& m_chairId;
	void execve() override;
};

class SimStatistics
{
public:
	[[nodiscard]]
	double avg_waiting_time() const;
	[[nodiscard]]
	double avg_que_length() const;
	[[nodiscard]]
	size_t get_wrap_up_time() const;
	void add_waiting_time(size_t waitingTime);
	void add_que_length(size_t queLength);
	void set_wrap_up_time(size_t duration);
private:
	std::vector<size_t> m_waitingTimeVec;
	std::vector<size_t> m_queLengthVec;
	//收尾工作时间
	size_t m_wrapUpTime { std::numeric_limits<size_t>::max() };
};


}	//namespace simulation
