#include <format>
#include "base_utility.hpp"
#include "exception.hpp"

namespace simulation
{
using namespace std;

Hms cvt_seconds_to_hms(size_t totalSeconds)
{	
	//具体的面向用户的错误检查由工厂完成
	size_t hour { totalSeconds / 3600 };
	totalSeconds %= 3600;
	size_t min { totalSeconds / 60 };
	size_t sec { totalSeconds % 60 };
	return Hms { .hour = hour, .min = min, .sec = sec };
}

Tick::Tick(size_t h, size_t m, size_t s) :
	Tick( Hms{.hour = h, .min = m, .sec = s } )
{ }

bool Tick::operator==(const Tick& rhs)const noexcept
{
	return m_timestamp == rhs.m_timestamp;
}

auto Tick::operator<=>(const Tick& rhs)const noexcept
-> std::strong_ordering 
{
	return m_timestamp <=> rhs.m_timestamp;
}

void Tick::increament(size_t h, size_t m, size_t s)
{
	decltype(auto) thisHms { tick2hms() };
	size_t hour = thisHms.hour + h;
	size_t min = thisHms.min + m;
	size_t sec = thisHms.sec + s;
	if (hour > 24)
	{
		throw logic_error { "Hour increament over 24" };
	}

	if ( sec >= 60 )
	{
		min++;
		sec -= 60;
	}
	if ( min >= 60 )
	{
		hour++;
		min -= 60;
	}

	Hms hms {
		.hour = hour,
		.min = min,
		.sec = sec,
	};

	m_timestamp = hms2tick(hms);
	s_tick2hmsTable[m_timestamp] = hms;
}

std::string Tick::to_string() const
{
	Hms hms { tick2hms() };
	return std::format("{:02}:{:02}:{:02}", hms.hour, hms.min, hms.sec);
}

Tick::Tick(const Hms& hms) :
	m_timestamp {0}
{
	check_valid(hms.hour, hms.min, hms.sec);	
	m_timestamp = hms2tick(hms);
	s_tick2hmsTable[m_timestamp] = hms;
}

auto Tick::tick2hms() const -> const Hms&
{	
	auto itr { s_tick2hmsTable.find(m_timestamp) };
	if (itr == s_tick2hmsTable.end())
		throw std::logic_error { format("m_timestamp: {} cannot find hms in hashmap", m_timestamp) };
	
	return itr->second;
}

uint64_t Tick::hms2tick(const Hms& hms)
{
	return hms.sec | (hms.min << 6) | (hms.hour << 12 );
}

void Tick::check_valid(size_t h, size_t m, size_t s)
{
	if (h > 24)
		throw InvalidTimepoint { format("input hours: {} invalid", h) };

	if (m >= 60)
		throw InvalidTimepoint { format("input minutes: {} invalid", m) };

	if (s >= 60)
		throw InvalidTimepoint { format("input second: {} invalid", s) };
}



}		//namespace simulation

