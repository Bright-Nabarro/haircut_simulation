# base_utility
## class: `Hms`
- 三个成员分别表示小时，分钟，时间

## class: `Tick`
- 表示时间戳
- 提供全向的比较
- 只能通过increament递增

## class: `Id<ty>`
- 提供标识（用于hash索引）
- 无法复制，保证存在唯一
- 提供全向的比较
- 提供哈希特化
- 可以通过常引用包装来进行引用
- 提供常引用包装的operator==、 哈希特化

# Customer
## class `Customer`
- `get_id`
- `get_level`               标识服务等级
- `get_time_factor`         标识顾客消耗时间系数
- `get_max_waiting_time`    标识顾客最长等待时间
- `haircut_started`         标识顾客是否开始了理发
- `start_cuting_hear`       设置使顾客开始理发

# Barber
## class `Barber`
- `get_id`
- `get_level`               标识服务等级
- `get_time_factor`         标识理发师消耗时间系数
- `busy`                    标识理发师是否忙碌
- `get_customer_id`         标识理发师服务顾客id
- `set_customer_id`         设置服务的顾客
- `release_customer`        设置顾客服务完毕

# Chair
## class: `Chair`
