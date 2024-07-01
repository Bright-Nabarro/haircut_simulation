#include "initial.hpp"
#include "business_logic.hpp"
using namespace std;
using namespace simulation;

static void initial_data(int argc, char* argv[], MainObjManager& objManager);

int main(int argc, char* argv[])
{
	MainObjManager objManager;
	initial_data(argc, argv, objManager);
	return main_loop(objManager);
}

static void initial_data(int argc, char* argv[], MainObjManager& objManager)
{
	load_saved_data(objManager);
	handle_argument(argc, argv, objManager);
	generate_customer(objManager);

}
