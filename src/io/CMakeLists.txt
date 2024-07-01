set(TARGET SimIo_Lib)
aux_source_directory(. SRC)
include_directories(./include)
include_directories(${CMAKE_SOURCE_DIR}/src/backstage/include)
#输出为静态
add_library(${TARGET} STATIC ${SRC})
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
if (ENABLE_DEBUG)
	add_compile_options(DEBUG_PRINT)
endif()


