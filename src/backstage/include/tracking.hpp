#pragma once
#include <vector>
#include <limits>

namespace simulation
{

class SimStatistics
{
public:
	[[nodiscard]]
	double avg_waiting_time() const;
	[[nodiscard]]
	double avg_que_length() const;
	[[nodiscard]]
	size_t get_wrap_up_time() const;
	void add_waiting_time(size_t waitingTime);
	void add_que_length(size_t queLength);
	void set_wrap_up_time(size_t duration);
private:
	std::vector<size_t> m_waitingTimeVec;
	std::vector<size_t> m_queLengthVec;
	//收尾工作时间
	size_t m_wrapUpTime { std::numeric_limits<size_t>::max() };
};

} 	//namespace simulation
