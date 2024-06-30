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

    // 创建一些TestEvent对象
    TestEvent event1(10, false);

    // 向EventManager中添加事件
    eventManager.push(event1);
	eventManager.execve();
}

TPF
void test_em_push()
{
	using namespace std;
	using namespace simulation;

    // 定义一个EventManager
    EventManager<TestEvent> eventManager;

    // 创建一些TestEvent对象
    TestEvent event1(10, false);
    TestEvent event2(20, false);
    TestEvent event3(5, false);
    TestEvent event4(15, false);

    // 向EventManager中添加事件
    eventManager.push(event1);
    eventManager.push(event2);
    eventManager.push(event3);
    eventManager.push(event4);

    // 检查EventManager是否不为空
    assert(!eventManager.empty());
}

TPF
void test_em_execve(bool display = false)
{
	using namespace std;
	using namespace simulation;
	// 定义一个EventManager
    EventManager<TestEvent> eventManager;

    // 创建一些TestEvent对象
    TestEvent event1(10, display);
    TestEvent event2(20, display);
    TestEvent event3(5, display);
    TestEvent event4(15, display);

    // 向EventManager中添加事件
    eventManager.push(event1);
    eventManager.push(event2);
    eventManager.push(event3);
    eventManager.push(event4);

    // 执行事件，并检查事件的顺序
    eventManager.execve(); // 应执行tick为5的事件
    assert(event3.tick() == 5);

    eventManager.execve(); // 应执行tick为10的事件
    assert(event1.tick() == 10);

    eventManager.execve(); // 应执行tick为15的事件
    assert(event4.tick() == 15);

    eventManager.execve(); // 应执行tick为20的事件
    assert(event2.tick() == 20);

    // 检查EventManager是否为空
    assert(eventManager.empty());

    // 确保异常被正确抛出
    try
    {
        eventManager.execve();
        assert(false); // 不应到达这里
    }
    catch (const logic_error& e)
    {
        assert(string(e.what()) == "No events in EventManager");
    }
}
