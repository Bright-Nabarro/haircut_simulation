#pragma once
#include <memory>
#include <format>
#include <unordered_map>
#include "base_utility.hpp"

namespace simulation
{

template<typename... Args>
class ObjectManager
{
public:
	ObjectManager()
	{
		( get_map<Args>() , ... );
	}

	template<typename Obj>
	void register_obj(const Id<Obj>& objId, std::shared_ptr<Obj> pObj)
	{
		auto& map { get_map<Obj>() };
		if (map.find(objId) != map.end())
		{
			throw std::invalid_argument { 
				std::format("id {} already existed", objId.get_id_number())
			}; 
		}
		map[std::cref(objId)] = pObj;
	}

	template<typename Obj>
	void remove_obj(const Id<Obj>& objId)
	{
	}

	template<typename Obj>
	std::shared_ptr<Obj> get_obj(const Id<Obj>& objId)
	{
	}
	
private:
	template<typename Obj>
	static auto& get_map()
	{
		static std::unordered_map<std::reference_wrapper<const Id<Obj>>,
			std::shared_ptr<Obj>> map;
		return map;
	}
};

}		//namespace simulation
