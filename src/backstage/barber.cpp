#include <format>
#include <cassert>
#include "barber.hpp"
#include "exception.hpp"
#include "chair.hpp"
#include "object_manager.hpp"
using namespace std;

namespace simulation
{

Barber::Barber(Level level, double timeFactor, double totalWorktime, double netIncome) :
	m_pId { make_shared<Id<Barber>>() },
 	m_level { level },
	m_timeFactor { timeFactor },
	m_pCustomerId { nullptr }, 
	m_totalWorktime { totalWorktime },
	m_netIncome { netIncome }
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
	return !(m_pCustomerId == nullptr);
}

[[nodiscard]]
const Id<Customer>& Barber::get_customer_id() const
{
	if (!busy())
		throw logic_error { format("m_customerId does not store member") };
	
	return *m_pCustomerId;
}

[[nodiscard]]
double Barber::get_total_worktime() const
{
	return m_totalWorktime;
}

[[nodiscard]]
double Barber::get_income() const
{
	return m_netIncome;
}

void Barber::set_customer_id(std::shared_ptr<Id<Customer>> pCustomerId)
{
	m_pCustomerId = pCustomerId;
}

void Barber::release_customer()
{
	if (!busy())
		throw logic_error { "Barber dose not store a customerId" };
	m_pCustomerId = nullptr;
}

void Barber::increase_worktime(double time)
{
	m_totalWorktime += time;
}

void Barber::add_income(double income)
{
	m_netIncome += income;
}

template<typename Manager>
BarberManager<Manager>::BarberManager():
	m_freeLevelTable { construct_map_set() },
	m_busyLevelTable { construct_map_set() }
{
	update();
}

template<typename Manager>
void BarberManager<Manager>::update()
{
	for (auto itr { m_manager.template cbegin<Barber>() };
		 itr != m_manager.template cend<Barber>();
		 ++itr)
	{
		if (itr->second->busy())
		{
			auto& busySet { m_busyLevelTable[itr->second->get_level()] };
			busySet.emplace(itr->second->get_id());
		}
		else
		{
			auto& freeSet { m_freeLevelTable[itr->second->get_level()] };
			freeSet.emplace(itr->second->get_id());
		}
	}
}

template<typename Manager>
std::shared_ptr<Barber> 
BarberManager<Manager>::get_free_barber(Level level)
{
	auto itr { m_freeLevelTable.find(level) };
	if ( itr == m_freeLevelTable.end() )
	{
		throw logic_error { "Unkown level in BarberManager, please check construct_map_set" };
	}
	auto& freeSet { itr->second };
	//此时对应等级的set为空
	if (freeSet.empty())
		return nullptr;
	//取出任意的理发师（第一个）
	auto retItr { freeSet.begin() };
	auto retBarber { m_manager.template get_obj<Barber>(retItr->get()) };
	freeSet.erase(retItr);

	auto& busySet { m_busyLevelTable[level] };
	busySet.emplace(retBarber->get_id());

	return retBarber;
}

template<typename Manager>
void BarberManager<Manager>::free_barber(const Id<Barber>& barberId)
{
	auto barber { m_manager.template get_obj<Barber>(barberId) };
	auto& busySet { m_busyLevelTable[barber->get_level()] };
	auto itr { busySet.find(barberId) };
	if (itr == busySet.cend())
		throw logic_error { format("Barber Manager cannot find id {} in busy barber table",
			barberId.get_id_number()) };
	busySet.erase(itr);
	barber->release_customer();
	auto& freeSet { m_freeLevelTable[barber->get_level()] };
	freeSet.emplace(barber->get_id());
}

template<typename Manager>
decltype(auto) BarberManager<Manager>::construct_map_set()
{
	static vector levels { Level::BEG, Level::INT, Level::ADV };
	std::unordered_map<Level, std::unordered_set<std::reference_wrapper<const Id<Barber>>>> map;
	for ( const auto& level : levels )
	{
		map[level] = {};
	}
	return map;
}

//显示实例化
using SimulationManager = ObjectManager<Customer, Barber, Chair>;
template class BarberManager<SimulationManager>;

//由于单例特性可能存在测试之间彼此污染的情况，添加多种实例化供单元测试使用
#ifdef DEBUG

using M1 = ObjectManager<Customer, Barber, Chair, A>;
template class BarberManager<M1>;
using M2 = ObjectManager<Customer, Barber, Chair, B>;
template class BarberManager<M2>;
using M3 = ObjectManager<Customer, Barber, Chair, C>;
template class BarberManager<M3>;

#endif

}		//namespace simulation
