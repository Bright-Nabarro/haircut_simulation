#pragma once
#include <map>
#include "base_utility.hpp"

namespace simulation
{

void load_parameter(
	size_t& customerNum,								//顾客人数
	size_t& baseTime,									//基准理发时间
	std::pair<double, double>& timeFactorRange,			//顾客理发时间系数
	std::pair<size_t, size_t>& maxBearingLenRange,		//顾客最大忍受队列长度
	std::map<Level, double>& feeSchedule,				//理发店收费标准
	std::pair<size_t, size_t>& openingHours				//理发营业时间
);

void save_parameter(
    size_t customerNum,
    size_t baseTime,
    const std::pair<double, double>& timeFactorRange,
    const std::pair<size_t, size_t>& maxBearingLenRange,
    const std::map<Level, double>& feeSchedule,
	std::pair<size_t, size_t>& openingHours				//理发营业时间
);

}	// namespace simulation
