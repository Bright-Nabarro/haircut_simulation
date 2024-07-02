#include <print>
#include <cstring>
#include "display.hpp"
#include "initial.hpp"
#include "business_logic.hpp"
using namespace std;
using namespace simulation;

static void initial_data(MainObjManager& objManager,
		MainEventManager& eventManager, Display& display, 
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager, SimStatistics& stics
);


int main(int argc, char* argv[])
{
	MainObjManager objManager;
	MainEventManager eventManager;
	MainCustWaitingQue customerWaitingQue { objManager };
    MainBarberManager barberManager { objManager };
	MainChairManager chairManager { objManager };
	SimStatistics stics;
	Display display { objManager, customerWaitingQue, true };
	
	initial_data(objManager, eventManager, display,
			customerWaitingQue, barberManager, chairManager, stics);

	if (argc > 1)
	{
		println("program use step mode");
		if (strcmp(argv[1], "-n") == 0)
			set_step();
	}

	if ( auto ret { main_loop(objManager, eventManager, stics) }; ret != 0 )
		return ret;

	display_stics(objManager, stics);
}

static void initial_data(MainObjManager& objManager,
		MainEventManager& eventManager, Display& display,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager, SimStatistics& stics)
{ 
	initial_parameter();
	load_saved_data(objManager);
	barberManager.update();
	chairManager.update();
	setting_parameter();
	generate_initial_event(objManager, eventManager,
			[&display](string_view sv){ display(sv); },
			customerWaitingQue, barberManager, chairManager, stics);
}
