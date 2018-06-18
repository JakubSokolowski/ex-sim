include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(Exchange)
set(TEST_SOURCES
        OrderBookTests.cpp
        PricePointTests.cpp
        MatchingEngineTests.cpp
        )
add_executable(RunTests ${TEST_SOURCES})
target_link_libraries(RunTests gtest gtest_main Exchange)