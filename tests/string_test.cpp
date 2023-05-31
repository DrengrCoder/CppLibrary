#define CATCH_CONFIG_MAIN

#include "../src/catch.hpp"
#include "../src/string.h"

TEST_CASE("String assignment tests", "[single-file]")
{
    //  #################### Assignment ####################

    std::cout << "Starting assignment tests...\n";

    String _customString;
    std::string _emptyString = "";
    bool _result = _customString.c_str() == _emptyString;
    REQUIRE(_result);

    std::string _stdstring = "std::string var";
    _customString = _stdstring;
    _result = _customString.c_str() == _stdstring;
    REQUIRE(_result);

    const char* _constCharPointer = "const char*";
    _customString = _constCharPointer;
    _result = _customString.c_str() == _constCharPointer;
    REQUIRE(_result);

    String _anotherString = "Another String object";
    _customString = _anotherString;
    _result = _customString.c_str() == _anotherString;
    REQUIRE(_result);
    
    char _charPointer[] = "Char Pointer";
    _customString = _charPointer;
    _result = _customString.c_str() == _charPointer;
    REQUIRE(_result);

    char _char = 'c';
    _customString = _char;
    _result = _customString.c_str() == std::string(1, _char);
    REQUIRE(_result);
}

TEST_CASE("String contains tests", "[single-file]")
{
    //  #################### Contains ####################

    std::cout << "Starting contains tests...";

    std::string _containsString = "Contains String";
    String _customString = "A check for Contains String boolean function";
    REQUIRE(_customString.contains(_containsString) == true);
    REQUIRE(_customString.contains("false") == false);

    const char* _containsConstCharPointer = "Contains char pointer";
    _customString = "A check for Contains char pointer bool function";
    REQUIRE(_customString.contains(_containsConstCharPointer) == true);
    REQUIRE(_customString.contains("failure") == false);
    
    const char _containsConstChar = 'q';
    _customString = "Contains q bool check";
    REQUIRE(_customString.contains(_containsConstChar) == true);
    REQUIRE(_customString.contains("nope") == false);
    
    char _containsChar = 'w';
    _customString = "Contains w bool check";
    REQUIRE(_customString.contains(_containsChar) == true);
    REQUIRE(_customString.contains('q') == false);
}

TEST_CASE("String replace tests", "[single-file]")
{
    //  #################### Replace ####################

    std::cout << "Starting replace tests...";

    std::string _replaceThis = "Replacing";
    std::string _withThis = "Replaced";

    const char* _replaceThisCPointer = "Replacing";
    const char* _withThisCPointer = "Replaced";

    const char _replaceThisCChar = 'q';
    const char _withThisCChar = 'w';

    char _replaceThisChar = 'q';
    char _withThisChar = 'w';

    std::string _resultString = "Replaced this q";

    String _customString = "Replacing this q";
    bool _result = _customString.replace(_replaceThis, _withThis).c_str() 
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCPointer, _withThisCPointer).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCChar, _withThisCChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisChar, _withThisChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThis, _withThisCPointer).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "w this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThis, _withThisCChar).c_str()
            == _resultString;
    REQUIRE(_result);

    _resultString = "Replacing this Replacing";
    _customString = "Replacing this w";
    _result = _customString.replace(_withThisCChar, _replaceThis).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "w this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThis, _withThisChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this Replacing";
    _customString = "Replacing this w";
    _result = _customString.replace(_withThisChar, _replaceThis).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "w this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCPointer, _withThisCChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this Replacing";
    _customString = "Replacing this w";
    _result = _customString.replace(_withThisCChar, _replaceThisCPointer).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "w this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCPointer, _withThisChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this Replacing";
    _customString = "Replacing this w";
    _result = _customString.replace(_withThisChar, _replaceThisCPointer).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCChar, _withThisChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    // #####

    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace("Replacing", _withThis).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThis, "Replaced").c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace("Replacing", _withThisCPointer).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replaced this q";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCPointer, "Replaced").c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace("q", _withThisCChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisCChar, "w").c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace("q", _withThisChar).c_str()
            == _resultString;
    REQUIRE(_result);
    
    _resultString = "Replacing this w";
    _customString = "Replacing this q";
    _result = _customString.replace(_replaceThisChar, "w").c_str()
            == _resultString;
    REQUIRE(_result);
}

