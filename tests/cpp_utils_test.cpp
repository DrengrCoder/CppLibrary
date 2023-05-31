#define CATCH_CONFIG_MAIN

#include "../src/catch.hpp"
#include "../src/cpp_utilities.h"

TEST_CASE("C++ Utilities elapsed time test", "[single-file]")
{
    std::cout << "The time stamp tests are basic print outs of elapsed time "
            << "and a user should be present and judge the time stamp "
            << "themselves to see whether they are appropriate or not.\n";

    std::cout << "First test will print out current time, and then elapsed "
            << "times onwards from that will be fixed points by usleep commands "
            << "in the following order: 0.1 seconds, 0.2 seconds, 0.5 seconds, "
            << "and 1 second, and print outs will be microseconds, milliseconds "
            << "and the std::string printout.\n";

    std::cout << "Start time: us = " << CurrentProgramRunningTime_Microseconds()
            << " ms = " << CurrentProgramRunningTime_Milliseconds()
            << " timestamp = " << CurrentProgramRunningTime() << "\n";
    
    usleep(100000);

    std::cout << "Elapsed time (0.1): us = " << CurrentProgramRunningTime_Microseconds()
            << " ms = " << CurrentProgramRunningTime_Milliseconds()
            << " timestamp = " << CurrentProgramRunningTime() << "\n";
    
    usleep(200000);
    
    std::cout << "Elapsed time (0.2): us = " << CurrentProgramRunningTime_Microseconds()
            << " ms = " << CurrentProgramRunningTime_Milliseconds()
            << " timestamp = " << CurrentProgramRunningTime() << "\n";
    
    usleep(500000);
    
    std::cout << "Elapsed time (0.5): us = " << CurrentProgramRunningTime_Microseconds()
            << " ms = " << CurrentProgramRunningTime_Milliseconds()
            << " timestamp = " << CurrentProgramRunningTime() << "\n";
    
    usleep(1000000);
    
    std::cout << "Elapsed time (1): us = " << CurrentProgramRunningTime_Microseconds()
            << " ms = " << CurrentProgramRunningTime_Milliseconds()
            << " timestamp = " << CurrentProgramRunningTime() << "\n";
    
}

TEST_CASE("C++ Utilities string is integer tests", "[single-file]")
{
    std::string _i = "1";
    REQUIRE(StringIsInteger(_i) == true);

    _i = "5251";
    REQUIRE(StringIsInteger(_i) == true);

    std::string _d = "1.23";
    REQUIRE(StringIsInteger(_d) == false);

    _d = "123.456";
    REQUIRE(StringIsInteger(_d) == false);

    std::string _s = "string";
    REQUIRE(StringIsInteger(_s) == false);

    _s = "s";
    REQUIRE(StringIsInteger(_s) == false);
}

TEST_CASE("C++ Utilities string is ip tests", "[single-file]")
{
    std::string _address = "192.168.55.200";
    REQUIRE(StringIsValidIp(_address) == true);

    _address = "192.168.55.";
    REQUIRE(StringIsValidIp(_address) == false);

    _address = "192.55.200";
    REQUIRE(StringIsValidIp(_address) == false);
    
    _address = "192.168.55.256";
    REQUIRE(StringIsValidIp(_address) == false);
    
    _address = "192.168.55.-2";
    REQUIRE(StringIsValidIp(_address) == false);
    
    _address = "192.168.55.200.123";
    REQUIRE(StringIsValidIp(_address) == false);
    
    _address = "192.sdfsdf.55.200";
    REQUIRE(StringIsValidIp(_address) == false);
}

TEST_CASE("C++ Utilities count occurrences test", "[single-file]")
{
    std::string _search = "search this";
    std::string _target = "Search";

    REQUIRE(CountOccurrences(_search, _target) == 0);
    REQUIRE(CountOccurrences(_search, _target, false) == 1);

    _search = "12131415";
    _target = "1";

    REQUIRE(CountOccurrences(_search, _target) == 4);

    _search = "SeArCh Search search seaRcH search SEarch SEarch";
    _target = "SEarch";

    REQUIRE(CountOccurrences(_search, _target) == 2);
    REQUIRE(CountOccurrences(_search, _target, false) == 7);
}

