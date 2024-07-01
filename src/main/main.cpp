#include <print>
#include "display.hpp"
#include "initial.hpp"
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
	
	
	initial_data(argc, argv, objManager, eventManager, display,
			customerWaitingQue, barberManager, chairManager);
	return main_loop(objManager, eventManager);
}

static void initial_data(int argc, char* argv[], MainObjManager& objManager,
		MainEventManager& eventManager, Display& display,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager)
{ 
	load_saved_data(objManager);
	handle_argument(argc, argv, objManager);
	generate_initial_event(objManager, eventManager,
			[&display](string_view sv){ display(sv); },
			customerWaitingQue, barberManager, chairManager);
}
