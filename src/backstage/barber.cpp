#include "barber.hpp"
#include "exception.hpp"
#include <format>
using namespace std;

namespace simulation
{

Barber::Barber(Level level, double timeFactor) :
	m_id{}, m_level { level }, m_timeFactor { timeFactor },
	m_customerId { nullopt }, m_totalWorktime {0}
{
	if (m_level == Level::FAST)
		throw invalid_argument { "Barber's Level cannot be FAST" };
	check_time_factor(m_timeFactor);
}

Barber::Barber(Level level, double timeFactor, const Id<Customer>& customerId) :
	m_id{}, m_level { level }, m_timeFactor { timeFactor },
	m_customerId { ref(customerId) }, m_totalWorktime {0}
{
	check_time_factor(m_timeFactor);
}

[[nodiscard]]
const Id<Barber>&  Barber::get_id() const noexcept
{
	return m_id;
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
	return m_customerId.has_value();
}

[[nodiscard]]
const Id<Customer>& Barber::get_customer_id() const
{
	if (!busy())
		throw logic_error { format("m_customerId does not store member") };
	
	return m_customerId.value().get();
}
	
void Barber::set_customer_id(const Id<Customer>& customerId)
{
	m_customerId.emplace(cref(customerId));
}

void Barber::release_customer()
{
	if (!busy())
		throw logic_error { "Barber dose not store a customerId" };
	m_customerId.reset();
}

}		//namespace simulation
