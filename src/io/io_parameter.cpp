#include <filesystem>
#include <fstream>
#include <sstream>
#include <format>
#include "io_parameter.hpp"
using namespace std;
namespace fs = std::filesystem;

namespace simulation
{

static const char* dirPath { "data" };
static const char* saveFilePath { "data/sim.ini" };

static unordered_map<string, size_t> loadMaping {
	{ "customerNum"s,			0 },
	{ "baseTime"s,				1 },
	{ "timeFactorRangeMin"s,	2 },
	{ "timeFactorRangeMax"s,	3 },
	{ "maxBearingLenRangeMin"s,	4 },
	{ "maxBearingLenRangeMax"s,	5 },
	{ "feeScheduleBEG"s,		6 },
	{ "feeScheduleINT"s,		7 },
	{ "feeScheduleADV"s,		8 },
};

void load_parameter(
	size_t& customerNum,
	size_t& baseTime,
	std::pair<double, double>& timeFactorRange,
	std::pair<size_t, size_t>& maxBearingLenRange,
	std::map<Level, double>& feeSchedule)
{
	fs::path dir { dirPath };
	if (!fs::exists(dir))
	{
		if (!fs::create_directories(dir))
			throw runtime_error { format("dir {} file cannot create", dirPath) };
	}

	ifstream inFile { saveFilePath };
	if (!inFile.is_open())
		throw runtime_error { format("Cannot open the file {}", saveFilePath) };

	auto searchTable = [&](const string& key, const string& value) {
		auto itr = loadMaping.find(key);
		if (itr == loadMaping.end())
			return;
		try
		{
			switch (itr->second)
			{
			case 0:
				customerNum = stoul(value);
				break;
			case 1:
				baseTime = stoul(value);
				break;
			case 2:
				timeFactorRange.first = stod(value);
				break;
			case 3:
				timeFactorRange.second = stod(value);
				break;
			case 4:
				maxBearingLenRange.first = stoul(value);
				break;
			case 5:
				maxBearingLenRange.second = stoul(value);
				break;
			case 6:
				feeSchedule[Level::BEG] = stod(value);
				break;
			case 7:
				feeSchedule[Level::INT] = stod(value);
				break;
			case 8:
				feeSchedule[Level::ADV] = stod(value);
				break;
			}
		} catch (...) {
			return;
		}
	};
	string eachLine;
	while (getline(inFile, eachLine))
	{
		istringstream issm { eachLine };
		string key, value;
		if (!getline(issm, key, '='))
			continue;

		if (!getline(issm, value))
			continue;

		searchTable(key, value);
	}
}

void save_parameter(
    size_t customerNum,
    size_t baseTime,
    const std::pair<double, double>& timeFactorRange,
    const std::pair<size_t, size_t>& maxBearingLenRange,
    const std::map<Level, double>& feeSchedule)
{
    fs::path dir { dirPath };
    if (!fs::exists(dir))
    {
        if (!fs::create_directories(dir))
            throw std::runtime_error { std::format("dir {} cannot be created", dirPath) };
    }

    std::ofstream outFile { saveFilePath };
    if (!outFile.is_open())
        throw std::runtime_error { std::format("Cannot open the file {}", saveFilePath) };

    outFile << std::format("customerNum={}\n", customerNum);
    outFile << std::format("baseTime={}\n", baseTime);
    outFile << std::format("timeFactorRangeMin={}\n", timeFactorRange.first);
    outFile << std::format("timeFactorRangeMax={}\n", timeFactorRange.second);
    outFile << std::format("maxBearingLenRangeMin={}\n", maxBearingLenRange.first);
    outFile << std::format("maxBearingLenRangeMax={}\n", maxBearingLenRange.second);

    for (const auto& [level, fee] : feeSchedule) {
        switch (level) {
            case Level::BEG:
                outFile << std::format("feeScheduleBEG={}\n", fee);
                break;
            case Level::INT:
                outFile << std::format("feeScheduleINT={}\n", fee);
                break;
            case Level::ADV:
                outFile << std::format("feeScheduleADV={}\n", fee);
                break;
			case Level::FAST:
				throw logic_error { "FAST cannot be a preread parameter" };
        }
    }

    outFile.close();
}

}	// namespace simulation
