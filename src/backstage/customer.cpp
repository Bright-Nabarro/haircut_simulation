#include "customer.hpp"
using namespace std;

namespace simulation
{

Customer::Customer(Level level, double timeFactor, size_t maxWaitingTime):
	m_pId{ std::make_shared<Id<Customer>>() },
	m_level { level }, 
	m_timeFactor { timeFactor }, 
	m_maxWaitingTime { maxWaitingTime },
	m_haircutStarted { false }
{ }

[[nodiscard]]
const Id<Customer>& Customer::get_id() const
{
	if (get_shared() == nullptr)
		throw logic_error { "Customer id pointer is null" };
	return *get_shared();
}

[[nodiscard]]
std::shared_ptr<Id<Customer>> Customer::get_shared() const
{
	return m_pId;
}

[[nodiscard]]
Level Customer::get_level() const noexcept
{
	return m_level;
}

[[nodiscard]]
double Customer::get_time_factor() const noexcept
{
	return m_timeFactor;
}

[[nodiscard]]
size_t Customer::get_max_waiting_time() const noexcept
{
	return m_maxWaitingTime;
}

[[nodiscard]]
bool Customer::haircut_started() const noexcept
{
	return m_haircutStarted;
}
	
void Customer::start_cuting_hear() noexcept
{
	m_haircutStarted = true;
}


}	//namespace simulation
