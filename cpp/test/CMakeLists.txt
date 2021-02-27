set(BINARY ${CMAKE_PROJECT_NAME}_test)

include_directories(${TALIB_SOURCE_DIRECTORIES} "${PROJECT_SOURCE_DIR}/lib/ta-lib/include")

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

SET(CMAKE_CXX_FLAGS -pthread)
enable_testing()

add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} PUBLIC gtest ta-lib)
add_test(NAME ${BINARY} COMMAND ${BINARY})
