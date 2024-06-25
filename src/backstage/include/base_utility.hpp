#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace simulation
{

struct Hms 
{
	uint hour;
	uint min;
	uint sec;
};


class Tick
{
public:
	//注意：初始化Tick时禁止填充0
	explicit Tick(uint h, uint m, uint s);
	virtual ~Tick() = default;

	[[nodiscard]]
	bool operator==(const Tick& rhs) const noexcept;
	
	[[nodiscard]]
	auto operator<=>(const Tick& rhs) const noexcept -> std::strong_ordering;

	void increament(uint h, uint m, uint s);

	void increament(uint m, uint s)
	{ increament(0, m, s); }

	void increament(uint s)
	{ increament(0, 0, s); }

private:
	const Hms& tick2hms() const;
	static uint64_t hms2tick(const Hms& hms);
	static void check_valid(uint h, uint m, uint s);

	//不会删除元素
	inline static std::unordered_map<uint64_t, Hms> s_tick2hmsTable {};
	uint64_t m_timestamp;
};


template<typename UserClass>
class Id
{
public:
	Id() : m_id { s_idCounter++ }
	{ }

	virtual ~Id() = default;

	Id(const Id&) = delete;

	Id& operator=(const Id&) = delete;

	[[nodiscard]]
	size_t get_id_number() const noexcept
	{ return m_id; }

	//复制构造删除之后的比较仍然必要
	[[nodiscard]]
	bool operator==(const Id& rhs) const noexcept
	{ return m_id == rhs.m_id; }

	[[nodiscard]]
	auto operator<=>(const Id& rhs) const noexcept
	-> std::strong_ordering
	{ return m_id == rhs.m_id; }

private:
	inline static size_t s_idCounter {0};
	const size_t m_id;
};

enum class Level { BEG = 0, INT = 1, ADV = 2, FAST = -1 };


}		//namespace simulation
