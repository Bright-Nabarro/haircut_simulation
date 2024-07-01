#include <print>
#include "display.hpp"

namespace simulation
{

void Display::operator()(std::string_view sv)
{
	std::println("{}", sv.data());
}

}	//namespace simulation
