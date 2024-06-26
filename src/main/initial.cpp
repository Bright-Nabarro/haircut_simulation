#include <random>
#include "initial.hpp"
#include "io_barchr.hpp"
#include "factory.hpp"
using namespace simulation;
using namespace std;

static int parser(int argc, char* argv[]);
static Tick random_tick();

size_t g_customerNum { 400 };
size_t g_baseTime { 600 };

void load_saved_data(MainObjManager& objManager)
{
	load_barber(objManager);
	load_chair(objManager);
}

void handle_argument(int argc, char* argv[], MainObjManager& objManager)
{
	(void)objManager;
	switch(parser(argc, argv))
	{
	}
}

void generate_initial_event(MainObjManager& objManager,
		MainEventManager& eventManager, function<void(string_view sv)> output,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager)
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
		eventManager.emplace<CustomerArrivalEvent>(
				objManager,
				eventManager,
				customerWaitingQue,
				barberManager,
				chairManager,
				random_tick(),
				g_baseTime,
				output,
				pCustomer->get_id()
		);
	}
}

static int parser(int argc, char* argv[])
{
	(void)argv;
	if (argc <= 1)
		return 0;
	return 1;
}

static Tick random_tick()
{
	static random_device rd;
	static mt19937 gen { rd() };
	std::normal_distribution<double> hourDistribution { 23., 1. };
	int hourInt { static_cast<int>(hourDistribution(gen)) };
    size_t hour = (hourInt < 0) ? 0 : (hourInt >= 24 ? 23 : static_cast<size_t>(hourInt));
	
	std::uniform_int_distribution<size_t> uniformDist { 0, 59 };
	size_t min { uniformDist(gen) };
	size_t sec { uniformDist(gen) };
	return Tick { hour, min, sec };
}
