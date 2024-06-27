#pragma once
#include <utility>
#include <functional>
#include "customer.hpp"
#include "barber.hpp"
#include "base_utility.hpp"

namespace simulation
{

class Chair
{
public:
	Chair();
	//这里不提供Barber和Customer的匹配检查，需要在上级完成
	Chair(const Id<Customer>& customerId, const Id<Barber>& barberId);
	virtual ~Chair() = default;

	[[nodiscard]]
	const Id<Chair>& get_id() const noexcept;

	[[nodiscard]]
	bool busy() const noexcept;

	[[nodiscard]]
	const Id<Customer>& get_customer_id() const;

	[[nodiscard]]
	const Id<Barber>& get_barber_id() const;

	void set_customer_barber(const Id<Customer>& customerId, const Id<Barber>& barberId);
	void release();

private:
	void check_bind() const;

private:
	Id<Chair> m_id;
	std::optional<
		std::pair<std::reference_wrapper<const Id<Customer>>,
		std::reference_wrapper<const Id<Barber>> >
	> m_cbId;
};

}	//namespace simulation
