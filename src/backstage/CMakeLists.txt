set(TARGET Backstage_Lib)
aux_source_directory(. SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/include)

#输出为静态
add_library(${TARGET} STATIC ${SRC})
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
