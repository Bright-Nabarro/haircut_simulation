#pragma once
#include <utility>
#include "object_manager.hpp"

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

}	//namespace simulation
