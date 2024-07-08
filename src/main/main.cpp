#include <print>
#include <cstring>
#include "display.hpp"
#include "initial.hpp"
#include "business_logic.hpp"
using namespace std;
using namespace simulation;

static void initial_data(
		MainEventManager& eventManager, Display& display, 
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager, SimStatistics& stics
);


int main(int argc, char* argv[])
{
	MainEventManager eventManager;
	MainCustWaitingQue customerWaitingQue;
    MainBarberManager barberManager;
	MainChairManager chairManager;
	SimStatistics stics;
	Display display { customerWaitingQue, true };
	
	initial_data(eventManager, display,
			customerWaitingQue, barberManager, chairManager, stics);

	if (argc > 1)
	{
		println("program use step mode");
		if (strcmp(argv[1], "-n") == 0)
			set_step();
	}

	if ( auto ret { main_loop(eventManager, stics) }; ret != 0 )
		return ret;

	display_stics(stics);
}

static void initial_data(
		MainEventManager& eventManager, Display& display,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager, SimStatistics& stics)
{ 
	initial_parameter();
	load_saved_data();
	barberManager.update();
	chairManager.update();
	setting_parameter();
	generate_initial_event(eventManager, [&display](string_view sv){ display(sv); },
			customerWaitingQue, barberManager, chairManager, stics);
}
