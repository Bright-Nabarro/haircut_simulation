#include <print>
#include <random>
#include <map>
#include "initial.hpp"
#include "io_barchr.hpp"
#include "io_parameter.hpp"
#include "factory.hpp"
using namespace simulation;
using namespace std;

static const pair<int, int> g_kLevelRange { 0, 3 };

static size_t g_customerNum { 10 };
static size_t g_baseTime { 600 };
static pair<double, double> g_timeFactorRange { 0.6, 1.7 };
static pair<size_t, size_t> g_maxBearingLenRange { 2, 10 };
static map<Level, double> g_feeSchedule {
	{ Level::BEG, 25 },
	{ Level::INT, 50 },
	{ Level::ADV, 100 }
};
static pair<size_t, size_t> g_openingHours { 7, 20 };

static int parser(int argc, char* argv[]);
static Tick random_tick();

void initial_parameter()
{
	try
	{
		load_parameter(g_customerNum, g_baseTime, g_timeFactorRange,
				g_maxBearingLenRange, g_feeSchedule, g_openingHours);
	} catch(runtime_error& e)
	{
		println(stderr, "{}", e.what());
		println(stderr, "use program default settings");
	} catch(...)
	{
		throw;
	}
}

void load_saved_data()
{
	load_barber();
	load_chair();
}

void setting_parameter()
{
}

void generate_initial_event(
		MainEventManager& eventManager, function<void(string_view sv)> output,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager,
		simulation::SimStatistics& stics)
{
	static random_device rd;
	static mt19937 gen { rd() };
	uniform_real_distribution<double> distTimeFactor { 
		g_timeFactorRange.first, g_timeFactorRange.second
	};
	uniform_int_distribution<int> distLevelNum {
		g_kLevelRange.first, g_kLevelRange.second
	};
	uniform_int_distribution<size_t> distMaxBearingLen {
		g_maxBearingLenRange.first, g_maxBearingLenRange.second
	};
	
	CustomerFactory customerFactory;

	for(size_t i {0}; i < g_customerNum; i++)
	{
		auto pCustomer = customerFactory.create_customer(
			static_cast<Level>(distLevelNum(gen)), distTimeFactor(gen),
			distMaxBearingLen(gen));
		EventHandlerComponents cpts {
			eventManager,
			customerWaitingQue,
			barberManager,
			chairManager,
			static_cast<double>(g_baseTime),
			g_feeSchedule,
			output,
			stics
		};
		eventManager.emplace<CustomerArrivalEvent>(
			cpts,
			random_tick(),
			pCustomer->get_id()
		);
	}
}

Tick close_door_tick()
{
	return Tick{g_openingHours.second, 0, 0};
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
	
	std::uniform_int_distribution<size_t> distMinSec { 0, 59 };
	std::uniform_int_distribution<size_t> distHour { g_openingHours.first, g_openingHours.second-1 };
	size_t hour { distHour(gen) };
	size_t min { distHour(gen) };
	size_t sec { distHour(gen) };
	return Tick { hour, min, sec };
}

