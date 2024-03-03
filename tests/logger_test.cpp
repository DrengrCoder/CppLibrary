#define CATCH_CONFIG_MAIN

#include "../src/catch2/catch.hpp"
#include "../src/log.h"

LogSettings LOG_SETTINGS;

TEST_CASE("Set var's to false", "[single-file]")
{
    //  Set to false

    LOG_SETTINGS.ls_overwrite = false;
    REQUIRE(LOG_SETTINGS.ls_overwrite == false);

    LOG_SETTINGS.ls_print_datetime = false;
    REQUIRE(LOG_SETTINGS.ls_print_datetime == false);

    LOG_SETTINGS.ls_print_fileline = false;
    REQUIRE(LOG_SETTINGS.ls_print_fileline == false);

    LOG_SETTINGS.ls_print_to_debug = false;
    REQUIRE(LOG_SETTINGS.ls_print_to_debug == false);

    LOG_SETTINGS.ls_print_to_file = false;
    REQUIRE(LOG_SETTINGS.ls_print_to_file == false);

    LOG_SETTINGS.ls_use_working_dir = false;
    REQUIRE(LOG_SETTINGS.ls_use_working_dir == false);
}

TEST_CASE("Set var's to true", "[single-file]")
{
    //  Set to true

    LOG_SETTINGS.ls_overwrite = true;
    REQUIRE(LOG_SETTINGS.ls_overwrite == true);

    LOG_SETTINGS.ls_print_datetime = true;
    REQUIRE(LOG_SETTINGS.ls_print_datetime == true);

    LOG_SETTINGS.ls_print_fileline = true;
    REQUIRE(LOG_SETTINGS.ls_print_fileline == true);

    LOG_SETTINGS.ls_print_to_debug = true;
    REQUIRE(LOG_SETTINGS.ls_print_to_debug == true);

    LOG_SETTINGS.ls_print_to_file = true;
    REQUIRE(LOG_SETTINGS.ls_print_to_file == true);

    LOG_SETTINGS.ls_use_working_dir = true;
    REQUIRE(LOG_SETTINGS.ls_use_working_dir == true);
}

TEST_CASE("Set log type enum var", "[single-file]")
{    
    //  Set logtype

    LogType::Value lt = LogType::LT_LL_INFO;
    LOG_SETTINGS.ls_selected_level = LogType::LT_LL_INFO;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LogType::LT_INFO;
    LOG_SETTINGS.ls_selected_level = LogType::LT_INFO;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LogType::LT_DEBUG;
    LOG_SETTINGS.ls_selected_level = LogType::LT_DEBUG;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LogType::LT_WARN;
    LOG_SETTINGS.ls_selected_level = LogType::LT_WARN;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LogType::LT_ERROR;
    LOG_SETTINGS.ls_selected_level = LogType::LT_ERROR;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LogType::LT_FATAL;
    LOG_SETTINGS.ls_selected_level = LogType::LT_FATAL;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);
}

TEST_CASE("Log line macro's", "[single-file]")
{
    //  Reset defaults
    LOG_SETTINGS.ls_selected_level = LogType::LT_LL_INFO;
    LOG_SETTINGS.ls_use_working_dir = false;
    LOG_SETTINGS.ls_overwrite = false;
    LOG_SETTINGS.ls_print_to_file = true;
    LOG_SETTINGS.ls_print_to_debug = true;
    LOG_SETTINGS.ls_print_datetime = true;
    LOG_SETTINGS.ls_print_fileline = true;

    //  Run log lines

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_LL_INFO;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_INFO;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_DEBUG;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_WARN;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_ERROR;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LogType::LT_FATAL;
    llog << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";
}

