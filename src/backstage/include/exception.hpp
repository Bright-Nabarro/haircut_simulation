#pragma once
#include <stdexcept>
#include <string_view>

namespace simulation
{

class InvalidTimepoint : public std::runtime_error
{
public:
	InvalidTimepoint(std::string_view sv): std::runtime_error{ sv.data() }
	{ }
};

class InvalidTimeFactor : public std::runtime_error
{
public:
	InvalidTimeFactor(std::string_view sv): std::runtime_error{ sv.data() }
	{ }
};

void check_time_factor(double timeFactor);

}		//namespace simulation
