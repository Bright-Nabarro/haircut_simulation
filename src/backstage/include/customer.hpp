#pragma once
#include <memory>
#include <deque>
#include "base_utility.hpp"

namespace simulation
{

class Customer
{
public:
	Customer(Level level, double timeFactor, size_t maxWaitingTime);
	virtual ~Customer() = default;

	[[nodiscard]]
	const Id<Customer>& get_id() const;

	[[nodiscard]]
	std::shared_ptr<Id<Customer>> get_shared() const;

	[[nodiscard]]
	Level get_level() const noexcept;

	[[nodiscard]]
	double get_time_factor() const noexcept;

	[[nodiscard]]
	size_t get_max_waiting_time() const noexcept;

	[[nodiscard]]
	bool haircut_started() const noexcept;
	
	void start_cuting_hear() noexcept;

private:
	const std::shared_ptr<Id<Customer>> m_pId;
	const Level m_level;
	const double m_timeFactor;	//消耗时间系数
	const size_t m_maxWaitingTime; //最长等待时间
	bool m_haircutStarted;
};

template<typename Manager>
class CustomerWaitingQue
{
public:
	CustomerWaitingQue(const Manager& manager);
	virtual ~CustomerWaitingQue() = default;
	//以人数作为比较依据，level不包含FAST
	size_t get_que_size(Level level) const;
	//这里支持FAST
	void push(const Id<Customer>& idCustomer);
	const Id<Customer>& pop(Level level);
	
private:
	void ini_map();
	Level find_min_que() const;
	auto& check_level(Level level);
	const auto& check_level(Level level) const;
	const Manager& m_manager;
	std::unordered_map<Level,
		std::deque<std::reference_wrapper<const Id<Customer>>>> m_waitingQues;
};

#ifdef DEBUG	//用于单元测试

struct CustTestA {};
struct CustTestB {};
struct CustTestC {};

#endif

}	//namespace simulation