TEST_CASE("Log Type c_str functions", "[single-file]")
{
    const char* base_llinfo = "LT_LL_INFO";
    const char* base_info = "LT_INFO";
    const char* base_debug = "LT_DEBUG";
    const char* base_warn = "LT_WARN";
    const char* base_error = "LT_ERROR";
    const char* base_fatal = "LT_FATAL";

    //  Return object c_str

    LogType test_value = LogType::LT_LL_INFO;
    REQUIRE(strcmp(test_value.c_str(), base_llinfo) == 0);

    test_value = LogType::LT_INFO;
    REQUIRE(strcmp(test_value.c_str(), base_info) == 0);

    test_value = LogType::LT_DEBUG;
    REQUIRE(strcmp(test_value.c_str(), base_debug) == 0);

    test_value = LogType::LT_WARN;
    REQUIRE(strcmp(test_value.c_str(), base_warn) == 0);

    test_value = LogType::LT_ERROR;
    REQUIRE(strcmp(test_value.c_str(), base_error) == 0);

    test_value = LogType::LT_FATAL;
    REQUIRE(strcmp(test_value.c_str(), base_fatal) == 0);

    //  Input value for c_str

    const char* test_string = LogType::c_str(LogType::LT_LL_INFO);
    REQUIRE(strcmp(test_string, base_llinfo) == 0);

    test_string = LogType::c_str(LogType::LT_INFO);
    REQUIRE(strcmp(test_string, base_info) == 0);

    test_string = LogType::c_str(LogType::LT_DEBUG);
    REQUIRE(strcmp(test_string, base_debug) == 0);

    test_string = LogType::c_str(LogType::LT_WARN);
    REQUIRE(strcmp(test_string, base_warn) == 0);

    test_string = LogType::c_str(LogType::LT_ERROR);
    REQUIRE(strcmp(test_string, base_error) == 0);

    test_string = LogType::c_str(LogType::LT_FATAL);
    REQUIRE(strcmp(test_string, base_fatal) == 0);

}

TEST_CASE("Log Type custom_str functions", "[single-file]")
{
    const char* custom_llinfo =  "low info";
    const char* custom_info =  "info";
    const char* custom_debug =  "debug";
    const char* custom_warn =  "warn";
    const char* custom_error =  "error";
    const char* custom_fatal =  "fatal";
    
    //  Return object custom str
    
    LogType test_value = LogType::LT_LL_INFO;
    REQUIRE(strcmp(test_value.custom_str(), custom_llinfo) == 0);

    test_value = LogType::LT_INFO;
    REQUIRE(strcmp(test_value.custom_str(), custom_info) == 0);

    test_value = LogType::LT_DEBUG;
    REQUIRE(strcmp(test_value.custom_str(), custom_debug) == 0);

    test_value = LogType::LT_WARN;
    REQUIRE(strcmp(test_value.custom_str(), custom_warn) == 0);

    test_value = LogType::LT_ERROR;
    REQUIRE(strcmp(test_value.custom_str(), custom_error) == 0);

    test_value = LogType::LT_FATAL;
    REQUIRE(strcmp(test_value.custom_str(), custom_fatal) == 0);

    //  Input value for custom_str

    const char* test_string = LogType::custom_str(LogType::LT_LL_INFO);
    REQUIRE(strcmp(test_string, custom_llinfo) == 0);

    test_string = LogType::custom_str(LogType::LT_INFO);
    REQUIRE(strcmp(test_string, custom_info) == 0);

    test_string = LogType::custom_str(LogType::LT_DEBUG);
    REQUIRE(strcmp(test_string, custom_debug) == 0);

    test_string = LogType::custom_str(LogType::LT_WARN);
    REQUIRE(strcmp(test_string, custom_warn) == 0);

    test_string = LogType::custom_str(LogType::LT_ERROR);
    REQUIRE(strcmp(test_string, custom_error) == 0);

    test_string = LogType::custom_str(LogType::LT_FATAL);
    REQUIRE(strcmp(test_string, custom_fatal) == 0);

}

TEST_CASE("Log Type custom_str_to_value function", "[single-file]")
{
    const char* custom_llinfo =  "low info";
    const char* custom_info =  "info";
    const char* custom_debug =  "debug";
    const char* custom_warn =  "warn";
    const char* custom_error =  "error";
    const char* custom_fatal =  "fatal";
    
    //  Input custom_str for value
    
    LogType test_value = LogType::custom_str_to_value(custom_llinfo);
    REQUIRE(test_value == LogType::LT_LL_INFO);

    test_value = LogType::custom_str_to_value(custom_info);
    REQUIRE(test_value == LogType::LT_INFO);

    test_value = LogType::custom_str_to_value(custom_debug);
    REQUIRE(test_value == LogType::LT_DEBUG);

    test_value = LogType::custom_str_to_value(custom_warn);
    REQUIRE(test_value == LogType::LT_WARN);

    test_value = LogType::custom_str_to_value(custom_error);
    REQUIRE(test_value == LogType::LT_ERROR);

    test_value = LogType::custom_str_to_value(custom_fatal);
    REQUIRE(test_value == LogType::LT_FATAL);
}
