#include <cassert>
#include "event.hpp"

using namespace std;
namespace simulation
{

/*
 *	Event,
 */
Event::Event(SimulationManager& objManager, EventManager<Event>& m_eventManager, const Tick& tick) :
	m_objManager 	{ objManager },
	m_eventManager 	{ m_eventManager },
	m_customerQue 	{ m_objManager },
	m_barberManager { m_objManager },
	m_chairManager 	{ m_objManager },
	m_tick 			{ tick }
{ }


/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(SimulationManager& objManager,
			EventManager<Event>& eventManager, const Tick& tick, const Id<Customer> customerId) :
	Event { objManager, eventManager, tick },
	pCustomer { objManager.get_obj<Customer>(customerId) }
{ }

void CustomerArrivalEvent::execve()
{
	m_customerQue.push(pCustomer->get_id());
	Tick maxWaitTime { max_wait_time() };
	Tick startHaircutTime { start_haircut_time() };
	if ( startHaircutTime < maxWaitTime ) 	//生成开始理发事件
	{
	}
	else 	//生成离队事件
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

