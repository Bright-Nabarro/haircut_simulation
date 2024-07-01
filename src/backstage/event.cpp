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
Event::Event( SimulationManager& objManager,
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		std::function<void(std::string_view)> output) :
	m_objManager 	{ objManager },
	m_eventManager 	{ eventManager },
	m_customerQue 	{ customerQue },
	m_barberManager { barberManager },
	m_chairManager 	{ chairManager },
	m_tick 			{ tick },
	m_baseTime 		{ baseTime },
	m_output 		{ output }
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
		SimulationManager& objManager,
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		std::function<void(std::string_view)> output,
		const Id<Customer>& customerId) :

	Event { objManager, eventManager, customerQue, barberManager,
		chairManager, tick, baseTime, output },
	m_pCustomer { objManager.get_obj<Customer>(customerId) }
{ }

void CustomerArrivalEvent::execve()
{
	m_output(std::format("CustomerArrival Cust[{}]", m_pCustomer->get_id().get_id_number()));

	auto factorList { m_customerQue.push(m_pCustomer->get_id()) };
	Tick maxWaitTime { max_wait_time() };
	Tick startHaircutTime { start_haircut_time(factorList) };
	if ( startHaircutTime < maxWaitTime ) 	//生成开始理发事件
	{

	}
	else 	//生成离队事件, 方便起见，顾客不入队
	{
		m_eventManager.emplace<CustomerLeaveEvent>(
			m_objManager,
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			maxWaitTime,
			base_time(),
			m_output,
			m_pCustomer->get_id()
		);
	}
}

Tick CustomerArrivalEvent::max_wait_time() const
{
	//具体的面向用户的错误检查由工厂完成
	Hms hms { cvt_seconds_to_hms(m_pCustomer->get_max_waiting_time()) };
	Tick result { tick() };
	result.increament(hms.hour, hms.min, hms.sec);
	
	return result;
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
	result.increament(hms.hour, hms.min, hms.sec);

	return result;
}


/*
 * StartHaircutEvent 
 */
StartHaircutEvent::StartHaircutEvent(SimulationManager& objManager,
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		std::function<void(std::string_view)> output,
		Level level) :
	Event { objManager, eventManager, customerQue, barberManager, chairManager,
		tick, baseTime, output },
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
	
	const auto& customerId { m_customerQue.pop(m_level) };
	auto pCustomer { m_objManager.get_obj<Customer>(customerId) };
	pBarber->set_customer_id(pCustomer->get_shared());
	pChair->set_customer_barber(pCustomer->get_shared(), pBarber->get_shared());

	double increaseTime { base_time() * pCustomer->get_time_factor() };
	pBarber->increase_worktime(increaseTime);

	Hms hms { cvt_seconds_to_hms(static_cast<size_t>(round(increaseTime))) };
	Tick completeTick { tick() };
	completeTick.increament(hms.hour, hms.min, hms.sec);

	m_eventManager.emplace<CompleteHaircutEvent>(
			m_objManager,
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			completeTick,
			base_time(),
			m_output,
			customerId,
			pBarber->get_id(),
			pChair->get_id()
	);

	m_output(std::format("StartHaircut Cust[{}] Bar[{}] Chr[{}]",
				customerId.get_id_number(), pBarber->get_id().get_id_number(),
				pChair->get_id().get_id_number()));
}

/*
 * CustomerLeaveEvent
 */
CustomerLeaveEvent::CustomerLeaveEvent(SimulationManager& objManager,
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		std::function<void(std::string_view)> output,
		const Id<Customer>& customerId) :
	Event { objManager, eventManager, customerQue, barberManager, chairManager, 
		tick, baseTime, output },
	m_pCustomer {  objManager.get_obj<Customer>(customerId) }
{ }

void CustomerLeaveEvent::execve()
{
	m_output(std::format("CustomerLeave Cust[{}]", m_pCustomer->get_id().get_id_number()));
}

/*
 * CompleteHaircutEvent
 * 回收Barber, Chair资源
 */
CompleteHaircutEvent::CompleteHaircutEvent(SimulationManager& objManager,
		EventManager<Event>& eventManager,
		CustomerWaitingQue<SimulationManager>& customerQue,
		BarberManager<SimulationManager>& barberManager,
		ChairManager<SimulationManager>& chairManager,
		const Tick& tick,
		double baseTime,
		std::function<void(std::string_view)> output,
		const Id<Customer>& customerId,
		const Id<Barber>& barberId,
		const Id<Chair>& chairId) :
	Event { objManager, eventManager, customerQue, barberManager, chairManager, tick, baseTime, output },
	m_customerId { customerId },
	m_barberId { barberId },
	m_chairId { chairId }
{ }

void CompleteHaircutEvent::execve()
{
	m_output(std::format("CompleteHaircut Cust[{}], Bar[{}], Chr[{}]",
				m_customerId.get_id_number(), m_barberId.get_id_number(),
				m_chairId.get_id_number()));

	m_barberManager.free_barber(m_barberId);
	m_chairManager.free_chair(m_chairId);
	Tick startHaircutTime { tick() };
	startHaircutTime.increament(1);
	auto pCustomer { m_objManager.get_obj<Customer>(m_customerId) };
	m_eventManager.emplace<StartHaircutEvent>(
			m_objManager,
			m_eventManager,
			m_customerQue,
			m_barberManager,
			m_chairManager,
			startHaircutTime,
			base_time(),
			m_output,
			pCustomer->get_level()
	);
}

}	//simulation

