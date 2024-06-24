#pragma once

class Event
{
public:
	virtual ~Event() = default;
	virtual void execve() const = 0;
private:
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
