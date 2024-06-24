#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace simulation
{

class Tick
{
	struct Hms;
public:
	explicit Tick(uint h, uint m, uint s);

	[[nodiscard]]
	bool operator==(const Tick& rhs) const noexcept;
	
	[[nodiscard]]
	auto operator<=>(const Tick& rhs) const noexcept;

	[[nodiscard]]
	std::string to_string() const;

	void increament(uint h, uint m, uint s);

	void increament(uint m, uint s)
	{ increament(0, m, s); }

	void increament(uint s)
	{ increament(0, 0, s); }

private:
	const Hms& tick2hms() const;
	static uint64_t hms2tick(const Hms& hms);
	static void check_valid(uint h, uint m, uint s);

	struct Hms 
	{
		uint hour;
		uint min;
		uint sec;
	};

	//不会删除元素
	inline static std::unordered_map<uint64_t, Hms> s_tick2hmsTable {};
	uint64_t m_timestamp;
};

}		//namespace simulation
