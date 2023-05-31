# Custom Library

## Table of contents

- [Brief](#brief)
- [Project Contents](#project-contents)
- [Description](#description)
    - [Disclaimer](#disclaimer)
- [Dependencies](#dependencies)
- [Installation / Build steps](#installation-and-build-steps)

## Brief

Custom C++ header files to simplify a number of common functions and operations.
To be installed in the includes folder.

## Project Contents

- Basic make file to run the installation of custom classes and build and run
    tests on each class file.
- A C++ logger class for customising and controlling output: directories,
    filenames, date and time stamps, class file name and line numbers, and a few
    other features. Can be disabled to stop printing anything if desired, such
    as log lines in tests.
- A C++ String object class that has some higher level functions similar to a C# 
    class, such as '.replace', '.split' and '.contains'.
- A Command Line Interface argument parser to allow a developer to quickly and 
    efficiently create and manage command line argument options.
- A C++ Utilities class with some common helper functions such as confirming a
    string is a valid IP address, validating a string as an integer, counting
    the number of times a specific string occurs within another string and
    printing the program running time using std::chrono C++ functions.
- A class for creating unit tests in C++. The catch.hpp file allows for 
    TEST_CASE functions to be created, built and ran as desired. This is not
    owned by us.

## Description
This library of classes can be used standalone for the most part (dependancies
listed below) and allows a developer to quickly write fast, efficient and
manageable code for all types of C/C++ projects (C compatibility will need to be
tested on a case-by-case basis).

### Disclaimer
The 'catch.hpp' is not something I own or developed, and is part of the
'catch2' library to help developers build and run unit tests in C++ programs. I
own no rights to this file and cannot guarantee its stability or security.

## Dependencies

cli_parser.h:
The cli parser requires the log class to print out, although the log lines can
be commented out or disabled, and the C++ utilities class for access to the
function that counts occurrences of a string within another string. The way to
define the logger in the custom code will determine how the cli parser prints
log information.

## Installation and Build steps

Run `sudo make install` and the files will be copied into the includes folder
and will create a baseline directory or the user can specify one. The install
location should not be directly in the includes folder as some files could clash
with existing file names, so make sure to install them in a sub directory within
the includes directory.
