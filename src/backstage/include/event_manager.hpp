#pragma once
#include <list>
#include <memory>
#include <queue>

namespace simulation
{

/*
 * 内部持有优先队列管理事件
 * 优先队列储存迭代器，指向内部的事件表（避免了事件在优先队列中被默认构造的情况）
 * 管理增加删除的情形(本质是对优先队列的包装)
 */
template<typename Event> 
class EventManager
{
	using Itr = typename std::list<std::unique_ptr<Event>>::iterator;
public:
	EventManager(): m_queue{}, m_events{}
	{ }
	void execve()
	{
		if (empty())
			throw std::logic_error("No events in EventManager");

		Itr itr { m_queue.top() };
		m_queue.pop();
		(*itr)->execve();
		//c++标准保证list的其余迭代器在删除之后仍然有效
		m_events.erase(itr);
	}

	template<typename... Args>
	void emplace(Args&&... args)
	{
		assert(m_queue.size() == m_events.size());
		m_events.emplace_front(std::make_unique<Event>(std::forward<Args>(args)...));
		m_queue.push(m_events.begin());
	}

	bool empty() const
	{
		assert(m_queue.size() == m_events.size());
		return m_queue.empty();
	}

private:
	struct EventCompare
	{
		//参数为了符合格式
		bool operator()(const Itr& lhs, const Itr& rhs) const
		{
			//标准库为最大堆，需要反向比较
			return rhs->get()->tick() < lhs->get()->tick();
		}
	};
private:
	std::priority_queue<Itr, std::vector<Itr>, EventCompare> m_queue;
	//只允许相同的事件独立存在, Event使用智能指针管理，仍然保持多态性
	std::list<std::unique_ptr<Event>> m_events;
};

}	//namespace simulation
