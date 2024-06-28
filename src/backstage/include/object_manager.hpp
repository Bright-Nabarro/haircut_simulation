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
	bool contains(const Id<Obj>& objId) const
	{
		return find_itr(objId) != get_map<Obj>().end();
	}

	template<typename Obj>
	void register_obj(const Id<Obj>& objId, std::shared_ptr<Obj> pObj)
	{
		auto& map { get_map<Obj>() };
		auto itr { find_itr<Obj>(objId) };
		if (itr != map.end())
		{
			throw std::invalid_argument { 
				std::format("id {} already existes", objId.get_id_number())
			}; 
		}
		map[std::cref(objId)] = pObj;
	}

	template<typename Obj>
	void remove_obj(const Id<Obj>& objId)
	{
		auto& map { get_map<Obj>() };
		auto itr { find_itr<Obj>(objId) };
		if (itr == map.end())
		{
			throw std::invalid_argument { 
				std::format("id {} doesnot exist", objId.get_id_number())
			}; 
		}
		map.erase(itr);
	}

	template<typename Obj>
	std::shared_ptr<Obj> get_obj(const Id<Obj>& objId) const
	{
		auto& map { get_map<Obj>() };
		auto itr { find_itr<Obj>(objId) };
		if (itr == map.end())
		{
			throw std::invalid_argument { 
				std::format("id {} doesnot exist", objId.get_id_number())
			}; 
		}
		return itr->second;
	}
	
private:
	template<typename Obj>
	auto find_itr(const Id<Obj>& objId) const
	{
		auto& map { get_map<Obj>() };
		return map.find(objId);
	}

	template<typename Obj>
	static auto& get_map()
	{
		static std::unordered_map<std::reference_wrapper<const Id<Obj>>,
			std::shared_ptr<Obj>> map;
		return map;
	}
};

}		//namespace simulation
