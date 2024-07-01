set(TARGET app)
aux_source_directory(. SRC)
include_directories(./include)
include_directories(../backstage/include)
include_directories(../io/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/include)

add_executable(${TARGET} ${SRC})
target_link_libraries(${TARGET} PRIVATE Backstage_Lib)
target_link_libraries(${TARGET}	PRIVATE SimIo_Lib)

