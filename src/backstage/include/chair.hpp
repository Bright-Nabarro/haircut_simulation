#pragma once
#include <utility>
#include "customer.hpp"
#include "barber.hpp"
#include "base_utility.hpp"

namespace simulation
{

class Chair
{
public:
	Chair();
	Chair(const Id<Customer>& customerId, const Id<Barber>& barberId);
	virtual ~Chair() = default;

	const Id<Chair>& get_id() const noexcept;

	bool busy() const noexcept;

	

private:
	Id<Chair> m_id;
	std::optional<
		std::pair<std::reference_wrapper<const Id<Customer>>,
		std::reference_wrapper<const Id<Barber>> >
	> m_cbId;
};

}	//namespace simulation
