set(TARGET app)
aux_source_directory(. SRC)
include_directories(./include)
include_directories(../backstage/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/include)

add_executable(${TARGET} ${SRC})
target_link_libraries(app PRIVATE Backstage_Lib)

