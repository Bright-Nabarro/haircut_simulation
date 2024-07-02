cmake_minimum_required(VERSION 3.20)

project(haircat_simulation)
option(ENABLE_DEBUG "Enable debug mod" ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 	#生成compile_commands.json文件

if(ENABLE_DEBUG)
	set(CMAKE_BUILD_TYPE Debug)
	add_compile_definitions(DEBUG)		#添加DEBUG宏定义
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(
	-Wall
	-Wextra
	-Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#主程序
add_subdirectory("src/backstage")
add_subdirectory("src/io")
add_subdirectory("src/main")

#单元测试
if(ENABLE_DEBUG)
	#add_subdirectory("test/backstage")
	#add_subdirectory("test/io")
endif()

#将clangd_custom_target复制到CMakeLists.txt所在目录
add_custom_target(move_compile_commands ALL
	COMMAND ${CMAKE_COMMAND} -E copy_if_different			#平台无关的复制命令
			"${CMAKE_BINARY_DIR}/compile_commands.json"
			"${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json"
	DEPENDS "${CMAKE_BINARY_DIR}/compile_commands.json"
)