TEST_CASE("String split tests", "[single-file]")
{
    //  #################### Split ####################
    std::vector<String> _vector;

    std::cout << "Starting split tests...";
    
#pragma GCC diagnostic ignored "-Wsign-compare"

    String _customString = "This is a space delimited string";
    _vector = _customString.split(' ');
    REQUIRE(_vector.size() == 6);
    
    _customString = "This-is-a-hyphon-delimited-string";   
    _vector = _customString.split('-');
    REQUIRE(_vector.size() == 6);
    
    _customString = "This/is/a/slash/delimited/string"; 
    _vector = _customString.split('/');
    REQUIRE(_vector.size() == 6);

    _customString = "This\n\nis\n\na\n\nnew\n\nline\n\ndelimited\n\nstring";
    _vector = _customString.split("\n\n");
    REQUIRE(_vector.size() == 7);
    
    _customString = "Thisasdisasdanasd a s d asddelimitedasdstring";
    _vector = _customString.split("asd");
    REQUIRE(_vector.size() == 6);

    //  #####

    std::string _splitStdString = "asdf";
    _customString = "Thisasdfisasdfanasdfa s d fasdfdelimitedasdfstring";
    _vector = _customString.split(_splitStdString);
    REQUIRE(_vector.size() == 6);

    const char* _splitCPointer = "fdsa";
    _customString = "Thisfdsaisfdsaafdsaspacefdsadelimitedfdsastring";
    _vector = _customString.split(_splitCPointer);
    REQUIRE(_vector.size() == 6);

    const char _splitCChar = 'q';
    _customString = "Thisqisqaqspaceqdelimitedqstring";
    _vector = _customString.split(_splitCChar);
    REQUIRE(_vector.size() == 6);

    char _splitChar = 'w';
    _customString = "Thiswiswawspacewdelimitedwstring";
    _vector = _customString.split(_splitChar);
    REQUIRE(_vector.size() == 6);

#pragma GCC diagnostic pop
}

TEST_CASE("String base C++ function tests", "[single-file]")
{
    std::cout << "Starting length and substr tests...";

    //  #################### length ####################
    
    String _customString = "0123456789";
    REQUIRE(_customString.length() == 10);
    
    _customString = "01234567890123456789\n\n";
    REQUIRE(_customString.length() == 22);
    
    //  #################### substr ####################
    
    std::string _resultString = " will be a";
    _customString = "This will be a substring";
    REQUIRE(_customString.substr(4, 10).c_str() == _resultString);

    _resultString = "This";
    REQUIRE(_customString.substr(0, 4).c_str() == _resultString);
    
    _resultString = " substring";
    REQUIRE(_customString.substr(14, _customString.length()).c_str() == _resultString);
}

TEST_CASE("String plus operator tests", "[single-file]")
{
    //  #################### operators + ####################
    
    std::cout << "Starting plus operator tests...";

    std::string _resultString = "Str Lit String literal";
    String _firstConcat = "String literal";
    String _secondConcat = "Str Lit " + _firstConcat;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "String literalstr lit";
    _secondConcat = _firstConcat + "str lit";
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "String literalstr litString literal";
    String _thirdConcat = _secondConcat + _firstConcat;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "String literalConst char pointer";
    const char* _concatCCharPointer = "Const char pointer";
    String _fourthConcat = _firstConcat + _concatCCharPointer;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "Const char pointerString literal";
    _fourthConcat = _concatCCharPointer + _firstConcat;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "String literalc";
    char _concatChar = 'c';
    String _fifthConcat = _firstConcat + _concatChar;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "cString literal";
    _fifthConcat = _concatChar + _firstConcat;
    REQUIRE(_secondConcat.c_str() == _resultString);

    _resultString = "ch";
    char _concatOtherChar = 'h';
    String _sixthConcat = _concatChar + _concatOtherChar;
    REQUIRE(_secondConcat.c_str() == _resultString);
    
    _resultString = "String Literal concatenated";
    String _seventhConcat = "String " + std::string("Literal concatenated");
    REQUIRE(_secondConcat.c_str() == _resultString);
}

TEST_CASE("String plus-equals operator tests", "[single-file]")
{
    //  #################### operators += ####################

    std::cout << "Starting plus-equals operator tests...";
    
    std::string _resultString = "1";
    String _myString = "1";
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "12";
    _myString += "2";
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "123";
    _myString += '3';
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "1234";
    std::string _stringToPlusEquals = "4";
    _myString += _stringToPlusEquals;
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "12345";
    const char* _charPointerToPlusEquals = "5";
    _myString += _charPointerToPlusEquals;
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "123456";
    const char _cCharToPlusEquals = '6';
    _myString += _cCharToPlusEquals;
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "1234567";
    char _charToPlusEquals = '7';
    _myString += _charToPlusEquals;
    REQUIRE(_myString.c_str() == _resultString);
    
    _resultString = "12345678";
    char _charArrayToPlusEquals[] = "8";
    _myString += _charArrayToPlusEquals;
    REQUIRE(_myString.c_str() == _resultString);
}

