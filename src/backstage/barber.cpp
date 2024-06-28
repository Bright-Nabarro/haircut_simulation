#include "barber.hpp"
#include "exception.hpp"
#include <format>
using namespace std;

namespace simulation
{

Barber::Barber(Level level, double timeFactor) :
	m_pId { make_shared<Id<Barber>>() },
 	m_level { level },
	m_timeFactor { timeFactor },
	m_pCustomerId { nullptr }, 
	m_totalWorktime {0}
{
	if (m_level == Level::FAST)
		throw invalid_argument { "Barber's Level cannot be FAST" };
	check_time_factor(m_timeFactor);
}

Barber::Barber(Level level, double timeFactor, std::shared_ptr<Id<Customer>> pCustomerId) :
	m_pId { make_shared<Id<Barber>>() },
	m_level { level },
	m_timeFactor { timeFactor },
	m_pCustomerId { pCustomerId },
	m_totalWorktime {0}
{
	check_time_factor(m_timeFactor);
}

[[nodiscard]]
const Id<Barber>& Barber::get_id() const
{
	if (get_shared() == nullptr)
		throw logic_error { " Barber id ptr is null " };
	return *get_shared();
}

[[nodiscard]]
std::shared_ptr<Id<Barber>> Barber::get_shared() const
{
	return m_pId;
}

[[nodiscard]]
Level Barber::get_level() const noexcept
{
	return m_level;
}

[[nodiscard]]
double Barber::get_time_factor() const noexcept
{
	return m_timeFactor;
}

[[nodiscard]]
bool Barber::busy() const noexcept
{
	return m_pCustomerId == nullptr;
}

[[nodiscard]]
const Id<Customer>& Barber::get_customer_id() const
{
	if (!busy())
		throw logic_error { format("m_customerId does not store member") };
	
	return *m_pCustomerId;
}
	
void Barber::set_customer_id(std::shared_ptr<Id<Customer>> pCustomerId)
{
	m_pCustomerId = pCustomerId;
}

void Barber::release_customer()
{
	if (!busy())
		throw logic_error { "Barber dose not store a customerId" };
	m_pCustomerId.reset();
}

}		//namespace simulation
