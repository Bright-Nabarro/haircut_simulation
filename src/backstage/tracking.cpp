#include "tracking.hpp"
#include <stdexcept>
using namespace std;

namespace simulation
{

[[nodiscard]]
double SimStatistics::avg_waiting_time() const
{
	double sum { 0 };
	for (auto x : m_waitingTimeVec)
		sum += static_cast<double>(x);

	return sum / static_cast<double>(m_waitingTimeVec.size());
}

[[nodiscard]]
double SimStatistics::avg_que_length() const
{
	double sum { 0 };
	for (auto x : m_queLengthVec)
		sum += static_cast<double>(x);

	return sum / static_cast<double>(m_queLengthVec.size());
}

[[nodiscard]]
size_t SimStatistics::get_wrap_up_time() const
{
	if (m_wrapUpTime == numeric_limits<size_t>::max())
		throw logic_error { "unset wrap up time" };
    return m_wrapUpTime;
}

void SimStatistics::add_waiting_time(size_t waitingTime)
{
	m_waitingTimeVec.push_back(waitingTime);
}

void SimStatistics::add_que_length(size_t queLength)
{
	m_queLengthVec.push_back(queLength);
}

void SimStatistics::set_wrap_up_time(size_t duration)
{
	static bool hasInvoked { false };
	if (!hasInvoked)
	{
		m_wrapUpTime = duration;
		hasInvoked = true;
	}
	else
		throw logic_error { "set_wrap_up_time has invoked twice" };
}

}		//namespace simulation

