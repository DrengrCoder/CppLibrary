# Custom C++ class file library

## Table of contents

- [Brief](#brief)
- [Project Contents](#project-contents)
- [Description](#description)
    - [Log - A custom and configurable logger](#log---a-custom-and-configurable-logger)
        - [Credit](#credit)
    - [String - A custom String class](#string---a-custom-string-class)
        - [Credit](#credit-1)
    - [C++ Utilities](#c-utilities)
    - [Command Line Interface (CLI) parser](#command-line-interface-cli-parser)
        - [The constructors and input parameters](#the-constructors-and-input-parameters)
        - [The types of options](#the-types-of-options)
    - [Disclaimer](#disclaimer)
- [Dependencies](#dependencies)
    - [cli_parser.h](#cli_parserh)
- [Installation / Build steps](#installation-and-build-steps)

## Brief

Custom C++ header files to simplify a number of common functions and operations. To be installed in the includes folder.

## Project Contents

- Basic make file to run the installation of custom classes and build and run tests on each class file.
- A C++ logger class for customising and controlling output: directories, filenames, date and time stamps, class file name and line numbers, and a few other features. Can be disabled to stop printing anything if desired, such as log lines in tests.
- A C++ String object class that has some higher level functions similar to a C# class, such as '.replace', '.split' and '.contains'.
- A Command Line Interface argument parser to allow a developer to quickly and efficiently create and manage command line argument options.
- A C++ Utilities class with some common helper functions such as confirming a string is a valid IP address, validating a string as an integer, counting the number of times a specific string occurs within another string and printing the program running time using std::chrono C++ functions.
- A class for creating unit tests in C++. The catch.hpp file allows for TEST_CASE functions to be created, built and ran as desired. This is not owned by us.

## Description
This library of classes can be used standalone for the most part (dependancies listed below) and allows a developer to quickly write fast, efficient and manageable code for all types of C/C++ projects (C compatibility will need to be tested on a case-by-case basis).

### Log - A custom and configurable logger

This class can print out to `std::cout` and/or to a text file with varying levels of log lines from low level information up to fatal. The directory the log files go to can be slightly configured and you're free to add your own options. The code comments in the file and code docs explain in detail how to setup and use this class. The format of the text is probably easier to read directly in the file.

The `LOG_SETTINGS` extern variable can be used to configure settings, and the `LogInit` function will reinitialise the logger with those settings. The logger will automatically print to `std::cout` before `LogInit` is called so nothing will need to be set up before you can start using the `log` macro's defined in the file, but if you want to change settings, such as printing to a file or toggling the filename and line number the log lines come from, you will need to change the settings on `LOG_SETTINGS` and call `LogInit` to set any changes. Calling `LogInit` will generate a 'LogFiles' folder in the appropriate location once called.

#### Credit

This class has been modified from an original forum post I found [here](https://levelup.gitconnected.com/logging-in-c-60cd1571df15) but has been contained in a single header file. I modified this implementation to automatically call `std::endl` so I didn't have to remember to do it myself at the end of every line, and I created several macro's that include the `__FILE__` and `__LINE__` macro's so the filename and line number the `log` macro's are called from is pulled in to the log line and can be printed to the text file and `std::cout`.

### String - A custom String class

This class implements several common functions and loops you might use in C++ to achieve the relevant functionality, but contained in a single header file for ease-of-use. This class is still a work in progress and although many tests have been performed, there is no certainty that everything will work in every case, but as many things as possible has been accounted for at this time.

The `String` is null-terminated correctly, but please report any errors you experience.

**Things to note**: This class can be assigned `char`'s, `const char*`, `std::string`'s and other `String` class objects. Concatenation and insertion functions work on this class, and a number of combinations of parameter lists have been considered for different functions. This class can also return **both** a `const char*` and a `std::string`, but when used in a function that accepts a `const char*` and a `std::string` in the same index position in the parameter list, you will need to force it to return a `const char*` by calling `c_str()` on the `String` object, otherwise you experience an error because your code is not sure which variable type you are trying to use and therefore, which function parameter list you are trying to specify.

#### Credit

This class was developed based on a [Geek for Geeks](https://www.geeksforgeeks.org/how-to-create-a-custom-string-class-in-c-with-basic-functionalities/) post, although partially modified for additional functionality.

### C++ Utilities

The utilities class solely contains a few static functions for various features and you should check the file for specific information. Nothing here is unique or proprietary and code can be found on a simple google search, but has been included in a file in this project as I tend to use them a lot in some of my projects.

### Command Line Interface (CLI) parser

Most of this code is completely unique, and I have developed everything from scratch (to an extent), but the original implementation was around the design of the command line argument parser and parser option class found in the [QT libraries](https://www.qt.io/).

This class allows a developer to quickly define and manage arguments that can be passed in on the command line for binary file execution. This will probably not do everything you want it to, but it does everything I want it to, and has been developed in a way to minimise the code I need to write. You should carefully examine the constructor list and their input parameters and code docs for detailed instructions on how to use this class, but here are a few points:

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

### Disclaimer
The 'catch.hpp' is not something I own or developed, and is part of the 'catch2' library to help developers build and run unit tests in C++ programs. I own no rights to this file and cannot guarantee its stability or security.

## Dependencies

### cli_parser.h
The cli parser requires the log class to print out to std::cout and text files, although the log lines can be commented out or disabled, and the C++ utilities class for access to the function that counts occurrences of a string within another string. The way you define the logger in your code will determine how the cli parser prints log information.

## Installation and Build steps

Run `sudo make install` and the files will be copied into the includes folder and will create a default directory or the user can specify one. The install location should not be directly in the includes folder as some files could clash with existing file names, so make sure to install them in a sub directory within the includes directory.
