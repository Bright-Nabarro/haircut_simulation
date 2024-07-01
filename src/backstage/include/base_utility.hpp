#pragma once
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <string>

namespace simulation
{

struct Hms 
{
	size_t hour;
	size_t min;
	size_t sec;
};

Hms cvt_seconds_to_hms(size_t seconds);


class Tick
{
public:
	//注意：初始化Tick时禁止在数字头部填充0
	Tick(size_t h, size_t m, size_t s);
	explicit Tick(const Hms& hms);
	virtual ~Tick() = default;

	[[nodiscard]]
	bool operator==(const Tick& rhs) const noexcept;
	
	[[nodiscard]]
	auto operator<=>(const Tick& rhs) const noexcept -> std::strong_ordering;

	void increament(size_t h, size_t m, size_t s);

	void increament(size_t m, size_t s)
	{ increament(0, m, s); }

	void increament(size_t s)
	{ increament(0, 0, s); }

	std::string to_string() const;

private:
	const Hms& tick2hms() const;
	static uint64_t hms2tick(const Hms& hms);
	static void check_valid(size_t h, size_t m, size_t s);

	//不会删除元素
	inline static std::unordered_map<uint64_t, Hms> s_tick2hmsTable {};
	uint64_t m_timestamp;
};


/*
 * 模板类型仅作为不同类的内部计数器的区分
 *
 */
template<typename UserClass>
class Id
{
public:
	Id() : m_id { s_idCounter++ }
	{ }

	virtual ~Id() = default;

	Id(const Id&) = delete;

	Id& operator=(const Id&) = delete;

	Id(Id&& rhs) : m_id { rhs.m_id }
	{ }

	Id& operator=(Id&& rhs)
	{
		m_id = rhs.m_id;
		return *this;
	}

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
	{ return m_id <=> rhs.m_id; }

private:
	inline static size_t s_idCounter {0};
	size_t m_id;
};

template<typename UserClass>
bool operator==(const std::reference_wrapper<const Id<UserClass>>& lhs,
				const std::reference_wrapper<const Id<UserClass>>& rhs)
{
	return lhs.get() == rhs.get();
}

enum class Level { BEG = 0, INT = 1, ADV = 2, FAST = 3 };


}		//namespace simulation


namespace std
{
	template<typename UserClass>
	struct hash<simulation::Id<UserClass>>
	{
		size_t operator()(const simulation::Id<UserClass>& rhs) const
		{
			return hash<size_t>{}(rhs.get_id_number());
		}
	};

	//注意带有const限定
	template<typename UserClass>
	struct hash<std::reference_wrapper<const simulation::Id<UserClass>>>
	{
		size_t operator()(const std::reference_wrapper<const simulation::Id<UserClass>>& rhs) const
		{
			return hash<simulation::Id<UserClass>>{}(rhs.get());
		}
	};
}		//namespace std ( for specialize hash<Id<Ty>> and its reference_wrapper )



