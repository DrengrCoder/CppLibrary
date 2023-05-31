#define CATCH_CONFIG_MAIN

#include "../src/catch.hpp"
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

    LogType lt = LT_LL_INFO;
    LOG_SETTINGS.ls_selected_level = LT_LL_INFO;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LT_INFO;
    LOG_SETTINGS.ls_selected_level = LT_INFO;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LT_DEBUG;
    LOG_SETTINGS.ls_selected_level = LT_DEBUG;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LT_WARN;
    LOG_SETTINGS.ls_selected_level = LT_WARN;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LT_ERROR;
    LOG_SETTINGS.ls_selected_level = LT_ERROR;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);

    lt = LT_FATAL;
    LOG_SETTINGS.ls_selected_level = LT_FATAL;
    REQUIRE(LOG_SETTINGS.ls_selected_level == lt);
}

TEST_CASE("Test log line macro's", "[single-file]")
{
    //  Reset defaults
    LOG_SETTINGS.ls_selected_level = LT_LL_INFO;
    LOG_SETTINGS.ls_use_working_dir = false;
    LOG_SETTINGS.ls_overwrite = false;
    LOG_SETTINGS.ls_print_to_file = true;
    LOG_SETTINGS.ls_print_to_debug = true;
    LOG_SETTINGS.ls_print_datetime = true;
    LOG_SETTINGS.ls_print_fileline = true;

    //  Run log lines

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_LL_INFO;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_INFO;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_DEBUG;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_WARN;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_ERROR;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";

    std::cout << "#########################################\n";

    LOG_SETTINGS.ls_selected_level = LT_FATAL;
    log << "A test low level log line.";
    ilog << "A test info level log line.";
    dlog << "A test debug level log line.";
    wlog << "A test warn level log line.";
    elog << "A test error level log line.";
    flog << "A test fatal level log line.";
}

