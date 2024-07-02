#pragma once
#include <unordered_map>
#include <unordered_set>
#include "base_utility.hpp"
#include "customer.hpp"

namespace simulation
{

class Barber
{
public:
	Barber(Level level, double timeFactor, double totalWorktime = 0, double netIncome = 0);
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

	[[nodiscard]]
	double get_total_worktime() const;

	[[nodiscard]]
	double get_income() const;

	void set_customer_id(std::shared_ptr<Id<Customer>> pCustomerId);

	void release_customer();

	void increase_worktime(double time);

	void add_income(double income);


private:
	const std::shared_ptr<Id<Barber>> m_pId;
	const Level m_level;
	const double m_timeFactor;
	//Id不可复制，所以这里使用引用包装
	std::shared_ptr<Id<Customer>> m_pCustomerId;
	double m_totalWorktime;
	double m_netIncome;
};


/*
 * 应在注册完所有的理发师之后进行构造
 * 作为一个能够快速访问空闲理发师的类
 * 不包含Level为FAST的理发师，FAST匹配由上层完成
 * 如果添加枚举成员，需要修改construct_map_set
 */
template<typename Manager>
class BarberManager
{
public:
	BarberManager(const Manager& manager);
	virtual ~BarberManager() = default;
	void update();
	//注意：此方法会修改状态
	std::shared_ptr<Barber> get_free_barber(Level level);
	void free_barber(const Id<Barber>& barberId);
private:
	decltype(auto) construct_map_set();
	const Manager& m_manager;
	std::unordered_map<Level, std::unordered_set<std::reference_wrapper<const Id<Barber>>>> m_freeLevelTable;
	std::unordered_map<Level, std::unordered_set<std::reference_wrapper<const Id<Barber>>>> m_busyLevelTable;
};

#ifdef DEBUG

struct A {};
struct B {};
struct C {};

#endif

}		//simulation
