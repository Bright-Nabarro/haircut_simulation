#include <format>
#include "chair.hpp"
#include "object_manager.hpp"
#include "barber.hpp"
#include "customer.hpp"

using namespace std;

namespace simulation
{

Chair::Chair() :
	m_pId { make_shared<Id<Chair>>() },
	m_pIdPair { nullptr, nullptr }
{ }

Chair::Chair(std::shared_ptr<Id<Customer>> pCustomerId, std::shared_ptr<Id<Barber>> pBarberId) :
	m_pId { make_shared<Id<Chair>>() },
	m_pIdPair { pCustomerId, pBarberId }
{ }

[[nodiscard]]
const Id<Chair>& Chair::get_id() const
{
	if (get_shared() == nullptr)
		throw logic_error { "Chair id pointer is null" };
	return *get_shared();
}

[[nodiscard]]
std::shared_ptr<Id<Chair>> Chair::get_shared() const
{
	return m_pId;
}

[[nodiscard]]
bool Chair::busy() const
{
	if (!(m_pIdPair.first == nullptr && m_pIdPair.second == nullptr) &&
		!(m_pIdPair.first != nullptr && m_pIdPair.second != nullptr))
	{
		throw logic_error { "Chair's pIdPair's status does not match" };
	}

	return !(m_pIdPair.first == nullptr);
}

[[nodiscard]]
const Id<Customer>& Chair::get_customer_id() const
{
	check_bind();
	return *m_pIdPair.first;
}

[[nodiscard]]
const Id<Barber>& Chair::get_barber_id() const
{
	check_bind();
	return *m_pIdPair.second;
}

void Chair::set_customer_barber(std::shared_ptr<Id<Customer>> pCustomerId, std::shared_ptr<Id<Barber>> pBarberId)
{
	m_pIdPair = { pCustomerId, pBarberId };
}

void Chair::release()
{
	check_bind();
	m_pIdPair = { nullptr, nullptr };
}

void Chair::check_bind() const
{
	if (!busy())
		throw logic_error { format("chair {} does not bind to a cb pair", m_pId->get_id_number()) };
}


template<typename Manager>
ChairManager<Manager>::ChairManager(const Manager& manager) :
	m_manager { manager },
	m_freeChairs {},
	m_busyChairs {}
{
	for (auto itr { m_manager.template cbegin<Chair>() };
		 itr != m_manager.template cend<Chair>();
		 ++itr)
	{
		if (itr->second->busy())
			m_busyChairs.emplace(itr->first);
		else
			m_freeChairs.emplace(itr->first);
	}
}

template<typename Manager>
std::shared_ptr<Chair> ChairManager<Manager>::get_free_chair()
{
	if (m_freeChairs.empty())
		return nullptr;
	auto itr { m_freeChairs.cbegin() };
	auto chair { m_manager.template get_obj<Chair>(*itr) };
	m_freeChairs.erase(itr);
	m_busyChairs.emplace(chair->get_id());
	return chair;
}

template<typename Manager>
void ChairManager<Manager>::free_chair(const Id<Chair>& chairId)
{
	auto chair { m_manager.template get_obj<Chair>(chairId) };
	auto itr { m_busyChairs.find(chair->get_id()) };
	if (itr == m_busyChairs.cend())
		throw logic_error { format("Chair busy list doesnot find id {}",
				chair->get_id().get_id_number()) };
	m_busyChairs.erase(itr);
	m_freeChairs.emplace(chair->get_id());
}

//显示实例化
using SimulationManager = ObjectManager<Customer, Barber, Chair>;
template class BarberManager<SimulationManager>;

//由于单例特性可能存在测试之间彼此污染的情况，添加多种实例化供单元测试使用
#ifdef DEBUG

using M1 = ObjectManager<Customer, Barber, Chair, ChairTestA>;
template class ChairManager<M1>;
using M2 = ObjectManager<Customer, Barber, Chair, ChairTestB>;
template class ChairManager<M2>;
using M3 = ObjectManager<Customer, Barber, Chair, ChairTestC>;
template class ChairManager<M3>;

#endif


}		//namespace simulation
