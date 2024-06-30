#include <cassert>
#include "event.hpp"

using namespace std;
namespace simulation
{

/*
 *	Event,
 *	Event不会将自身压入EventManager, 由子类压入
 */
Event::Event(SimulationManager& objManager, EventManager<Event>& m_eventManager, const Tick& tick) :
	m_objManager 	{ objManager },
	m_eventManager 	{ m_eventManager },
	m_barberManager { m_objManager },
	m_chairManager 	{ m_objManager },
	m_tick 			{ tick }
{
}


/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(SimulationManager& objManager,
			EventManager<Event>& eventManager, const Tick& tick, const Id<Customer> customerId) :
	Event { objManager, eventManager, tick },
	pCustomer { objManager.get_obj<Customer>(customerId) }
{
	m_eventManager.push(*this);
}

void CustomerArrivalEvent::execve()
{
	
	
}

void CustomerArrivalEvent::handle_barber()
{
	Level level { pCustomer->get_level() };
	if (level == Level::FAST)
	{
		
	}
	else
	{
	}
}

/*
 * StartHaircutEvent 
 */

//注意此时理发师已经分配完毕
//椅子也判断完成
void StartHaircutEvent::execve()
{
	auto pChair { m_chairManager.get_free_chair() };
	if (pChair == nullptr)
		throw logic_error { "StartHaircutEvent doesnot have a empty chair" };
}

}	//simulation

