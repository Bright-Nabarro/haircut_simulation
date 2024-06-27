#pragma once
#include <optional>
#include <functional>
#include "base_utility.hpp"
#include "customer.hpp"

namespace simulation
{

class Barber
{
public:
	Barber(Level level, double timeFactor);
	//这里不提供等级是否匹配的检查，需要在上层手动检查
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

	void set_customer_id(const Id<Customer>& customerId);
	void release_customer();

private:
	const Id<Barber> m_id;
	const Level m_level;
	const double m_timeFactor;
	//Id不可复制，所以这里使用引用包装
	std::optional<std::reference_wrapper<const Id<Customer>>> m_customerId;
	double m_totalWorktime;
};

}		//simulation
