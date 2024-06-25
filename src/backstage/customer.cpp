#include "customer.hpp"

namespace simulation
{

Customer::Customer(Level level, double timeFactor, int maxWaitingTime):
	m_id{}, m_level { level }, m_timeFactor { timeFactor }, m_maxWaitingTime { maxWaitingTime },
	m_haircutStarted { false }
{ }

[[nodiscard]]
size_t Customer::get_id() const noexcept
{
	return m_id.get();
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
int Customer::get_max_waiting_time() const noexcept
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
