# Copyright (C) Denys Valchuk - All Rights Reserved
# ZHZhbGNodWtAZ21haWwuY29tCg==

include(CMakePrintHelpers)

add_executable(pidcoll_test pidcoll_test.cpp)

find_package(GTest)
target_link_libraries(pidcoll_test 
        PRIVATE pidcoll
        gtest::gtest)

include(GoogleTest)
gtest_discover_tests(pidcoll_test)
