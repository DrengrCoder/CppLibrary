# Custom C++ class file library

## Table of contents

- [Brief](#brief)
- [Project Contents and Overview](#project-contents-and-overview)
    - [Custom code contents](#custom-code-contents)
    - [Custom binary programs](#custom-binary-programs)
    - [Third-party code contents](#third-party-code-contents)
    - [Third-party binary programs](#third-party-binary-programs)
- [Code Descriptions](#code-descriptions)
    - [Catch.hpp (third-party)](#catchhpp-third-party)
        - [Credit](#credit)
        - [Usage](#usage)
    - [Command line interface parser (cli_parser.h)](#command-line-interface-parser-cli_parserh)
        - [The constructors and input parameters](#the-constructors-and-input-parameters)
        - [The types of options](#the-types-of-options)
        - [Credit](#credit-1)
        - [Dependencies](#dependencies)
        - [Usage](#usage-1)
    - [C++ Utilities](#c-utilities)
    - [HTTPRequest.hpp (third-party)](#httprequesthpp-third-party)
        - [Credit](#credit-2)
    - [Log - A custom and configurable logger](#log---a-custom-and-configurable-logger)
        - [Credit](#credit-3)
        - [Usage](#usage-2)
    - [Nlohmann JSON (third-party)](#nlohmann-json-third-party)
        - [Credit](#credit-4)
    - [Proxy-based HTTP Request header](#proxy-based-http-request-header)
        - [Credit](#credit-5)
        - [Dependencies](#dependencies-1)
        - [Usage](#usage-3)
    - [RapidXML (third-party)](#rapidxml-third-party)
        - [Credit](#credit-6)
    - [String - A custom String class](#string---a-custom-string-class)
        - [Credit](#credit-7)
        - [Usage](#usage-4)
    - [TCP Client network socket class](#tcp-client-network-socket-class)
        - [Error codes](#error-codes)
        - [Dependencies](#dependencies-2)
        - [Usage](#usage-5)
    - [TCP Server network socket class](#tcp-server-network-socket-class)
        - [Error codes](#error-codes-1)
        - [Dependencies](#dependencies-3)
        - [Usage](#usage-6)
- [Binary descriptions](#binary-descriptions)
    - [cppnamelint (third-party)](#cppnamelint-third-party)
    - [Automated Version Incrementor program](#automated-version-incrementor-program)
- [Installation and Build steps](#installation-and-build-steps)

## Brief

Custom C++ header files to simplify a number of common functions and operations. To be installed in the includes folder.

This project also includes a number of third-party classes and binary programs that are extremely useful for most projects and developers. Their respective license files are included under the Licenses folder, appended with their project name, and the git repository they can be accessed from (at time of writing on 31/08/2023) at the top of the file.

## Project Contents and Overview

### Custom code contents

- Basic make file to run the installation of custom and third-party classes and binary files, and build and run tests on each custom class file.
- A Command Line Interface argument parser to allow a developer to quickly and efficiently create and manage command line argument options.
- A C++ Utilities class with some common helper functions such as confirming a string is a valid IP address, validating a string as an integer, counting the number of times a specific string occurs within another string and printing the program running time using `std::chrono` C++ functions.
- A C++ logger class for customising and controlling output: directories, filenames, date and time stamps, class file name and line numbers, and a few other features. Can be disabled to stop printing anything if desired, such as log lines in tests.
- A C++ HTTP request header class that uses the custom TCP Client class to issue HTTP requests to devices with different IP addresses. Some testing has proven it works when using IP addresses routed through a VPN.
- A C++ String object class that has some higher level functions similar to a C# class, such as '.replace', '.split' and '.contains'.
- A C++ TCP Client and Server class for quickly creating and easily managing TCP/IP applications and data transferral.

### Custom binary programs

- The Version Incrementor program binary is included in this project and can be used to automatically generate a version_header.h file and manage incrementing their values through simple GNU makefile recipe's.

### Third-party code contents

- The catch.hpp file is part of the [Catch2 project](https://github.com/catchorg/Catch2) can be used to create C++ unit test files. Running these files individually produces minimal console output with colour coded success and failure markers, and running them on mass with the 'make run_tests' recipe for example, will produce a detailed output for all the unit tests in each test file.
- The HTTPRequest.hpp file is part of the [HTTPRequest project](https://github.com/elnormous/HTTPRequest/tree/master) can be used to issue HTTP Request to Web-based URL's. The custom proxy http request header file has been heavily influenced by this file, while modifications to allow connecting to API's on devices with different IP addresses.
- The [nlohmann json project](https://github.com/nlohmann/json) is used to easily manage creating and parsing JSON strings and files. This project has been optimised for performance and complex JSON objects and strings can be created and parsed within microseconds.
- The rapidxml headers is part of the RapidXML project found at "https://github.com/Fe-Bell/RapidXML" and "https://github.com/viriw/rapidxml", and is used to easily manage creating and parsing XML strings and files. This project has been optimised for performance and complex XML objects and strings can be created and parsed within microseconds.

### Third-party binary programs

- The cppnamelint binary is part of the [CppNameLint project](https://github.com/dougpuob/cppnamelint/tree/master) can be used to define custom naming conventions. '.toml' files are a string-based file that defines some standard for naming conventions (following a strict format that can be read on the git repository) and can be run on single or multiple files at once. This projects makefile defines a recipe that shows you how the terminal command would be constructed.

## Code Descriptions

This library of classes can be used standalone for the most part (dependencies listed) and allows a developer to quickly write fast, efficient and manageable code for all types of C/C++ projects (C compatibility will need to be tested on a case-by-case basis, but all classes developed for C++ compatibility).

## Catch.hpp (third-party)

This header file allows you to write unit tests for C++ programs and comes from the 'Catch2' library. This has been included in the project to be used in another projects as desired, and is also used by the tests in this project.

### Credit

The 'catch.hpp' is not something I own or developed, and is part of the [Catch2](https://github.com/catchorg/Catch2) library to help developers build and run unit tests in C++ programs. I own no rights to this file and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

### Usage

Create a new C++ file and add the `#define CATCH_CONFIG_MAIN` macro at the top of the file. You can then create as many tests as desired using `TEST_CASE("Description", "[single-file]"){}`.
- Description can be replaced for whatever descriptive text desired for this unit test.
- `"[single-file]"` is the most common option for the second parameter, but others are available if you read the Catch2 documentation on the git repository.
- There is an example recipe for building test files in the makefile in this project.

## Command line interface parser (cli_parser.h)

This class allows a developer to quickly define and manage arguments that can be passed in on the command line for binary file execution. This will probably not do everything you want it to, but it does everything I want it to, and has been developed in a way to minimise the code I need to write. You should carefully examine the constructor list and their input parameters and code docs for detailed instructions on how to use this class:

### The constructors and input parameters

- Tags automatically prepend the appropriate number of hyphon characters, so you should only define tags as a string without any hyphons.

- Leading and trailing white space in certain parameters is removed entirely, and some parameters that have remaining white space will be replaced with hyphons.

- You can define the first parameter as a single string, a brace-initialised list of strings or a `std::vector` of strings, before passing it to the constructor.

- You can define a list of specific choices as the third parameter, solely as a brace-initialised list. You may want to create your own constructors for other parameter types but pay close attention to how certain constructors are delegated to each other (I wasn't able to figure out how to delegate constructors in an initialiser list after constructing some data based on input parameters within another constructor, so for consistency, all constructors are delegated the same way being called from within other constructors. This is not standard practice but it works.).

- You can define a list of choices or specify that only a single piece of data may follow this argument tag, by specifying the third argument as a string.

- You may define a default value to an argument if desired, or you can define the argument as being **required** in the argument list as the final parameter.

- There are several more constructors available for use, but most of them strictly exist so there can be a cascading flow of delegating constructors without resetting or losing data. It is assumed that if a flag is required, that it will not have a default (otherwise you'd have specified the argument with a default value) and vice versa. Your options should be as follows:
    - tags, description, choices, default value (optional)
    - tags, description, choices, required bool (optional)
    - tags, description, value name (optional), default value (optional)
    - tags, description, required bool, value name
        - This constructor has been defined this way because it is assumed that if you specify the argument as being required, that it expects data.

### The types of options

- You can define a flag-based argument (it either exists or it doesn't), by simply defining a tag and a description. Used to define boolean related variables.

- You can define a data-based argument, with or without a default value after it. If there is no default value and the argument is found, it expects to find some sort of data after it.

- You can define a data-based argument with a specific list of choices, with or without a default value after it. If there is no default value and the argument is found, it expects to find some sort of data after it, and the data value will be checked that it is a valid choice from the list of input choices. 

### Credit

Most of this code is completely unique, and I have developed everything from scratch (to an extent), but the original implementation was influenced by the functionality of the command line argument parser and parser option class found in the [QT libraries](https://www.qt.io/).

- The [Qt Library Command Line Parser Class](https://doc.qt.io/qt-6/qcommandlineparser.html).
- The [Qt Library Command Line Option Class](https://doc.qt.io/qt-6/qcommandlineoption.html).

### Dependencies

This class requires the custom [logger class](#log---a-custom-and-configurable-logger) and the custom [C++ utilities class](#c-utilities) for some additional functions.

### Usage

- You can declare Parser Options with a single string for a tag and a description for the simplest command line args like `ParserOption opt = ParserOption("tag", "description");`.
- You can create as many as you like before adding them to a Parser. A Parser can be created by passing in the `argc` and `argv` variable pulled in by int main: `Parser parser = Parser(argc, argc, "Description");`.
- Parser Options can be added in a brace-initialiser list: `parser.AddOptions({opt});`.
- And the arguments can be parsed compared to the Parser Options: `parser.Process();`.

## C++ Utilities

The utilities class solely contains a few static functions for various features and you should check the file for specific information. Nothing here is unique or proprietary and code can be found on a simple google search, but has been included in a file in this project as I tend to use them a lot in some of my projects. This class is a dependency to other custom classes.

## HTTPRequest.hpp (third-party)

This header file allows you to issue HTTP Requests to Web-based URL's, and implements the standards for HTTP/URI syntax defined by IETF. This header file as very complex and thorough, and you should investigate all the code through yourself as required.

Usage examples can be found on the git repository documentation.

### Credit

The 'HTTPRequest.hpp' is not something I own or developed, and is part of the [HTTPRequest](https://github.com/elnormous/HTTPRequest/tree/master) project. I own no rights to this file and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

## Log - A custom and configurable logger

This class can print out to `std::cout` and/or to a text file with varying levels of log lines from low level information up to fatal. The directory the log files go to can be slightly configured and you're free to add your own options. The code comments in the file and code docs explain in detail how to setup and use this class. The format of the text is probably easier to read directly in the file.

The `LOG_SETTINGS` extern variable can be used to configure settings, and the `LogInit` function will reinitialise the logger with those settings. The logger will automatically print to `std::cout` before `LogInit` is called so nothing will need to be set up before you can start using the `log` macro's defined in the file, but if you want to change settings, such as printing to a file or toggling the filename and line number the log lines come from, you will need to change the settings on `LOG_SETTINGS` and call `LogInit` to set any changes. Calling `LogInit` will generate a 'LogFiles' folder in the appropriate location once called.

### Credit

This class has been modified from an original forum post I found [here](https://levelup.gitconnected.com/logging-in-c-60cd1571df15) but has been contained in a single header file. I modified this implementation to automatically call `std::endl` so I didn't have to remember to do it myself at the end of every line, and I created several macro's that include the `__FILE__` and `__LINE__` macro's so the filename and line number the `log` macro's are called from is pulled in to the log line and can be printed to the text file and `std::cout`.

### Usage

- Declare the Log Settings extern at the top of main and **Only main**: `LogSettings LOG_SETTINGS;`.
- Set the settings on this object in the int main function: 
    - `LOG_SETTINGS.ls_selected_level = LogType::LT_LL_INFO;`.
    - `LOG_SETTINGS.ls_print_to_file = false;`.
- Initialise the logger: `LogInit(argv);`.
- Start calling the logger macro's:
    - `clog << "Basic log line";`.
    - `flog << "Fatal log line";`.

## Nlohmann JSON (third-party)

The entire library folder and its contents are required but the single json.hpp file can be used as an include statement in any file that needs JSON objects. I own no rights to this file and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

Usage examples can be found on the git repository documentation.

### Credit

The [JSON libraries](https://github.com/nlohmann/json) provided by [nlohmann](https://github.com/nlohmann) is widely regarded as the standard for JSON object manipulation by many developers, and the library folder has been included in this project as it is something I commonly use across a lot of programs I create.

## Proxy-based HTTP Request header

This header file uses the custom TCP Client class to create network sockets that allow data transfer to API's on devices with different IP address ranges.

### Credit

This file has been heavily influenced by the [HTTPRequest project](https://github.com/elnormous/HTTPRequest/tree/master), but with some minor modifications for my custom naming conventions, additional code docs and a customised class enum type to add custom functions.

### Dependencies

This class requires the custom [logger class](#log---a-custom-and-configurable-logger), the custom [TCP Client class](#tcp-client-network-socket-class) and the custom [String class](#string---a-custom-string-class) for some additional functions.

### Usage

- Optionally decide whether you need the default `HTTP::Method::GET` or some other HTTP Request type.
- Declare the http request object:
    - `HTTP::Request request{};`.
    - `HTTP::Request request("uri string", "ip address");`.
- Send the HTTP Request and read a response: `HTTP::Response response = request.send("uri string", "ip address");`.
- And read the body: `std::string body = std::string(response._body.begin(), response._body.end());`.

You can optionally send additional header fields, for example if you need to set a different content type: `HTTP::HeaderField fields = { ._name = Content-Type", ._value = "text/xml"};` and include them in the `HTTP::Request` object when calling `.send`.

## RapidXML (third-party)

The rapid XML project includes 3 header files but only 2 are required: the base 'rapidxml.hpp' and 'rapidxml_print.hpp' file to provide the functionality required and print-friendly functions respectively.

### Credit

The [original project](https://github.com/Fe-Bell/RapidXML) and the [forked repository](https://github.com/viriw/rapidxml) contain these files, although the forked repository derivatives are easier and simpler to use. I own no rights to these files and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

## String - A custom String class

This class implements several common functions and loops you might use in C++ to achieve the relevant functionality, but contained in a single header file for ease-of-use. This class is still a work in progress and although many tests have been performed, there is no certainty that everything will work in every case, but as many things as possible has been accounted for at this time.

The `String` is null-terminated correctly, but please report any errors you experience.

Nothing here is especially unique, but all functions have been collected into a single header file.

**Things to note**:
1. This class can be assigned `char`'s, `const char*`, `std::string`'s and other `String` class objects.
2. Concatenation and insertion functions work on this class, and a number of combinations of parameter lists have been considered for different functions.
3. This class can return **both** a `const char*` and a `std::string`, but when used in a function that accepts a `const char*` and a `std::string` in the same index position in the parameter list, you will need to force it to return a `const char*` by calling `c_str()` on the `String` object, otherwise you experience an error because your code is not sure which variable type you are trying to use and therefore, which function parameter list you are trying to specify.
4. While you can concatenate two `char`'s and assign the concatenation to a `String` object, it is worth noting that 'adding' two `char`'s to each other produces a different `char` because they are essentially integers.

### Credit

This class was developed based on a [Geek for Geeks](https://www.geeksforgeeks.org/how-to-create-a-custom-string-class-in-c-with-basic-functionalities/) post, although partially modified for additional functionality.

### Usage

- Initialise a string in multiple ways with multiple data types:
```
String _string;
String _string = "asdf";
String _string = 's';

std::string str = ""; 
String _string = str;

const char* chars = "asdf";
String _string = chars;
```
- Concatenate strings:
```
String _str1;
String _str2;
String _str3 = _str1 + _str2;
_str3 += _str1;
String _string = "string" + _str1;
```
- Return as data types:
```
std::string s = _string;
const char* c = _string;
``` 
- NOTE: Because this String class object can be returned as both a std::string and a const char*, any time it is used in a function parameter list where a std::string and a const char* can be accepted in the same index position, the compiler throws an error related to ambiguous function calls because it is unsure what type you are trying to use. In these cases, you will need to use .c_str() to forcibly return it as a const char* only:
```
void Overloaded(std::string);
void Overloaded(const char*);
Overloaded(_string);            // Throws error
Overloaded(_string.c_str());    // Does not throw error
```
- Use as an iterator:
```
for (auto it = _string.begin(); it != _string.end(); it++);
```
- Preexisting C++ string functionality:
```
const char* c = _string.c_str();
int i = _string.length();
String _new = _string.substr(2, 5);
```
- 'Replacement' functionality:
```
String _new = _string.replace('c', 'a');
String _new = _string.replace("str", "ing");
String _new = _string.replace("short", "longer");
String _new = _string.replace("longer", "short");
String _new = _string.replace('c', "str");
String _new = _string.replace("str", 'c');
```
- Split to vector functionality:
```
std::vector<String> vec = _string.split(' ');
std::vector<String> vec = _string.split("\r\n");
```
- 'Contains' functionality:
```
bool r = _string.contains('c');
bool r = _string.contains("str");
```
- White space trimming functionality:
```
String _string = "   left trim    ";
String r = _string.ltrim();
r = "left trim    ";
```
```
String _string = "    right trim    ";
String r = _string.rtrim();
r = "    right trim";
```
```
String _string = "    full trim    ";
String r = _string.trim();
r = "full trim";
```
- To upper and lower case functionality:
```
String _string = "lower to UPPER";
String r = _string.to_upper();
r = "LOWER TO UPPER";
```
```
String _string = "UPPER TO lower";
String r = _string.to_lower();
r = "upper to lower";
```

## TCP Client network socket class

Network sockets are originally very basic and must be configured for TCP or UDP type data streams. You do not need to be familiar with how network sockets work to use this class, but this class makes it simple to implement client-side functionality, with appropriate functions to initialise and connect to network sockets, applying some common functions and argument options that would be expected to configure a socket for a TCP-based data stream connection.

Network socket objects are easier to manipulate if they are pointer objects.

Nothing here is especially unique and can be learned after a few google searches and looking at the right forum posts, but the simplest implementations have been created. This class can be inherited to provide more advanced functionality. If you need to rewrite these classes for your own purposes, I suggest learning more about network sockets and network programming theory and practice before attempting to do something too complex.

### Error codes

There is a unique error code numbering system implemented for this class. The error code system is a combination of custom error codes (between 100 and 9999) and partial custom error codes combined with the ‘errno’ macro to produce values 10000 and over.

Custom error codes between 100 and 9999 denote some sort of usage error. Error codes 10000 and over follow the pattern YYXXX where YY is a 2 digit custom code I have given it, and XXX is the 'errno' macro error code added to the value. For example:
- '12047' is the custom error code '12' and the errno code '047' or just '47'.
- '10111' is the custom error code '10' and the errno code '111'.

The custom codes are documented here, but the 'errno' code meaning will change depending on what caused the errno code to be set and you should investigate this yourself:
- 10xxx = socket creation failed in the constructor. Typically an exception is thrown here and you might not be able to access this number on an instantiated object, but 'errno' will be accessible.
- 11xxx = Address invalid / not supported, the last 3 digits will be 'errno' and will provide more specific details.
- 12xxx = Connection attempt failed, the last 3 digits will be 'errno' and will provide more specific details.
- 13xxx = Less than 0 bytes returned when sending and reading from the socket, the last 3 digits will be 'errno' and will provide more specific details.
- 101 = Attempted to read from or write to the socket without a valid socket file descriptor being set or initialised. 'errno' will not be set as this error is manually caught by the class.

### Dependencies

This class requires the custom [logger class](#log---a-custom-and-configurable-logger) to log unique errors.

### Usage

- Initialise a TCP Client object:
```
TcpClient *client = new TcpClient();
TcpClient *client = new TcpClient(TcpClient::InternetProtocol::v6);
```
- Connect to an IP address and port (127.0.0.1 for IP if connecting to port of current device):
```
client->Connect("127.0.0.1", 1234);
```
- Send and Read bytes on the socket:
```
int n_sent = client->Send("Sending this string");
```
```
char buff[1024];
int n_read = client->Read(buff, 1024);
// buff contains the bytes read from socket
```
- Retrieve error message and codes:
```
std::string errmsg = client->ERR_MSG();
int errcode = client->ERR_NO();
```

## TCP Server network socket class

Network sockets are originally very basic and must be configured for TCP or UDP type data streams. You do not need to be familiar with how network sockets work to use this class, but this class makes it simple to implement server-side functionality, with appropriate functions to initialise and listen to network socket ports, applying some common functions and argument options that would be expected to configure a socket for a TCP-based data stream connection.

Nothing here is especially unique and can be learned after a few google searches and looking at the right forum posts, but the simplest implementations have been created. This class can be inherited to provide more advanced functionality. If you need to rewrite these classes for your own purposes, I suggest learning more about network sockets and network programming theory and practice before attempting to do something too complex.

### Error codes

There is a unique error code numbering system implemented for this class. The error code system is a combination of custom error codes (between 100 and 9999) and partial custom error codes combined with the ‘errno’ macro to produce values 10000 and over.

Custom error codes between 100 and 9999 denote some sort of usage error. Error codes 10000 and over follow the pattern YYXXX where YY is a 2 digit custom code I have given it, and XXX is the 'errno' macro error code added to the value. For example:
- '12047' is the custom error code '12' and the errno code '047' or just '47'.
- '10111' is the custom error code '10' and the errno code '111'.

The custom codes are documented here, but the 'errno' code meaning will change depending on what caused the errno code to be set and you should investigate this yourself:
- 10xxx = socket creation failed in the constructor. Typically an exception is thrown here and you might not be able to access this number on an instantiated object, but 'errno' will be accessible.
- 14xxx = Setting socket options failed, the last 3 digits will be 'errno' and will provide more specific details.
- 15xxx = Binding the socket failed, the last 3 digits will be 'errno' and will provide more specific details.
- 16xxx = Starting to listen on a network port failed, the last 3 digits will be 'errno' and will provide more specific details.
- 17xxx = Attempting to accept the next connection in the queue failed, the last 3 digits will be 'errno' and will provide more specific details.

### Dependencies

This class requires the custom [logger class](#log---a-custom-and-configurable-logger) to log unique errors.

### Usage

- Initialise TCP Server object:
```
TcpServer *server = new TcpServer();
TcpServer *server = new TcpServer(TcpServer::InternetProtocol::v6);
```
- Listen to a specific port number:
```
server->StartListening(1234);
```
- Get the next connection in the queue (Blocking call):
```
int fd = server->NextConnection();
```
- Use the returned fd integer to store reference to a client socket:
```
TcpClient *client = new TcpClient(server->NextConnection());
```
- Retrieve error message and codes:
```
std::string errmsg = client->ERR_MSG();
int errcode = client->ERR_NO();
```

## Binary descriptions

Binary programs have been included in this project, but are not built in this project. They have been included here as common tools used across all systems I develop on and may have limited use for most users.

### cppnamelint (third-party)

The [CppNameLint project](https://github.com/dougpuob/cppnamelint/tree/master) was created as a way of defining custom naming conventions and applying them to your code. '.toml' files are used in conjunction with this binary file and a source file (or multiple source files) to quickly produce a report of what is and is not obeying the defined conventions.

This has been included for testing, along with a custom config file.

### Automated Version Incrementor program

The Version Incrementor program has been created to quickly manage automated version number incrementation when building with GNU make. A make recipe should be defined that triggers the build commands and this binary program.

This program accepts a file path to the header file that is being incremented, and a flag to denote which version number is being incremented. This version numbering system follows [semantic versioning 2.0.0](https://semver.org/) system, with the added number for BUILD that is automatically incremented if a flag is omitted from the Version Incrementor command line args list.

The header file is created if it does not exist, and behaviour is undefined is passing in a directory as the argument for the file path. Use the -h flag with the Version Incrementor to see the list of arguments and their expected formats.

## Installation and Build steps

- This project requires no building as the files are simply to be copied directly to an appropriate location.
- Running `make` will build the test files.
- Run `sudo make install` and the files will be copied into the required or specified folders.
- The C++ source files will be copied to the local include's directory, the binary files will be copied to `/usr/bin` and the cppnamelint config files will be copied to `/usr/local/bin/lint_config` by default. You can alternatively specify their include directories by appending the `sudo make install` command with `src_at`, `bin_at` and `lint_config_at`, followed by the desired file paths, for example: `sudo make install src_at="/my/source/file/path" bin_at="/my/bin/file/path" lint_config_at="/my/lint/config/path"`.
- The install location should not be directly in the base includes folder as some files could clash with existing file names in the C++ language or other installed libraries, so make sure to install them in a sub directory within the includes directory if you're installing them in custom locations.
