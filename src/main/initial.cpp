#include <random>
#include "initial.hpp"
#include "io_barchr.hpp"
#include "factory.hpp"
using namespace simulation;
using namespace std;

static int parser(int argc, char* argv[]);
static void generate_customer_event(MainObjManager& objManager,
		MainEventManager& eventManager);
static Tick random_tick();

size_t g_customerNum { 100 };
size_t g_baseTime { 600 };

void load_saved_data(MainObjManager& objManager)
{
	load_barber(objManager);
	load_chair(objManager);
}

void handle_argument(int argc, char* argv[], MainObjManager& objManager)
{
	switch(parser(argc, argv))
	{
	}
}

void generate_initial_event(MainObjManager& objManager,
		MainEventManager& eventManager)
{
}

static void generate_customer_event(MainObjManager& objManager,
		MainEventManager& eventManager)
{
	static random_device rd;
	static mt19937 gen { rd() };
	uniform_real_distribution<double> distTimeFactor { 0.6, 1.7 };
	uniform_int_distribution<int> distLevelNum {0, 3};
	uniform_int_distribution<size_t> distMaxWaitingTime { 100, 7200 };
	
	CustomerFactory customerFactory { objManager };

	for(size_t i {0}; i < g_customerNum; i++)
	{
		auto pCustomer = customerFactory.create_customer(
			static_cast<Level>(distLevelNum(gen)), distTimeFactor(gen),
			distMaxWaitingTime(gen));
		eventManager.emplace<CustomerArrivalEvent>(objManager,
				eventManager,
				random_tick(),
				g_baseTime,
				nullptr,
				pCustomer->get_id()
		);
	}
}

static int parser(int argc, char* argv[])
{
	if (argc <= 1)
		return 0;
	return 1;
}
