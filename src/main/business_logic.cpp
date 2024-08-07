#include <print>
#include <cmath>
#include <iostream>
#include "business_logic.hpp"
using namespace std;
using namespace simulation;

static bool g_useStep { false };

void printManager(MainObjManager& objManager)
{
	println("{{");
	//Customer
	for(auto itr { objManager.cbegin<Customer>() };
		itr != objManager.cend<Customer>();
		++itr)
	{
		const auto& p { itr->second };
		println("\tCustomer {}, factor time {}, maxWaiting time {}",
				p->get_id().get_id_number(), p->get_time_factor(), p->get_max_bearing_len());
	}
	//Barber
	for(auto itr { objManager.cbegin<Barber>() };
	itr != objManager.cend<Barber>();
	++itr)
	{
		const auto& p { itr->second };
		println("\tBarber {}, factor time {}, level {}",
				p->get_id().get_id_number(), p->get_time_factor(), static_cast<int>(p->get_level()));
	}
	//Chair
	for(auto itr { objManager.cbegin<Chair>() };
	itr != objManager.cend<Chair>();
	++itr)
	{
		const auto& p { itr->second };
		println("\tChair {}", p->get_id().get_id_number());
	}
	
	println("}}");
}

void set_step()
{
	g_useStep ^= 1;
}

int main_loop(MainEventManager& eventManager, SimStatistics& stics)
{
	Tick lastTick {0, 0, 0};
	while(!eventManager.empty())
	{
		if (g_useStep)
			print("\033c");
		//printManager(objManager);
		eventManager.execve();
		if (eventManager.size() == 1)
		{
			lastTick = eventManager.get_next_event_tick();
		}
		if (g_useStep)
		{
			string keyEnter;
			getline(cin, keyEnter);
			if (keyEnter == "q" || keyEnter == "quit")
				exit(0);
			if (keyEnter == "c" || keyEnter == "continue")
			{
				set_step();
				println("----------------------------------------------------------------");
			}
		}
		else
		{
			println("----------------------------------------------------------------");
		}
	}

	Tick closeTick { close_door_tick() };
	if (closeTick > lastTick)
		stics.set_wrap_up_time(0);
	else
	{
		size_t diffSeconds {
			hms_to_seconds(lastTick.get_hms()) - hms_to_seconds(closeTick.get_hms())
		};
		stics.set_wrap_up_time(diffSeconds);
	}
	return 0;
}

void display_stics(SimStatistics& stics)
{
	MainObjManager objManager;
	println("\n[Statistics]");
	println("Customer average cost time {} seconds", stics.avg_waiting_time());
	println("Queue average length {}", stics.avg_que_length());
	Hms wrapUpHms = cvt_seconds_to_hms(stics.get_wrap_up_time());
	println("Wrap up time {:02}h {:02}m {:02}s",
			wrapUpHms.hour, wrapUpHms.min, wrapUpHms.sec);
	double  totalWorktime { 0 };
	double incomeBeg {0}, incomeInt{0}, incomeAdv{0};
	for (auto itr { objManager.cbegin<Barber>() };
		 itr != objManager.cend<Barber>();
		 ++itr)
	{
		switch(itr->second->get_level())
		{
		case Level::BEG:
			incomeBeg += itr->second->get_income();
			break;
		case Level::INT:
			incomeInt += itr->second->get_income();
			break;
		case Level::ADV:
			incomeAdv += itr->second->get_income();
			break;
		case Level::FAST:
			break;
		}
		totalWorktime += itr->second->get_total_worktime();
	}
	println("BEG net income {}", incomeBeg);
	println("INT net income {}", incomeInt);
	println("ADV net income {}", incomeAdv);
	println("Total net income {}", incomeBeg + incomeInt + incomeAdv);
	Hms totalWorkHms = cvt_seconds_to_hms(static_cast<size_t>(round(totalWorktime)));
	println("Total barbers work time {:02}h {:02}m {:02}s",
			totalWorkHms.hour, totalWorkHms.min, totalWorkHms.sec);
	
	if (g_useStep)
	{
		println("[Notices]");
		println("Program has ended");
		string _;
		getline(cin, _);
	}
}
