#include <print>
#include "display.hpp"
#include "initial.hpp"
#ifdef TMP_DEBUG
#include "factory.hpp"
#include "event.hpp"
#include "display.hpp"
#endif
#include "business_logic.hpp"
using namespace std;
using namespace simulation;

static void initial_data(int argc, char* argv[], MainObjManager& objManager,
		MainEventManager& eventManager, Display& display, 
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager 
);


int main(int argc, char* argv[])
{
	MainObjManager objManager;
	MainEventManager eventManager;
	Display display;
	MainCustWaitingQue customerWaitingQue { objManager };
    MainBarberManager barberManager { objManager };
	MainChairManager chairManager { objManager };
	
	
	
#ifdef TMP_DEBUG
	CustomerFactory custFactory{ objManager };
	BarberFactory barberFactory{ objManager };
	ChairFactory chairFactory{ objManager };
	barberFactory.create_barber(Level::BEG, 1, 0);
	chairFactory.create_chair();
	auto cust1 = custFactory.create_customer(Level::BEG, 1, 100);
	Tick t1(8, 19, 9);
	auto cust2 = custFactory.create_customer(Level::BEG, 1, 100);
	Tick t2(8, 37, 13);
	auto cust3 = custFactory.create_customer(Level::BEG, 1, 100);
	Tick t3(10, 7, 7);
	auto cust4 = custFactory.create_customer(Level::BEG, 1, 100);
	Tick t4(10, 18, 10);
	assert(t2 < t3);

	barberManager.update();
	chairManager.update();
	eventManager.emplace<CustomerArrivalEvent>(
		objManager,
		eventManager,
		customerWaitingQue,
		barberManager,
		chairManager,
		t1,
		600,
		[&display](string_view sv){ display(sv); },
		cust1->get_id());

	eventManager.emplace<CustomerArrivalEvent>(objManager, eventManager,
		customerWaitingQue, barberManager, chairManager, t2, 600,
		[&display](string_view sv){ display(sv); }, cust2->get_id());
	eventManager.emplace<CustomerArrivalEvent>(
		objManager,
		eventManager,
		customerWaitingQue,
		barberManager,
		chairManager,
		t3,
		600,
		[&display](string_view sv){ display(sv); },
		cust3->get_id());
	eventManager.emplace<CustomerArrivalEvent>(
		objManager,
		eventManager,
		customerWaitingQue,
		barberManager,
		chairManager,
		t4,
		600,
		[&display](string_view sv){ display(sv); },
		cust4->get_id());
	while(!eventManager.empty())
		eventManager.execve();
#else
	initial_data(argc, argv, objManager, eventManager, display,
			customerWaitingQue, barberManager, chairManager);
	return main_loop(objManager, eventManager);
#endif
}

static void initial_data(int argc, char* argv[], MainObjManager& objManager,
		MainEventManager& eventManager, Display& display,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager)
{ 
	initial_parameter();
	load_saved_data(objManager);
	barberManager.update();
	chairManager.update();
	handle_argument(argc, argv, objManager);
	generate_initial_event(objManager, eventManager,
			[&display](string_view sv){ display(sv); },
			customerWaitingQue, barberManager, chairManager);
}
