//
// Created by Dylan Andrew McAdam (DrengrCoder) on 23/05/23.
//

#ifndef __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__
#define __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__

#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#pragma GCC diagnostic ignored "-Wunused-function"

/**
 * The start time of the application, based on the std::chrono high resolution
 * clock for the current system time. This value cannot be changed. This value
 * is set when this header is included in another file.
 */
static const auto __START = std::chrono::high_resolution_clock::now();
/**
 * The current running time, based on the std::chrono high resolution
 * clock for the current system time. This is updated in
 * 'CurrentProgramRunningTime_Microseconds',
 * 'CurrentProgramRunningTime_Milliseconds', and
 * 'CurrentProgramRunningTime' functions to calculate elapsed running
 * time for the program. These variables simply make it quick and easy
 * to define elapsed running time.
 */
static auto __END = std::chrono::high_resolution_clock::now();

/**
 * Calculate and return the current running time of the program in
 * microseconds, as int64_t (1,000,000 microseconds (us) = 1 second (s)).
 */
static auto CurrentProgramRunningTime_Microseconds() {
    __END = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(__END - __START).count();
    return us;
}

/**
 * Calculate and return the current running time of the program in
 * milliseconds, as double (1,000 milliseconds (ms) = 1 second (s)).
 */
static auto CurrentProgramRunningTime_Milliseconds() {
    __END = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(__END - __START).count();
    auto ms = us / 1000.0;
    return ms;
}

/**
 * Calculate and return the current running time of the
 * program as a string, printing microseconds and either
 * seconds or milliseconds, depending on numeric size
 * (1,000,000 us = 1,000 ms = 1 s).
 */
static std::string CurrentProgramRunningTime() {
    __END = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(__END - __START).count();
    auto s = us / 1000000.0;
    auto ms = us / 1000.0;
    std::stringstream output;
    output << "Current running time: " << us << " Microseconds ("
        << (s < 1.0 ? std::to_string(ms) + " milliseconds" : std::to_string(s) + " seconds")
        << ")";
    return output.str();
}

/**
 * Return true if STR is a valid integer number, false otherwise.
 */
static bool StringIsInteger(std::string& str) {
    //  Make sure the string isn't empty and only contains numbers
    return !str.empty() && str.find_first_not_of("[0123456789]")
        == std::string::npos;
}

/**
 * Return true if STR is valid as an IPv4 IP Address, false otherwise.
 */
static bool StringIsValidIp(std::string& str) {
    //  Spliting the string using native C++ code
    std::stringstream inputSs(str);
    std::vector<std::string> numList;
    std::string currentStringPlaceholder;
    while (std::getline(inputSs, currentStringPlaceholder, '.')) {
        numList.push_back(currentStringPlaceholder);
    }

    //  Make sure there are a total of 4 numbers
    if (numList.size() != 4) return false;

    for (std::string str : numList) {
        //  Make sure every number is a valid number and within range
        if (!StringIsInteger(str) || std::stoi(str) > 255 || std::stoi(str) < 0) {
            return false;
        }
    }

    return true;
}

/**
 * @brief   Counts the number of occurrences TARGET appears in SEARCH.
 *
 * @param search            The string to search through.
 * @param target            The target to search for.
 * @param caseSensitive     True if you want the search to be case sensitive.
 * @return                  The number of times TARGET appears in SEARCH.
 */
static int CountOccurrences(std::string search, std::string target,
        bool caseSensitive = true) {
    int occurrences = 0;
    std::string::size_type pos = 0;

    if (!caseSensitive) {
        std::stringstream oss;
        for (char c : search) {
            oss << char(tolower(c));
        }
        search = oss.str();
        oss.str(std::string());
        for (char c : target) {
            oss << char(tolower(c));
        }
        target = oss.str();
    }

    while ((pos = search.find(target, pos)) != std::string::npos) {
        ++occurrences;
        pos += target.length();
    }
    return occurrences;
}

#pragma GCC diagnostic pop

#endif  //  __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__