# Custom C++ class file library

## Table of contents

## Brief

Custom C++ header files to simplify a number of common functions and operations. To be installed in the includes folder.

This project also includes a number of third-party classes and binary programs that are extremely useful for most projects and developers. Their respective license files are included under the Licenses folder, appended with their project name, and the git repository they can be accessed from (at time of writing on 31/08/2023) at the top of the file.

## Project Contents

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

This library of classes can be used standalone for the most part (dependancies listed) and allows a developer to quickly write fast, efficient and manageable code for all types of C/C++ projects (C compatibility will need to be tested on a case-by-case basis, but all classes developed for C++ compatibility).

### Catch.hpp (third-party)

This header file allows you to write unit tests for C++ programs and comes from the 'Catch2' library. This has been included in the project to be used in another projects as desired, and is also used by the tests in this project.

#### Credit

Disclaimer: The 'catch.hpp' is not something I own or developed, and is part of the [Catch2](https://github.com/catchorg/Catch2) library to help developers build and run unit tests in C++ programs. I own no rights to this file and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

### Command line interface parser (cli_parser.h)

Most of this code is completely unique, and I have developed everything from scratch (to an extent), but the original implementation was influenced by the functionality of the command line argument parser and parser option class found in the [QT libraries](https://www.qt.io/).

This class allows a developer to quickly define and manage arguments that can be passed in on the command line for binary file execution. This will probably not do everything you want it to, but it does everything I want it to, and has been developed in a way to minimise the code I need to write. You should carefully examine the constructor list and their input parameters and code docs for detailed instructions on how to use this class:

#### The constructors and input parameters

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

#### The types of options

- You can define a flag-based argument (it either exists or it doesn't), by simply defining a tag and a description. Used to define boolean related variables.

- You can define a data-based argument, with or without a default value after it. If there is no default value and the argument is found, it expects to find some sort of data after it.

- You can define a data-based argument with a specific list of choices, with or without a default value after it. If there is no default value and the argument is found, it expects to find some sort of data after it, and the data value will be checked that it is a valid choice from the list of input choices. 

#### Credit

- The [Qt Library Command Line Parser Class](https://doc.qt.io/qt-6/qcommandlineparser.html).
- The [Qt Library Command Line Option Class](https://doc.qt.io/qt-6/qcommandlineoption.html).

#### Dependencies

This class requires the custom logger class and the custom C++ utilities class for some additional functions.

### C++ Utilities

The utilities class solely contains a few static functions for various features and you should check the file for specific information. Nothing here is unique or proprietary and code can be found on a simple google search, but has been included in a file in this project as I tend to use them a lot in some of my projects. This class is a dependency to other custom classes.

### HTTPRequest.hpp (third-party)

This header file allows you to issue HTTP Requests to Web-based URL's, and implements the standards for HTTP/URI syntax defined by IETF. This header file as very complex and thorough, and you should investigate all the code through yourself as required.

#### Credit

Disclaimer: The 'HTTPRequest.hpp' is not something I own or developed, and is part of the [HTTPRequest](https://github.com/elnormous/HTTPRequest/tree/master) project. I own no rights to this file and cannot guarantee its stability or security. The license for this project has been added under the Licencses folder.

### Log - A custom and configurable logger

This class can print out to `std::cout` and/or to a text file with varying levels of log lines from low level information up to fatal. The directory the log files go to can be slightly configured and you're free to add your own options. The code comments in the file and code docs explain in detail how to setup and use this class. The format of the text is probably easier to read directly in the file.

The `LOG_SETTINGS` extern variable can be used to configure settings, and the `LogInit` function will reinitialise the logger with those settings. The logger will automatically print to `std::cout` before `LogInit` is called so nothing will need to be set up before you can start using the `log` macro's defined in the file, but if you want to change settings, such as printing to a file or toggling the filename and line number the log lines come from, you will need to change the settings on `LOG_SETTINGS` and call `LogInit` to set any changes. Calling `LogInit` will generate a 'LogFiles' folder in the appropriate location once called.

#### Credit

This class has been modified from an original forum post I found [here](https://levelup.gitconnected.com/logging-in-c-60cd1571df15) but has been contained in a single header file. I modified this implementation to automatically call `std::endl` so I didn't have to remember to do it myself at the end of every line, and I created several macro's that include the `__FILE__` and `__LINE__` macro's so the filename and line number the `log` macro's are called from is pulled in to the log line and can be printed to the text file and `std::cout`.

### Nlohmann JSON (third-party)

The entire library folder and its contents are required but the single json.hpp file can be used as an include statement in any file that needs JSON objects.

#### Credit

The [JSON libraries](https://github.com/nlohmann/json) provided by [nlohmann](https://github.com/nlohmann) is widely regarded as the standard for JSON object manipulation by many developers, and the library folder has been included in this project as it is something I commonly use across a lot of programs I create.

### Proxy-based HTTP Request header

This header file uses the custom TCP Client class to create network sockets that allow data transfer to API's on devices with different IP address ranges.

#### Credit

This file has been heavily influenced by the [HTTPRequest project](https://github.com/elnormous/HTTPRequest/tree/master), but with some minor modifications for my custom naming conventions, additional code docs and a customised class enum type to add custom functions.

#### Dependencies

This class requires the custom logger class, the custom TCP Client class and the custom String class for some additional functions.

### RapidXML (third-party)

The rapid XML project includes 3 header files but only 2 are required: the base 'rapidxml.hpp' and 'rapidxml_print.hpp' file to provide the functionality required and print-friendly functions respectively.

#### Credit

The [original project](https://github.com/Fe-Bell/RapidXML) and the [forked repository](https://github.com/viriw/rapidxml) contain these files, although the forked repository derivatives are easier and simpler to use.

### String - A custom String class

This class implements several common functions and loops you might use in C++ to achieve the relevant functionality, but contained in a single header file for ease-of-use. This class is still a work in progress and although many tests have been performed, there is no certainty that everything will work in every case, but as many things as possible has been accounted for at this time.

The `String` is null-terminated correctly, but please report any errors you experience.

Nothing here is especially unique, but all functions have been collected into a single header file.

**Things to note**:
1. This class can be assigned `char`'s, `const char*`, `std::string`'s and other `String` class objects.
2. Concatenation and insertion functions work on this class, and a number of combinations of parameter lists have been considered for different functions.
3. This class can return **both** a `const char*` and a `std::string`, but when used in a function that accepts a `const char*` and a `std::string` in the same index position in the parameter list, you will need to force it to return a `const char*` by calling `c_str()` on the `String` object, otherwise you experience an error because your code is not sure which variable type you are trying to use and therefore, which function parameter list you are trying to specify.
4. While you can concatenate two `char`'s and assign the concatenation to a `String` object, it is worth noting that 'adding' two `char`'s to each other produces a different `char` because they are essentially integers.

#### Credit

This class was developed based on a [Geek for Geeks](https://www.geeksforgeeks.org/how-to-create-a-custom-string-class-in-c-with-basic-functionalities/) post, although partially modified for additional functionality.

### TCP Client network socket class

Network sockets are originally very basic and must be configured for TCP or UDP type data streams. You do not need to be familiar with how network sockets work to use this class, but this class makes it simple to implement client-side functionality, with appropriate functions to initialise and connect to network sockets, applying some common functions and argument options that would be expected to configure a socket for a TCP-based data stream connection.

Nothing here is especially unique and can be learned after a few google searches and looking at the right forum posts, but the simplest implementations have been created. This class can be inherited to provide more advanced functionality. If you need to rewrite these classes for your own purposes, I suggest learning more about network sockets and network programming theory and practice before attempting to do something too complex.

#### Error codes

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

#### Dependencies

This class requires the custom logger class to log unique errors.

### TCP Server network socket class

Network sockets are originally very basic and must be configured for TCP or UDP type data streams. You do not need to be familiar with how network sockets work to use this class, but this class makes it simple to implement server-side functionality, with appropriate functions to initialise and listen to network socket ports, applying some common functions and argument options that would be expected to configure a socket for a TCP-based data stream connection.

Nothing here is especially unique and can be learned after a few google searches and looking at the right forum posts, but the simplest implementations have been created. This class can be inherited to provide more advanced functionality. If you need to rewrite these classes for your own purposes, I suggest learning more about network sockets and network programming theory and practice before attempting to do something too complex.

#### Error codes

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

## Installation and Build steps

- This project requires no building as the files are simply to be copied directly to an appropriate location.
- Running `make` will build the test files.
- Run `sudo make install` and the files will be copied into the required or specified folders.
- The C++ source files will be copied to the local include's directory, the binary files will be copied to `/usr/bin` and the cppnamelint config files will be copied to `/usr/local/bin/lint_config` by default. You can alternatively specify their include directories by appending the `sudo make install` command with `src_at`, `bin_at` and `lint_config_at`, followed by the desired file paths, for example: `sudo make install src_at="/my/source/file/path" bin_at="/my/bin/file/path" lint_config_at="/my/lint/config/path"`.
- The install location should not be directly in the base includes folder as some files could clash with existing file names in the C++ language or other installed libraries, so make sure to install them in a sub directory within the includes directory.
