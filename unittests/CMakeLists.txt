cmake_minimum_required(VERSION 3.15)
project(UnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)

add_executable(ParserUnitTests ParserTests.cpp ../Parser.cpp ../BinTree.cpp ../Graph.cpp)

target_link_libraries(ParserUnitTests ${GTEST_LIBRARIES} pthread)