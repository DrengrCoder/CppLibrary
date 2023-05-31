#define CATCH_CONFIG_MAIN

#include "../src/catch.hpp"
#include "../src/cli_parser.h"

LogSettings LOG_SETTINGS;

TEST_CASE("CLI args parser create options tests", "[single-file]")
{
    //  disable the std::cout printing
    LOG_SETTINGS.ls_print_to_debug = false;

    ParserOption o1 = ParserOption("tag 1", "description1", {"choice 1"});
    ParserOption o2 = ParserOption("tag2", "descr2", {"choice2", ""}, std::string("default 2"));
    ParserOption o3 = ParserOption(" tag3", "descr3", {" choice3", "choice3.1"}, true);

    REQUIRE(o1.GetTags().size() == 1);
    if (o1.GetTags().size() > 0){
        REQUIRE(o1.GetTags()[0] == "--tag-1");
    }

    REQUIRE(o1.GetChoices().size() == 1);
    if (o1.GetChoices().size() > 0){
        REQUIRE(o1.GetChoices()[0] == "choice-1");
    }

    REQUIRE(o2.GetChoices().size() == 1);
    REQUIRE(o2.GetDefaultValue() == "default-2");

    REQUIRE(o3.GetTags().size() == 1);
    if (o3.GetTags().size() > 0){
        REQUIRE(o3.GetTags()[0] == "--tag3");
    }
    REQUIRE(o3.GetChoices().size() == 2);
    if (o3.GetChoices().size() > 1){
        REQUIRE(o3.GetChoices()[0] == "choice3");
    }

    ParserOption o4 = ParserOption({"tag 4", "tag4.1"}, "descr4", {"choice 4"});
    ParserOption o5 = ParserOption({"tag5", "tag5.1"}, "descr5", {"choice5", ""}, std::string("default 5"));
    ParserOption o6 = ParserOption({" tag6", "tag6.1"}, "descr6", {" choice6", "choice6.1"}, true);

    REQUIRE(o4.GetTags().size() == 2);
    if (o4.GetTags().size() > 0){
        REQUIRE(o4.GetTags()[0] == "--tag-4");
    }

    REQUIRE(o4.GetChoices().size() == 1);
    if (o4.GetChoices().size() > 0){
        REQUIRE(o4.GetChoices()[0] == "choice-4");
    }

    REQUIRE(o5.GetChoices().size() == 1);
    REQUIRE(o5.GetDefaultValue() == "default-5");

    REQUIRE(o6.GetTags().size() == 2);
    if (o6.GetTags().size() > 0){
        REQUIRE(o6.GetTags()[0] == "--tag6");
    }
    REQUIRE(o6.GetChoices().size() == 2);
    if (o6.GetChoices().size() > 1){
        REQUIRE(o6.GetChoices()[0] == "choice6");
    }

    std::vector<std::string> v7{"tag 7"};
    std::vector<std::string> v8{"tag8"};
    std::vector<std::string> v9{" tag9"};
    ParserOption o7 = ParserOption(v7, "descr7", {"choice 7"});
    ParserOption o8 = ParserOption(v8, "descr8", {"choice8", ""}, std::string("default 8"));
    ParserOption o9 = ParserOption(v9, "descr9", {" choice9", "choice 9.1"}, true);

    REQUIRE(o7.GetTags().size() == 1);
    if (o7.GetTags().size() > 0){
        REQUIRE(o7.GetTags()[0] == "--tag-7");
    }

    REQUIRE(o7.GetChoices().size() == 1);
    if (o7.GetChoices().size() > 0){
        REQUIRE(o7.GetChoices()[0] == "choice-7");
    }

    REQUIRE(o8.GetChoices().size() == 1);
    REQUIRE(o8.GetDefaultValue() == "default-8");

    REQUIRE(o9.GetTags().size() == 1);
    if (o9.GetTags().size() > 0){
        REQUIRE(o9.GetTags()[0] == "--tag9");
    }
    REQUIRE(o9.GetChoices().size() == 2);
    if (o9.GetChoices().size() > 1){
        REQUIRE(o9.GetChoices()[0] == "choice9");
    }
}

