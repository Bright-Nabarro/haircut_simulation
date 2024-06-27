#pragma once
#include <cassert>
#include <print>

//测试函数只会调用一次，简化起见在头文件定义，使用强制内联
#if defined(__GNUC__) || defined(__clang__)
	#define TPF [[gnu::always_inline]] inline 	//后一个inline只是为了消除静态检测警告
#elif defined(_MSC_VER)
	#define TPF __forceinline inline
#else
	#error "Unsupported comiler. This code requires gcc, clang or MSVC"
#endif

