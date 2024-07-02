#include "factory.hpp"
#include "exception.hpp"

namespace simulation
{

using namespace std;

[[maybe_unused]]
std::shared_ptr<Customer> CustomerFactory::create_customer(Level level, double timeFactor,
		size_t maxBearingLen)
{
	check_time_factor(timeFactor);

	return create_obj(level, timeFactor, maxBearingLen);
}

[[maybe_unused]]
std::shared_ptr<Barber> BarberFactory::create_barber(Level level, double timeFactor, double totalTime)
{
	if (level == Level::FAST)
		throw InvalidLevel { "Invalid Level FAST in Barber constructor" };
	check_time_factor(timeFactor);
	return create_obj(level, timeFactor, totalTime);
}

[[maybe_unused]]
std::shared_ptr<Chair> ChairFactory::create_chair()
{
	return create_obj();
}

}	//namespace simulation
