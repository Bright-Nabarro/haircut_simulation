set(TARGET backstage)
include_directories(${CMAKE_SOURCE_DIR}/src/backstage/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/include)
include_directories(./include)
aux_source_directory(. SRC)

add_executable(${TARGET} ${SRC})
target_link_libraries(${TARGET} PRIVATE Backstage_Lib)
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/test)

