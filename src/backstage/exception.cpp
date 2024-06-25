#include <format>
#include "exception.hpp"

using namespace std;

namespace simulation
{

void check_time_factor(double timeFactor)
{
	if (timeFactor < 0)
		throw InvalidTimepoint { format("Invalid timeFactor {}", timeFactor) };
}

}		//namespace simulation
