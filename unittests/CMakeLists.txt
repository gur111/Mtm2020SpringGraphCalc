cmake_minimum_required(VERSION 3.15)
project(UnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)


set(CMAKE_TEST_DEBUG "-std=c++11 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_CXX_FLAGS ${CMAKE_TEST_DEBUG})

add_library(Graph ../Graph.cpp ../Exceptions.cpp)
add_library(GraphSwig ../GraphSwig.cpp)

add_library(GraphCalc ../Parser.cpp
                ../Exceptions.cpp
                ../BinTree.cpp
                ../Graph.cpp
                ../TreeRunner.cpp
                ../Utils.cpp
                ../CalcRunner.cpp
                ../GraphCalc.cpp
                ../StorageManager.cpp)

add_executable(UnitTests TestRunner.cpp GraphTests.cpp ParserTests.cpp FullCalcTests.cpp)
add_executable(gcalc ../Main.cpp)
target_link_libraries(GraphSwig Graph)
target_link_libraries(GraphCalc Graph)
target_link_libraries(UnitTests GraphCalc)
target_link_libraries(gcalc GraphCalc)

target_link_libraries(UnitTests ${GTEST_LIBRARIES} pthread)