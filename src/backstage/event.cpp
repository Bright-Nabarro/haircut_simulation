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
		double baseTime,
		const std::map<Level, double>& feeSchedule,
		std::function<void(std::string_view)> output,
		SimStatistics& stics):
	m_eventManager { eventManager },
	m_customerQue { customerQue },
	m_barberManager { barberManager },
	m_chairManager { chairManager },
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
Event::Event(EventHandlerComponents& cpts, Tick tick) :
	m_cpts { cpts },
	m_tick { tick }
{ }

Tick Event::tick() const
{
	return m_tick;
}

/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(
		EventHandlerComponents& cpts,
		Tick tick,
		const Id<Customer>& customerId
		):

	Event { cpts, tick },
	m_pCustomer { m_cpts.get_obj_manager().get_obj<Customer>(customerId) }
{ }

void CustomerArrivalEvent::execve()
{
	m_cpts.invoke_output(std::format("[{}] CustomerArrival Cust[{}]:[{}]", m_tick.to_string(),
				m_pCustomer->get_id().get_id_number(), get_level_str(m_pCustomer->get_level())));

	auto factorList { m_cpts.get_customer_que().push(m_pCustomer->get_id()) };
	Tick startHaircutTime { start_haircut_time(factorList) };
	size_t queLen { m_cpts.get_customer_que().get_que_size(m_pCustomer->get_level()) };
	m_cpts.get_stics().add_que_length(queLen-1);
	if (queLen == 1) 	//如果相应的顾客队列为空(自己推入),生成开始理发事件
	{
		m_cpts.get_event_manager().emplace<StartHaircutEvent>(
			m_cpts,	
			m_tick,
			m_pCustomer->get_level()
		);
	}
	else if (m_pCustomer->get_max_bearing_len() < queLen) 	//生成离队事件, 方便起见，顾客不入队
	{
		Tick leaveTick { m_tick };
		leaveTick.increament(1);
		m_cpts.get_event_manager().emplace<CustomerLeaveEvent>(
			m_cpts,
			leaveTick,
			m_pCustomer->get_id()
		);
	}
}


Tick CustomerArrivalEvent::start_haircut_time(const std::vector<double>& factorList)
{
	size_t second {0};
	for (double factor : factorList)
	{
		second += static_cast<size_t>(round(factor * m_cpts.get_base_time()));
	}
	Tick result { m_tick };
	Hms hms { cvt_seconds_to_hms(second) };
	m_cpts.get_stics().add_waiting_time(second);
	result.increament(hms.hour, hms.min, hms.sec);

	return result;
}


/*
 * StartHaircutEvent 
 */
StartHaircutEvent::StartHaircutEvent(
		EventHandlerComponents& cpts,
		Tick tick,
		Level level) :
	Event { cpts, tick },
	m_level { level }
{
	if (level == Level::FAST)
		throw logic_error { "level cannot be fast be StartHaircutEvent" };
}


void StartHaircutEvent::execve()
{
	auto pBarber { m_cpts.get_barber_manager().get_free_barber(m_level) };
	auto pChair { m_cpts.get_chair_manager().get_free_chair() };
	if (pBarber == nullptr)
		throw logic_error { "barberManager busy" };
	if (pChair == nullptr)
		throw logic_error { "charManger busy" };
	
	const auto& customerId { m_cpts.get_customer_que().top(m_level) };
	auto pCustomer { m_cpts.get_obj_manager().get_obj<Customer>(customerId) };
	pBarber->set_customer_id(pCustomer->get_shared());
	pChair->set_customer_barber(pCustomer->get_shared(), pBarber->get_shared());

	double increaseTime { m_cpts.get_base_time() * pCustomer->get_time_factor() };
	pBarber->increase_worktime(increaseTime);

	Hms hms { cvt_seconds_to_hms(static_cast<size_t>(round(increaseTime))) };
	Tick completeTick { m_tick };
	completeTick.increament(hms.hour, hms.min, hms.sec);

	m_cpts.get_event_manager().emplace<CompleteHaircutEvent>(
		m_cpts,
		completeTick,
		pChair->get_id()
	);

	m_cpts.invoke_output(std::format("[{}] StartHaircut Cust[{}]:[{}] Bar[{}]:[{}] Chr[{}]",
		m_tick.to_string(),
		customerId.get_id_number(), get_level_str(pCustomer->get_level()),
		pBarber->get_id().get_id_number(), get_level_str(pBarber->get_level()),
		pChair->get_id().get_id_number()));
}

/*
 * CustomerLeaveEvent
 */
CustomerLeaveEvent::CustomerLeaveEvent(
		EventHandlerComponents& cpts,
		Tick tick,
		const Id<Customer>& customerId) :
	Event { cpts, tick },
	m_pCustomer {  cpts.get_obj_manager().get_obj<Customer>(customerId) }
{ }

void CustomerLeaveEvent::execve()
{
	m_cpts.invoke_output(std::format("[{}] CustomerLeave Cust[{}]:[{}]",
				m_tick.to_string(),
				m_pCustomer->get_id().get_id_number(),
				get_level_str(m_pCustomer->get_level())));
}

/*
 * CompleteHaircutEvent
 * 回收Barber, Chair资源
 */
CompleteHaircutEvent::CompleteHaircutEvent(
		EventHandlerComponents& cpts,
		Tick tick,
		const Id<Chair>& chairId) :
	Event { cpts, tick },
	m_pChair { m_cpts.get_obj_manager().get_obj<Chair>(chairId) },
	m_customerId { m_pChair->get_customer_id() },
	m_barberId { m_pChair->get_barber_id() }
{ }

void CompleteHaircutEvent::execve()
{
	auto pCustomer { m_cpts.get_obj_manager().get_obj<Customer>(m_customerId) };
	auto pBarber { m_cpts.get_obj_manager().get_obj<Barber>(m_barberId) };

	m_cpts.invoke_output(std::format("[{}] CompleteHaircut Cust[{}]:[{}], Bar[{}]:[{}], Chr[{}]",
				m_tick.to_string(),
				m_customerId.get_id_number(), get_level_str(pCustomer->get_level()),
				m_barberId.get_id_number(), get_level_str(pBarber->get_level()),
				m_pChair->get_id().get_id_number()));

	m_cpts.get_customer_que().pop(pCustomer->get_level());
	m_cpts.get_barber_manager().free_barber(m_barberId);
	m_cpts.get_chair_manager().free_chair(m_pChair->get_id());
	pBarber->add_income(m_cpts.get_fee_schedule().find(pBarber->get_level())->second);
	Tick startHaircutTime { m_tick };
	if (m_cpts.get_customer_que().get_que_size(pCustomer->get_level()) > 0)
		m_cpts.get_event_manager().emplace<StartHaircutEvent>(
			m_cpts,
			startHaircutTime,
			pCustomer->get_level()
		);
}

}	//simulation

