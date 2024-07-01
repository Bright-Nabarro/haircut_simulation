#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "object_manager.hpp"
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

void Customer::set_level(Level level)
{
	if (level == Level::FAST)
		throw logic_error { "set level of Customer cannot be FAST" };
	m_level = level;
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

template<typename Manager>
CustomerWaitingQue<Manager>::CustomerWaitingQue(const Manager& manager) :
	m_manager { manager },
	m_waitingQues {}
{
	ini_map();
}

template<typename Manager>
size_t CustomerWaitingQue<Manager>::get_que_size(Level level) const
{
	if (level == Level::FAST)
		throw logic_error { "get_que_size method cannot passing FAST as Level parameter" };
	const auto& que { check_level(level) };
	return que.size();
}

template<typename Manager>
[[maybe_unused]]
std::vector<double> CustomerWaitingQue<Manager>::push(const Id<Customer>& idCustomer)
{
	auto pCustomer { m_manager.template get_obj<Customer>(idCustomer) };
	if (pCustomer == nullptr)
		throw logic_error { format("cannot find customer id {} in object_manager",
				idCustomer.get_id_number()) };
	Level level { pCustomer->get_level() };
	if ( level == Level::FAST )
		level = find_min_que();

	auto& que { check_level(level) };
	que.push_back(pCustomer->get_id());
	pCustomer->set_level(level);

	std::vector<double> result;
	result.reserve(que.size());
	for (const auto& customerIdRef : que)
	{
		auto eachCustomer { m_manager.template get_obj<Customer>(customerIdRef.get()) };
		result.push_back(eachCustomer->get_time_factor());
	}
	return result;
}

template<typename Manager>
const Id<Customer>& CustomerWaitingQue<Manager>::pop(Level level)
{
	auto& que { check_level(level) };
	
	auto result { que.front() };
	que.pop_front();
	
	return result.get();
}

template<typename Manager>
void CustomerWaitingQue<Manager>::ini_map()
{
	for ( Level level : { Level::BEG, Level::INT, Level::ADV })
	{
		m_waitingQues[level];
	}
}

template<typename Manager>
Level CustomerWaitingQue<Manager>::find_min_que() const
{
	Level retLevel { m_waitingQues.begin()->first };
	size_t minSize { numeric_limits<size_t>::max() };
	for (const auto& [ level, que ] : m_waitingQues)
	{
		if (size_t queSize {get_que_size(level)}; queSize < minSize )
		{
			minSize = queSize;
			retLevel = level;
		}
	}
	return retLevel;
}

template<typename Manager>
auto& CustomerWaitingQue<Manager>::check_level(Level level)
{
	auto itr { m_waitingQues.find(level) };
	if (itr == m_waitingQues.end())
		throw logic_error { "get_que_size method recived a illegal Level parameter" };
	return itr->second;
}

template<typename Manager>
const auto& CustomerWaitingQue<Manager>::check_level(Level level) const
{
	auto itr { m_waitingQues.find(level) };
	if (itr == m_waitingQues.end())
		throw logic_error { "get_que_size method recived a illegal Level parameter" };
	return itr->second;
}

//显示实例化
using SimulationManager = ObjectManager<Customer, Barber, Chair>;
template class CustomerWaitingQue<SimulationManager>;

//由于单例特性可能存在测试之间彼此污染的情况，添加多种实例化供单元测试使用
#ifdef DEBUG

using M1 = ObjectManager<Customer, Barber, Chair, CustTestA>;
template class CustomerWaitingQue<M1>;
using M2 = ObjectManager<Customer, Barber, Chair, CustTestB>;
template class CustomerWaitingQue<M2>;
using M3 = ObjectManager<Customer, Barber, Chair, CustTestC>;
template class CustomerWaitingQue<M3>;

#endif


}	//namespace simulation
