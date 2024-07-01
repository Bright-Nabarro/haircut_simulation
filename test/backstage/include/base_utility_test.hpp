#pragma once
#include <unordered_set>
#include "base_utility.hpp"
#include "test_header.hpp"


TPF
void test_Tick_ini()
{
	using namespace simulation;
	using namespace std;
	Tick t1(1, 30, 45);
	assert(t1 == Tick(1, 30, 45));

	Hms hms1 {10, 20, 30};
    Tick tick1(hms1);

    // 定义一个 Hms 对象
    Hms hms2 {10, 20, 30};
    Tick tick2(hms2);

    // 使用相同的时间初始化另一个 Tick 对象
    Tick tick3(10, 20, 30);

    // 检查两个 Tick 对象是否相等（假设有一个相等运算符）
    assert(tick3 == tick2);
}

TPF
void test_Tick_cpr()
{
	using namespace simulation;
	using namespace std;

	// 测试Tick的比较运算符
	Tick t1(1, 30, 45);
	Tick t2(2, 15, 30);
	assert((t1 < t2) == true);
	assert((t2 > t1) == true);
	assert((t1 == t2) == false);
}

TPF
void test_Tick_inc()
{
	using namespace simulation;
	using namespace std;

	// 测试Tick的increament函数
	Tick t1(1, 30, 45);
	t1.increament(0, 29, 15); // 增加29分钟15秒
	assert(t1 == Tick(2, 0, 0)); // 2:00:00

	t1.increament(1, 0, 0); // 增加1小时
	assert(t1 == Tick(3, 0, 0)); // 3:00:00

	t1.increament(0, 30); // 增加30秒
	assert(t1 == Tick(3, 0, 30)); // 3:00:30

	t1.increament(45); // 增加45秒
	assert(t1 == Tick(3, 1, 15)); // 3:01:15
}

TPF
void test_cvts2h()
{
    using namespace simulation;

    // 测试转换函数
    size_t seconds = 3661; // 1 hour, 1 minute, 1 second
    Hms hms = cvt_seconds_to_hms(seconds);

    // 检查转换结果
    assert(hms.hour == 1);
    assert(hms.min == 1);
    assert(hms.sec == 1);

    // 测试不同的秒数
    seconds = 7322; // 2 hours, 2 minutes, 2 seconds
    hms = cvt_seconds_to_hms(seconds);

    assert(hms.hour == 2);
    assert(hms.min == 2);
    assert(hms.sec == 2);

    // 测试边界情况
    seconds = 0; // 0 hours, 0 minutes, 0 seconds
    hms = cvt_seconds_to_hms(seconds);

    assert(hms.hour == 0);
    assert(hms.min == 0);
    assert(hms.sec == 0);

    // 测试仅有秒数
    seconds = 59; // 0 hours, 0 minutes, 59 seconds
    hms = cvt_seconds_to_hms(seconds);

    assert(hms.hour == 0);
    assert(hms.min == 0);
    assert(hms.sec == 59);

    // 测试分钟和秒数
    seconds = 3600; // 1 hour, 0 minutes, 0 seconds
    hms = cvt_seconds_to_hms(seconds);

    assert(hms.hour == 1);
    assert(hms.min == 0);
    assert(hms.sec == 0);

    // 测试大于一天的秒数
    seconds = 90061; // 25 hours, 1 minute, 1 second (extra hours)
    hms = cvt_seconds_to_hms(seconds);

    assert(hms.hour == 25);
    assert(hms.min == 1);
    assert(hms.sec == 1);
}

TPF
void test_Id_ini()
{
	using namespace simulation;

	// 测试Id的初始化
	Id<int> id1;
	Id<int> id2;

	// 每个Id实例应该有不同的id_number
	assert(id1.get_id_number() != id2.get_id_number());
	assert(id1 == id1);
	assert(!(id1 == id2));
}

TPF
void test_Id_get_number(bool display = false)
{
	using namespace simulation;

	// 测试Id的get_id_number方法
	Id<int> id1{};
	Id<int> id2{};

	size_t id1_number = id1.get_id_number();
	size_t id2_number = id2.get_id_number();

	assert(id1_number != id2_number);

	if (display) {
		std::print("ID1 number: {}\n", id1_number);
		std::print("ID2 number: {}\n", id2_number);
	}
}


TPF
void test_Id_cpr()
{
	using namespace simulation;

	// 测试Id的比较运算符
	Id<int> id1{};
	Id<int> id2{};
	Id<int> id3{std::move(id1)};

	assert(id1 != id2);
	assert(id1 == id3);
	assert(id1.get_id_number() != id2.get_id_number());
	assert(id1.get_id_number() == id3.get_id_number());

	assert(id1 < id2 || id1 > id2);  // 确保两个不同的Id实例有顺序关系
}

TPF
void test_Id_ownership(bool display = false)
{
	using namespace simulation;

	// 测试Id的所有权转移
	Id<int> id1{};
	Id<int> id2{};

	if (display) {
		std::println("Before move:");
		std::println("ID1 number: {}", id1.get_id_number());
		std::println("ID2 number: {}", id2.get_id_number());
	}

	Id<int> id3{std::move(id1)};
	id2 = std::move(id3);
	if (display) {
		std::println("After move:");
		std::println("ID1 number: {}", id1.get_id_number()); // 可能未定义行为，不建议使用
		std::println("ID2 number: {}", id2.get_id_number());
		std::println("ID3 number: {}", id3.get_id_number()); // 可能未定义行为，不建议使用
	}

	// 由于移动后id1和id3的状态不确定，我们不对它们进行进一步的断言测试
	assert(id2.get_id_number() == id1.get_id_number() || id2.get_id_number() == id3.get_id_number());
}

TPF
void test_hash_equal_Id(bool display = false)
{
	using namespace simulation;
	using namespace std;

	// 测试Id的哈希功能
	Id<int> id1{};
	Id<int> id2{};
	Id<int> id3{};

	hash<Id<int>> hasher;
	size_t hash1 = hasher(id1);
	size_t hash2 = hasher(id2);
	size_t hash3 = hasher(id3);

	assert(hash1 != hash2);
	assert(hash1 != hash3);
	assert(hash2 != hash3);

	if (display) {
		std::println("ID1 number: {}, Hash: {}", id1.get_id_number(), hash1);
		std::println("ID2 number: {}, Hash: {}", id2.get_id_number(), hash2);
		std::println("ID3 number: {}, Hash: {}", id3.get_id_number(), hash3);
	}

	// Testing with reference_wrapper
	unordered_set<reference_wrapper<const Id<int>>> ref_set1;
	ref_set1.emplace(id1);
	ref_set1.emplace(id2);
	ref_set1.emplace(id3);

	assert(ref_set1.size() == 3);
	assert(ref_set1.contains(cref(id1)));
	assert(ref_set1.contains(cref(id2)));
	assert(ref_set1.contains(cref(id3)));

	if (display) {
		std::println("Reference Set size: {}", ref_set1.size());
	}

	// 创建大规模的Id对象
	const size_t num_ids = 10000;
	vector<Id<int>> ids;
	ids.reserve(num_ids);

	for (size_t i = 0; i < num_ids; ++i) {
		ids.emplace_back();
	}

	// 测试哈希和相等比较功能
	unordered_set<reference_wrapper<const Id<int>>> ref_set2;
	for (const auto& id : ids) {
		ref_set2.emplace(id);
	}

	assert(ref_set2.size() == num_ids);

	for (const auto& id : ids) {
		assert(ref_set2.contains(cref(id)));
	}

	if (display) {
		std::println("Reference Set size: {}", ref_set2.size());
	}
}
