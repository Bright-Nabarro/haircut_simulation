#include <format>
#include "exception.hpp"

using namespace std;

namespace simulation
{

void check_time_factor(double timeFactor)
{
	if (timeFactor < 0 || timeFactor > 3)
		throw InvalidTimeFactor { 
			format("Invalid time factor: {}. Time factor must be between 0 and 3.", timeFactor)
		};
}

}		//namespace simulation
