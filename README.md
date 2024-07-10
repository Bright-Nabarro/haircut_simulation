# 理发店经营模拟程序

## 项目描述
本项目模拟一个理发店的日常经营活动。通过队列管理系统，
模拟顾客在理发店内等待和服务过程，预测理发店的经营情况。
项目的目的是评估理发店的运营效率和盈利能力。

## 功能
#### 启动参数
- 默认模式：直接输出模式，模拟过程中的所有事件将按时间顺序自动输出。
- 步进模式（-n）：用户可以通过命令逐步控制模拟过程，查看每一步的详细信息。
#### 步进模式命令
- `q`, `quit`：正常退出程序。终止当前模拟，并输出最终统计结果。
- `c`, `continue`：转换成直接输出模式运行。模拟过程将自动继续，直到结束或被手动终止。
- **其他按键**：触发下一步。模拟过程暂停，等待用户输入按键以继续到下一个事件。

## 系统要求
- **操作系统**： Unix-like 或 Windows
- **编译器**：支持C++20、C++23 \<print\>库的编译器。
如 GCC 14.0.3 或更高版本，Clang 19.0 或更高版本，
MSVC 19.34 或更高版本。
- **CMake**: 3.20或更高

## 安装步骤 （针对Unix-like）
1. **获取代码**
将项目代码仓库克隆到计算机
```bash
git clone https://github.com/Bright-Nabarro/haircut_simulation.git
cd haircut_simulation
```

2. **构建项目**
```bash
mkdir build
cmake -B build -DENABLE_DEBUG=OFF  #开启或关闭DEBUG模式
make --build build
```

3. **运行项目**
- 直接输出模式
```bash
bin/app
```
- 步进模式
```bash
bin/app -n
```



