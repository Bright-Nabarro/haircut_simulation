#include <format>
#include "chair.hpp"

using namespace std;

namespace simulation
{

Chair::Chair() : m_id{}, m_cbId { std::nullopt }
{ }

Chair::Chair(const Id<Customer>& customerId, const Id<Barber>& barberId) :
	m_id{}, m_cbId { pair { cref(customerId), cref(barberId) } }
{ }

[[nodiscard]]
const Id<Chair>& Chair::get_id() const noexcept
{
	return m_id;
}

[[nodiscard]]
bool Chair::busy() const noexcept
{
	return m_cbId.has_value();
}

[[nodiscard]]
const Id<Customer>& Chair::get_customer_id() const
{
	check_bind();
	return m_cbId.value().first;
}

[[nodiscard]]
const Id<Barber>& Chair::get_barber_id() const
{
	check_bind();
	return m_cbId.value().second;
}

void Chair::set_customer_barber(const Id<Customer>& customerId, const Id<Barber>& barberId)
{
	m_cbId.emplace(cref(customerId), cref(barberId));
}

void Chair::release()
{
	check_bind();
	m_cbId.reset();
}

void Chair::check_bind() const
{
	if (!busy())
		throw logic_error { format("chair {} does not bind to a cb pair", m_id.get_id_number()) };
}


}		//namespace simulation
