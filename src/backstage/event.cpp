#include <cassert>
#include <format>
#include <cmath>
#include "event.hpp"

using namespace std;
namespace simulation
{

/*
 *	Event,
 */
Event::Event(
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics) :
	m_eventManager 	{ eventManager },
	m_customerQue 	{ customerQue },
	m_barberManager { barberManager },
	m_chairManager 	{ chairManager },
	m_tick 			{ tick },
	m_baseTime 		{ baseTime },
	m_feeSchedule 	{ feeSchedule },
	m_output 		{ output },
	m_stics 		{ stics }
{ }

Tick Event::tick() const
{
	return m_tick;
}

constexpr double Event::base_time() const
{
	return m_baseTime;
}

/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(
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
		):

	Event { eventManager, customerQue, barberManager,
		chairManager, tick, baseTime, feeSchedule, output, stics},
	m_pCustomer { m_objManager.get_obj<Customer>(customerId) }
{ }

void CustomerArrivalEvent::execve()
{
	m_output(std::format("[{}] CustomerArrival Cust[{}]:[{}]", tick().to_string(),
				m_pCustomer->get_id().get_id_number(), get_level_str(m_pCustomer->get_level())));

	auto factorList { m_customerQue.push(m_pCustomer->get_id()) };
	Tick startHaircutTime { start_haircut_time(factorList) };
	size_t queLen { m_customerQue.get_que_size(m_pCustomer->get_level()) };
	m_stics.add_que_length(queLen-1);
	if (queLen == 1) 	//如果相应的顾客队列为空(自己推入),生成开始理发事件
	{
		m_eventManager.emplace<StartHaircutEvent>(
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			tick(),
			base_time(),
			m_feeSchedule,
			m_output,
			m_stics,
			m_pCustomer->get_level()
		);
	}
	else if (m_pCustomer->get_max_bearing_len() < queLen) 	//生成离队事件, 方便起见，顾客不入队
	{
		Tick leaveTick { tick() };
		leaveTick.increament(1);
		m_eventManager.emplace<CustomerLeaveEvent>(
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			leaveTick,
			base_time(),
			m_feeSchedule,
			m_output,
			m_stics,
			m_pCustomer->get_id()
		);
	}
}


Tick CustomerArrivalEvent::start_haircut_time(const std::vector<double>& factorList) const
{
	size_t second {0};
	for (double factor : factorList)
	{
		second += static_cast<size_t>(round(factor * base_time()));
	}
	Tick result { tick() };
	Hms hms { cvt_seconds_to_hms(second) };
	m_stics.add_waiting_time(second);
	result.increament(hms.hour, hms.min, hms.sec);

	return result;
}


/*
 * StartHaircutEvent 
 */
StartHaircutEvent::StartHaircutEvent(
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics,
		Level level) :
	Event { eventManager, customerQue, barberManager, chairManager,
		tick, baseTime, feeSchedule, output, stics},
	m_level { level }
{
	if (level == Level::FAST)
		throw logic_error { "level cannot be fast be StartHaircutEvent" };
}


void StartHaircutEvent::execve()
{
	auto pBarber { m_barberManager.get_free_barber(m_level) };
	auto pChair { m_chairManager.get_free_chair() };
	if (pBarber == nullptr)
		throw logic_error { "barberManager busy" };
	if (pChair == nullptr)
		throw logic_error { "charManger busy" };
	
	const auto& customerId { m_customerQue.top(m_level) };
	auto pCustomer { m_objManager.get_obj<Customer>(customerId) };
	pBarber->set_customer_id(pCustomer->get_shared());
	pChair->set_customer_barber(pCustomer->get_shared(), pBarber->get_shared());

	double increaseTime { base_time() * pCustomer->get_time_factor() };
	pBarber->increase_worktime(increaseTime);

	Hms hms { cvt_seconds_to_hms(static_cast<size_t>(round(increaseTime))) };
	Tick completeTick { tick() };
	completeTick.increament(hms.hour, hms.min, hms.sec);

	m_eventManager.emplace<CompleteHaircutEvent>(
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			completeTick,
			base_time(),
			m_feeSchedule,
			m_output,
			m_stics,
			customerId,
			pBarber->get_id(),
			pChair->get_id()
	);

	m_output(std::format("[{}] StartHaircut Cust[{}]:[{}] Bar[{}]:[{}] Chr[{}]",
				tick().to_string(),
				customerId.get_id_number(), get_level_str(pCustomer->get_level()),
				pBarber->get_id().get_id_number(), get_level_str(pBarber->get_level()),
				pChair->get_id().get_id_number()));
}

/*
 * CustomerLeaveEvent
 */
CustomerLeaveEvent::CustomerLeaveEvent(
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics,
		const Id<Customer>& customerId) :
	Event { eventManager, customerQue, barberManager, chairManager, 
		tick, baseTime, feeSchedule, output, stics},
	m_pCustomer {  m_objManager.get_obj<Customer>(customerId) }
{ }

void CustomerLeaveEvent::execve()
{
	m_output(std::format("[{}] CustomerLeave Cust[{}]:[{}]",
				tick().to_string(),
				m_pCustomer->get_id().get_id_number(),
				get_level_str(m_pCustomer->get_level())));
}

/*
 * CompleteHaircutEvent
 * 回收Barber, Chair资源
 */
CompleteHaircutEvent::CompleteHaircutEvent(
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
		const Id<Chair>& chairId) :
	Event { eventManager, customerQue, barberManager,
		chairManager, tick, baseTime, feeSchedule, output, stics},
	m_customerId { customerId },
	m_barberId { barberId },
	m_chairId { chairId }
{ }

void CompleteHaircutEvent::execve()
{
	auto pCustomer { m_objManager.get_obj<Customer>(m_customerId) };
	auto pBarber { m_objManager.get_obj<Barber>(m_barberId) };

	m_output(std::format("[{}] CompleteHaircut Cust[{}]:[{}], Bar[{}]:[{}], Chr[{}]",
				tick().to_string(),
				m_customerId.get_id_number(), get_level_str(pCustomer->get_level()),
				m_barberId.get_id_number(), get_level_str(pBarber->get_level()),
				m_chairId.get_id_number()));

	m_customerQue.pop(pCustomer->get_level());
	m_barberManager.free_barber(m_barberId);
	m_chairManager.free_chair(m_chairId);
	pBarber->add_income(m_feeSchedule.find(pBarber->get_level())->second);
	Tick startHaircutTime { tick() };
	if (m_customerQue.get_que_size(pCustomer->get_level()) > 0)
		m_eventManager.emplace<StartHaircutEvent>(
				m_eventManager,
				m_customerQue,
				m_barberManager,
				m_chairManager,
				startHaircutTime,
				base_time(),
				m_feeSchedule,
				m_output,
				m_stics,
				pCustomer->get_level()
		);
}

double SimStatistics::avg_waiting_time() const
{
	double sum { 0 };
	for (auto x : m_waitingTimeVec)
		sum += static_cast<double>(x);

	return sum / static_cast<double>(m_waitingTimeVec.size());
}

double SimStatistics::avg_que_length() const
{
	double sum { 0 };
	for (auto x : m_queLengthVec)
		sum += static_cast<double>(x);

	return sum / static_cast<double>(m_queLengthVec.size());
}

size_t SimStatistics::get_wrap_up_time() const
{
	if (m_wrapUpTime == numeric_limits<size_t>::max())
		throw logic_error { "unset wrap up time" };
    return m_wrapUpTime;
}

void SimStatistics::add_waiting_time(size_t waitingTime)
{
	m_waitingTimeVec.push_back(waitingTime);
}

void SimStatistics::add_que_length(size_t queLength)
{
	m_queLengthVec.push_back(queLength);
}

void SimStatistics::set_wrap_up_time(size_t duration)
{
	static bool hasInvoked { false };
	if (!hasInvoked)
	{
		m_wrapUpTime = duration;
		hasInvoked = true;
	}
	else
		throw logic_error { "set_wrap_up_time has invoked twice" };
}

}	//simulation

