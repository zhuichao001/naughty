cmake_minimum_required(VERSION 2.8)

set(SRCS
    test.cpp
    )
set(INCS
    ./
    )

include_directories(${INCS})
add_executable(test_threadpool ${SRCS})
target_link_libraries(test_threadpool pthread)
