#include <format>
#include "factory.hpp"
#include "exception.hpp"

namespace simulation
{

using namespace std;

[[maybe_unused]]
std::shared_ptr<Customer> CustomerFactory::create_customer(Level level, double timeFactor,
		size_t maxWaitingTime)
{
	check_time_factor(timeFactor);
	if (maxWaitingTime >= 60*60*24)
		throw InvalidMaxWaitingTime {
			format("Invalid maximum waiting time: {}."
				   "Maximum waiting time must be less than 24 hours.",
					maxWaitingTime)
		};

	return create_obj(level, timeFactor, maxWaitingTime);
}

[[maybe_unused]]
std::shared_ptr<Barber> BarberFactory::create_barber(Level level, double timeFactor)
{
	if (level == Level::FAST)
		throw InvalidLevel { "Invalid Level FAST in Barber constructor" };
	check_time_factor(timeFactor);
	return create_obj(level, timeFactor);
}

[[maybe_unused]]
std::shared_ptr<Chair> ChairFactory::create_chair()
{
	return create_obj();
}

}	//namespace simulation
