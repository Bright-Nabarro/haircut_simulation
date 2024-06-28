#include <format>
#include "chair.hpp"

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

	return m_pIdPair.first == nullptr;
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


}		//namespace simulation
