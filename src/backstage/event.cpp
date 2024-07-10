#include <cassert>
#include <format>
#include <cmath>
#include "event.hpp"

using namespace std;
namespace simulation
{

EventHandlerComponents::EventHandlerComponents(
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics):
	m_eventManager { eventManager },
	m_customerQue { customerQue },
	m_barberManager { barberManager },
	m_chairManager { chairManager },
	m_tick { tick },
	m_baseTime { baseTime },
	m_feeSchedule { feeSchedule },
	m_output { output },
	m_stics { stics }
{
}

auto EventHandlerComponents::get_obj_manager() -> SimulationManager&
{
	return m_objManager;
}

auto EventHandlerComponents::get_event_manager() -> EventManager<Event>&
{
	return m_eventManager;
}

auto EventHandlerComponents::get_customer_que() -> CustomerWaitingQue<SimulationManager>&
{
	return m_customerQue;
}

auto EventHandlerComponents::get_barber_manager() -> BarberManager<SimulationManager>&
{
	return m_barberManager;
}
auto EventHandlerComponents::get_tick() const -> Tick
{
	return m_tick;
}

auto EventHandlerComponents::get_base_time() const -> double
{
	return m_baseTime;
}
auto EventHandlerComponents::get_chair_manager() -> ChairManager<SimulationManager>&
{
	return m_chairManager;
}

auto EventHandlerComponents::get_fee_schedule() const -> const std::map<Level, double>&
{
	return m_feeSchedule;
}

auto EventHandlerComponents::get_stics() -> SimStatistics&
{
	return m_stics;
}

/*
 *	Event,
 */
Event::Event(EventHandlerComponents& cpts) :
	m_cpts { cpts }
{ }

/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(
		EventHandlerComponents& cpts,
		const Id<Customer>& customerId
		):

	Event { cpts },
	m_pCustomer { m_cpts.get_obj_manager().get_obj<Customer>(customerId) }
{ }

void CustomerArrivalEvent::execve()
{
	m_cpts.invoke_output(std::format("[{}] CustomerArrival Cust[{}]:[{}]", get_tick().to_string(),
				m_pCustomer->get_id().get_id_number(), get_level_str(m_pCustomer->get_level())));

	auto factorList { m_cpts.get_customer_que().push(m_pCustomer->get_id()) };
	Tick startHaircutTime { start_haircut_time(factorList) };
	size_t queLen { m_cpts.get_customer_que().get_que_size(m_pCustomer->get_level()) };
	m_cpts.get_stics().add_que_length(queLen-1);
	if (queLen == 1) 	//如果相应的顾客队列为空(自己推入),生成开始理发事件
	{
		m_cpts.get_event_manager().emplace<StartHaircutEvent>(
			m_cpts,
			m_pCustomer->get_level()
		);
	}
	else if (m_pCustomer->get_max_bearing_len() < queLen) 	//生成离队事件, 方便起见，顾客不入队
	{
		Tick leaveTick { get_tick() };
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
		const Id<Chair>& chairId) :
	Event { eventManager, customerQue, barberManager,
		chairManager, tick, baseTime, feeSchedule, output, stics},
	m_pChair { m_objManager.get_obj<Chair>(chairId) },
	m_customerId { m_pChair->get_customer_id() },
	m_barberId { m_pChair->get_barber_id() }
{ }

void CompleteHaircutEvent::execve()
{
	auto pCustomer { m_objManager.get_obj<Customer>(m_customerId) };
	auto pBarber { m_objManager.get_obj<Barber>(m_barberId) };

	m_output(std::format("[{}] CompleteHaircut Cust[{}]:[{}], Bar[{}]:[{}], Chr[{}]",
				tick().to_string(),
				m_customerId.get_id_number(), get_level_str(pCustomer->get_level()),
				m_barberId.get_id_number(), get_level_str(pBarber->get_level()),
				m_pChair->get_id().get_id_number()));

	m_customerQue.pop(pCustomer->get_level());
	m_barberManager.free_barber(m_barberId);
	m_chairManager.free_chair(m_pChair->get_id());
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

}	//simulation

