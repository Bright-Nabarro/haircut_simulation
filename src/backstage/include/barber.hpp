#pragma once
#include <optional>
#include "customer.hpp"
#include "base_utility.hpp"

namespace simulation
{

class Barber
{
public:
	Barber(Level level, double timeFactor);
	Barber(Level level, double timeFactor, const Id<Customer>& customerId);
	virtual ~Barber() = default;
	
	[[nodiscard]]
	const Id<Barber>& get_id() const noexcept;

	[[nodiscard]]
	Level get_level() const noexcept;

	[[nodiscard]]
	double get_time_factor() const noexcept;

	[[nodiscard]]
	bool busy() const noexcept;

	[[nodiscard]]
	const Id<Customer>& get_customer_id() const;

	[[nodiscard]]
	const Id<Barber>& get_barber_id() const;

private:
	const Id<Barber> m_id;
	const Level m_level;
	const double m_timeFactor;
	//Id不可复制，所以这里使用引用包装
	std::optional<std::reference_wrapper<const Id<Customer>>> m_customerId;
	double m_totalWorktime;
};

}		//simulation
