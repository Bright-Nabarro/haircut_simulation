#pragma once
#include "base_utility.hpp"

namespace simulation
{

class Event
{
public:
	Event(const Tick& tick): m_tick { tick }
	{ }
	virtual ~Event() = default;
	virtual void execve() const = 0;
private:
	Tick m_tick;
};


class CustomerArrivalEvent: public Event
{
public:
	void execve() const override;
private:
};


class StartHaircutEvent: public Event
{
public:
	void execve() const override;
private:
};


class CustomerLeaveEvent: public Event
{
public:
	void execve() const override;
private:
};


class CompleteHaircutEvent: public Event
{
public:
	void execve() const override;
private:
};

}	//namespace simulation
