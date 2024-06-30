#pragma once
#include "test_header.hpp"
#include "event_manager.hpp"

class TestEvent
{
public:
    TestEvent(int t, bool verbose = true) : m_tick(t), m_verbose(verbose) {}
    virtual ~TestEvent() = default;

    virtual void execve() const
    {
        if (m_verbose)
        {
            std::println("Executing event at tick: {}", m_tick);
        }
    }

    int tick() const { return m_tick; }

private:
    int m_tick;
    bool m_verbose;
};

TPF
void test_em_ini()
{
	using namespace simulation;
	using namespace std;
	EventManager<TestEvent> eventManager;
    // 测试空的EventManager
    assert(eventManager.empty());
    // 向EventManager中添加事件
    eventManager.emplace(10, false);
	eventManager.empty();
	eventManager.execve();
}

TPF
void test_em_emplace()
{
	using namespace std;
	using namespace simulation;

    // 定义一个EventManager，使用std::priority_queue作为优先队列
    EventManager<TestEvent> eventManager;

    // 测试空的EventManager
    assert(eventManager.empty());

    // 向EventManager中添加事件
    eventManager.emplace(10, false);
    eventManager.emplace(20, false);
    eventManager.emplace(5, false);
    eventManager.emplace(15, false);

    // 检查EventManager是否不为空
    assert(!eventManager.empty());
}

TPF
void test_em_execve(bool display = false)
{
	using namespace std;
	using namespace simulation;

    // 定义一个EventManager，使用std::priority_queue作为优先队列
    EventManager<TestEvent> eventManager;

    // 测试空的EventManager
    assert(eventManager.empty());

    // 向EventManager中添加事件
    eventManager.emplace(10, display);
    eventManager.emplace(20, display);
    eventManager.emplace(5, display);
    eventManager.emplace(15, display);

    // 检查EventManager是否不为空
    assert(!eventManager.empty());
	eventManager.execve();
	eventManager.execve();
	eventManager.execve();
	eventManager.execve();
	try
	{
		eventManager.execve();
		assert(false);
	}
	catch(logic_error& e)
	{
		if (display)
			println("{}", e.what());
	}
	catch(...)
	{
		assert(false);
	}
}
