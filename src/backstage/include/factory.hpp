#pragma once
#include <utility>
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"

namespace simulation
{

template<typename Ty, typename Manager = ObjectManager<Ty>>
class ObjFactory
{
public:
	ObjFactory(Manager& objectManager):
		m_objectManager { objectManager }
	{ }
	virtual ~ObjFactory() = default;
	
	template<typename... Args>
	[[nodiscard]]
	std::shared_ptr<Ty> create_obj(Args&&... args)
	{
		auto obj { std::make_shared<Ty>(std::forward<Args>(args)...) };
		m_objectManager.template register_obj<Ty>(obj->get_id(), obj);
		return obj;
	}

private:
	Manager& m_objectManager;
};

using UseObjManager = ObjectManager<Customer, Barber, Chair>;

class CustomerFactory : ObjFactory<Customer, UseObjManager>
{
public:
	using ObjFactory<Customer, UseObjManager>::ObjFactory;
	[[maybe_unused]]
	std::shared_ptr<Customer> create_customer(Level level, double timeFactor, size_t maxBearingLen);
};

class BarberFactory : ObjFactory<Barber, UseObjManager>
{
public:
	using ObjFactory<Barber, UseObjManager>::ObjFactory;
	[[maybe_unused]]
	std::shared_ptr<Barber> create_barber(Level level, double timeFactor,
			double totalTime, double netIncome);
};

class ChairFactory : ObjFactory<Chair, UseObjManager>
{
public:
	using ObjFactory<Chair, UseObjManager>::ObjFactory;
	[[maybe_unused]]
	std::shared_ptr<Chair> create_chair();
};

}	//namespace simulation
