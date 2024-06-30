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
protected:
    bool m_verbose;
};

class DerivedTestEvent : public TestEvent
{
public:
    DerivedTestEvent(int t, bool verbose = true) : TestEvent(t, verbose) {}

    void execve() const override
    {
        if (m_verbose)
        {
            std::println("Executing derived event at tick: {}", tick());
        }
    }
};

TPF
void test_em_ini()
{
	using namespace simulation;
    using namespace std;

    // 定义一个EventManager
    EventManager<TestEvent> eventManager;

    // 创建并测试emplace接口
    eventManager.emplace<TestEvent>(10, false);
    eventManager.emplace<DerivedTestEvent>(20, false);

    // 测试execve接口
    eventManager.execve();  // 应该执行TestEvent
    eventManager.execve();  // 应该执行DerivedTestEvent
}

TPF
void test_em_push()
{
	using namespace simulation;
    using namespace std;

    // 定义一个EventManager
    EventManager<TestEvent> eventManager;

    // 创建多个TestEvent和DerivedTestEvent对象并使用push接口添加
    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0) {
            TestEvent event(i, false);
            eventManager.emplace<TestEvent>(event);
        } else {
            DerivedTestEvent event(i, false);
            eventManager.emplace<DerivedTestEvent>(event);
        }
    }

    // 检查是否不为空
    assert(!eventManager.empty());

    // 测试execve接口，确保所有事件都能按顺序执行
    for (int i = 0; i < 50; ++i) {
        eventManager.execve();
    }

    // 检查是否为空
    assert(eventManager.empty());
}

TPF
void test_em_execve(bool display = false)
{
	using namespace simulation;
    using namespace std;

    // 定义一个EventManager
    EventManager<TestEvent> eventManager;

    // 创建多个TestEvent和DerivedTestEvent对象并使用emplace接口添加
    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0) {
            eventManager.emplace<TestEvent>(i, display);
        } else {
            eventManager.emplace<DerivedTestEvent>(i, display);
        }
    }

    // 检查是否不为空
    assert(!eventManager.empty());

    // 测试execve接口，确保所有事件都能按顺序执行
    for (int i = 0; i < 50; ++i) {
        eventManager.execve();
    }

    // 检查是否为空
    assert(eventManager.empty());
}
