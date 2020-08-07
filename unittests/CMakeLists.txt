cmake_minimum_required(VERSION 3.15)
project(UnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)

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
target_link_libraries(UnitTests GraphCalc)
target_link_libraries(gcalc GraphCalc)

target_link_libraries(UnitTests ${GTEST_LIBRARIES} pthread)