TEST_CASE("CLI args parser add option tests", "[single-file]")
{
    ParserOption o11 = ParserOption("tag11", "description11", std::string("value11"));
    ParserOption o12 = ParserOption("tag12", "descr12", std::string("value12"), std::string("default12"));
    ParserOption o13 = ParserOption("tag13", "descr13", true, "value13");

    ParserOption o14 = ParserOption({"tag14"}, "descr14", std::string("value14"));
    ParserOption o15 = ParserOption({"tag15"}, "descr15", std::string("value15"), std::string("default15"));
    ParserOption o16 = ParserOption({"tag16"}, "descr16", true, std::string("value16"));

    std::vector<std::string> v17{"tag17"};
    std::vector<std::string> v18{"tag18"};
    std::vector<std::string> v19{"tag19"};
    ParserOption o17 = ParserOption(v17, "descr17", std::string("value17"));
    ParserOption o18 = ParserOption(v18, "descr18", std::string("value18"), std::string("default18"));
    ParserOption o19 = ParserOption(v19, "descr19", true, std::string("value19"));

#pragma GCC diagnostic ignored "-Wwrite-strings"
    int argc = 5;
    char* argv[] = 
    { 
        "the first argument is always the execution command for the program", 
        "tag11", 
        "tag15",
        "tag18", 
        "tag19" 
    };
#pragma GCC diagnostic pop

    Parser _parser = Parser(argc, argv, "This is a test description");
    
    int r = _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});
    REQUIRE(r == 0);

    r = _parser.AddOptions({o11, o12, o13});
    REQUIRE(r == 0);
}

TEST_CASE("CLI args parser parse args tests", "[single-file]")
{
    ParserOption o11 = ParserOption("tag11", "description11", std::string("value11"));
    ParserOption o12 = ParserOption("tag12", "descr12", std::string("value12"), std::string("default12"));
    ParserOption o13 = ParserOption("tag13", "descr13", true, "value13");

    ParserOption o14 = ParserOption({"tag14"}, "descr14", std::string("value14"));
    ParserOption o15 = ParserOption({"tag15"}, "descr15", std::string("value15"), std::string("default15"));
    ParserOption o16 = ParserOption({"tag16"}, "descr16", true, std::string("value16"));

    std::vector<std::string> v17{"tag17"};
    std::vector<std::string> v18{"tag18"};
    std::vector<std::string> v19{"tag19"};
    ParserOption o17 = ParserOption(v17, "descr17", std::string("value17"));
    ParserOption o18 = ParserOption(v18, "descr18", std::string("value18"), std::string("default18"));
    ParserOption o19 = ParserOption(v19, "descr19", true, std::string("value19"));

#pragma GCC diagnostic ignored "-Wwrite-strings"
    char* argv1[] = 
    { 
        "the first argument is always the execution command for the program", 
        "tag11", 
        "tag15",
        "tag18", 
        "tag19" 
    };
    char* argv2[] = 
    { 
        "the first argument is always the execution command for the program", 
        "tag13", 
        "tag16",
        "tag18", 
        "tag19" 
    };
    char* argv3[] = 
    { 
        "the first argument is always the execution command for the program", 
        "--tag13",
        "somedata",
        "--tag16",
        "somemoredata",
        "--tag18", 
        "--tag19",
        "finaldata"
    };
    char* argv4[] = 
    { 
        "the first argument is always the execution command for the program", 
        "this is not data",
        "tag11", 
        "tag15",
        "tag18", 
        "tag19" 
    };
    char* argv5[] = 
    { 
        "the first argument is always the execution command for the program"
    };
#pragma GCC diagnostic pop

    int argc = 5;
    Parser _parser = Parser(argc, argv1, "This is a test description");
    _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});

    bool parserResult = _parser.Process();
    REQUIRE(parserResult == false);
    
    argc = 5;
    _parser = Parser(argc, argv2, "This is a test description");
    _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});

    parserResult = _parser.Process();
    REQUIRE(parserResult == false);
    
    argc = 8;
    _parser = Parser(argc, argv3, "This is a test description");
    _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});

    parserResult = _parser.Process();
    REQUIRE(parserResult == true);
    
    argc = 6;
    _parser = Parser(argc, argv4, "This is a test description");
    _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});

    parserResult = _parser.Process();
    REQUIRE(parserResult == false);
    
    argc = 1;
    _parser = Parser(argc, argv5, "This is a test description");
    _parser.AddOptions({o11, o12 , o13, o14, o15, o16, o17, o18, o19});

    parserResult = _parser.Process();
    REQUIRE(parserResult == false);
}

