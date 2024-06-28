#pragma once
#include "base_utility.hpp"
#include "customer.hpp"

namespace simulation
{

class Barber
{
public:
	Barber(Level level, double timeFactor);
	//这里不提供等级是否匹配的检查，需要在上层手动检查
	Barber(Level level, double timeFactor, std::shared_ptr<Id<Customer>> pCustomerId);
	virtual ~Barber() = default;
	
	[[nodiscard]]
	const Id<Barber>& get_id() const;

	[[nodiscard]]
	std::shared_ptr<Id<Barber>> get_shared() const;

	[[nodiscard]]
	Level get_level() const noexcept;

	[[nodiscard]]
	double get_time_factor() const noexcept;

	[[nodiscard]]
	bool busy() const noexcept;

	[[nodiscard]]
	const Id<Customer>& get_customer_id() const;

	void set_customer_id(std::shared_ptr<Id<Customer>> pCustomerId);
	void release_customer();

private:
	const std::shared_ptr<Id<Barber>> m_pId;
	const Level m_level;
	const double m_timeFactor;
	//Id不可复制，所以这里使用引用包装
	std::shared_ptr<Id<Customer>> m_pCustomerId;
	double m_totalWorktime;
};

}		//simulation
