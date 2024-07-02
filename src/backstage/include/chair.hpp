#pragma once
#include <unordered_set>
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
	Chair(std::shared_ptr<Id<Customer>> pCustomerId, std::shared_ptr<Id<Barber>> pBarberId);
	virtual ~Chair() = default;

	[[nodiscard]]
	const Id<Chair>& get_id() const;

	[[nodiscard]]
	std::shared_ptr<Id<Chair>> get_shared() const;

	[[nodiscard]]
	bool busy() const;

	[[nodiscard]]
	const Id<Customer>& get_customer_id() const;

	[[nodiscard]]
	const Id<Barber>& get_barber_id() const;

	void set_customer_barber(std::shared_ptr<Id<Customer>> pCustomerId, std::shared_ptr<Id<Barber>> pBarberId);
	void release();

private:
	void check_bind() const;

private:
	std::shared_ptr<Id<Chair>> m_pId;
	std::pair<std::shared_ptr<Id<Customer>>, std::shared_ptr<Id<Barber>>> m_pIdPair;
};

template<typename Manager>
class ChairManager
{
public:
	ChairManager(const Manager& manager);
	virtual ~ChairManager() = default;
	void update();
	std::shared_ptr<Chair> get_free_chair();
	void free_chair(const Id<Chair>& chairId);
private:
	const Manager& m_manager;
	std::unordered_set<std::reference_wrapper<const Id<Chair>>> m_freeChairs;
	std::unordered_set<std::reference_wrapper<const Id<Chair>>> m_busyChairs;
};

#ifdef DEBUG	//用于单元测试

struct ChairTestA {};
struct ChairTestB {};
struct ChairTestC {};

#endif


}	//namespace simulation
