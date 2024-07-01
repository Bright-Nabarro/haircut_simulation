#pragma once
#include <string_view>

namespace simulation
{
//直接输出到控制台
class Display
{
public:
	void operator()(std::string_view sv);
};

}	//namespace simulation
