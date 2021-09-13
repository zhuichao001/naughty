cmake_minimum_required (VERSION 2.8)

project(code-for-blog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-rtti -std=c++11 -Wall")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(MyTest test.cpp)
target_link_libraries(MyTest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(MyTest ${CMAKE_THREAD_LIBS_INIT})

add_test(Test MyTest)
enable_testing()
