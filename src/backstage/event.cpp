#include "event.hpp"

using namespace std;
namespace simulation
{
/*
 *	CustomerArrivalEvent
 */

CustomerArrivalEvent::CustomerArrivalEvent(const Tick& tick, SimulationManager& manager,
	const Id<Customer> customerId) :
	Event { tick, manager },
	pCustomer { manager.get_obj<Customer>(customerId) }
{
}

void CustomerArrivalEvent::execve()
{
}

/*
 * StartHaircutEvent 
 */
StartHaircutEvent::StartHaircutEvent(const Tick& tick, SimulationManager& manager) :
	Event { tick, manager }
{
}

//注意此时理发师已经分配完毕
//椅子也判断完成
void StartHaircutEvent::execve()
{
	auto pChair { m_chairManager.get_free_chair() };
	if (pChair == nullptr)
		throw logic_error { "StartHaircutEvent doesnot have a empty chair" };
}

}	//simulation

