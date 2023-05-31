//
// Created by Dylan Andrew McAdam on 23/05/23.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__

#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#pragma GCC diagnostic ignored "-Wunused-function"

/**
 * @brief   Defines the start time of the application, based on the current
 *          system clock. This value cannot be changed.
 */
static const auto __START = std::chrono::high_resolution_clock::now();
/**
 * @brief   Defines a point in time the current running time was requested by
 *          another function. Called and converted into seconds in the following
 *          functions: CurrentProgramRunningTime_Microseconds, 
 *          CurrentProgramRunningTime_Milliseconds, CurrentProgramRunningTime.
 */
static auto __END = std::chrono::high_resolution_clock::now();

/**
 * @brief   Retrieve the current running time of the program since it started,
 *          defined by the '__START' variable, returned as an 'auto' variable
 *          type (At time of writing, the return type is int64_t).
 * 
 * @return  An 'auto' variable type representing the current running time in
 *          microseconds (1 / 1,000,000 seconds).
 */
static auto CurrentProgramRunningTime_Microseconds(){
    __END = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(__END - __START).count();
    return us;
}

/**
 * @brief   Retrieve the current running time of the program since it started,
 *          defined by the '__START' variable, returned as an 'auto' variable
 *          type (At time of writing, the return type is double).
 * 
 * @return  An 'auto' variable type representing the current running time in
 *          microseconds (1 / 1,000 seconds).
 */
static auto CurrentProgramRunningTime_Milliseconds(){
    __END = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(__END - __START).count();
    auto ms = us / 1000.0;
    return ms;
}

/**
 * @brief   Retrieve the current running time of the program since it started,
 *          defined by the '__START' variable, returned as a std::string. This
 *          string is return as: "Current running time: 'X' Microseconds ( 'X'
 *          milliseconds )"
 * 
 * @return  A std::string printing the current running time of the program in
 *          microseconds and either seconds or milliseconds if the running time
 *          was less than 1 second.
 */
static std::string CurrentProgramRunningTime(){
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
 * @brief   Determine if the input string is a valid integer number.
 * 
 * @param str       The std::string to validate.
 * @return true     if the string is a valid integer,
 * @return false    otherwise.
 */
static bool StringIsInteger(std::string &str){
    //  Make sure the string isn't empty and only contains numbers
    return !str.empty() && str.find_first_not_of("[0123456789]")
                == std::string::npos;
}

/**
 * @brief   Determine if the input string is valid as an IPv4 IP Adress.
 * 
 * @param str       The std::string to validate.
 * @return true     if the string is valid as an IPv4 IP address,
 * @return false    otherwise.
 */
static bool StringIsValidIp(std::string &str){
    //  Spliting the string using native C++ code
    std::stringstream inputSs(str);
    std::vector<std::string> numList;
    std::string currentStringPlaceholder;
    while (std::getline(inputSs, currentStringPlaceholder, '.')){
        numList.push_back(currentStringPlaceholder);
    }
    
    //  Make sure there are a total of 4 numbers
    if (numList.size() != 4) return false;
    
    for (std::string str : numList){
        //  Make sure every number is a valid number and within range
        if (!StringIsInteger(str) || std::stoi(str) > 255 || std::stoi(str) < 0){
            return false;
        }
    }

    return true;
}

/**
 * @brief   Counts the number of occurrences a specific 'target' string appears
 *          in a 'string to search'.
 * 
 * @param stringToSearch    The string to search through.
 * @param target            The target string to search for.
 * @param caseSensitive     True if you want the search to be case sensitive,
 *                          false otherwise.
 * @return                  An integer for the number of times the 'target' 
 *                          appeared in the 'string to search' 
 */
static int CountOccurrences(std::string stringToSearch, std::string target, 
        bool caseSensitive = true){
    int occurrences = 0;
    std::string::size_type pos = 0;

    if (!caseSensitive){
        std::stringstream oss;
        for (char c : stringToSearch){
            oss << char(tolower(c));
        }
        stringToSearch = oss.str();
        oss.str(std::string());
        for (char c : target){
            oss << char(tolower(c));
        }
        target = oss.str();
    }

    while ((pos = stringToSearch.find(target, pos)) != std::string::npos) {
            ++occurrences;
            pos += target.length();
    }
    return occurrences;
}

/**
 * @brief Handling POSIX signals
 * 
 * Include:
 *      #include <csignal>
 * 
 * Add handlers to POSIX signals:
 *      signal(SIGINT, signalHandler);
 *      signal(SIGILL, signalHandler);
 *      signal(SIGABRT, signalHandler);
 *      signal(SIGFPE, signalHandler);
 *      signal(SIGKILL, signalHandler);
 *      signal(SIGSEGV, signalHandler);
 *      signal(SIGTERM, signalHandler);
 * 
 * Signal handler function:
 *      bool __PROGRAM_RUNNING = true;
 *      void signalHandler(int signum){
 *
 *          if (signum == SIGINT){          //  2
 *              dlog << "SIGINT Received.";
 *              log << "Typically an interrupt key from keyboard.";
 *          }
 *          else if (signum == SIGILL){    //  4
 *              dlog << "SIGILL Received.";
 *          }
 *          else if (signum == SIGABRT){   //  6
 *              dlog << "SIGABRT Received.";
 *              log << "Typically a system or program error from unhandled exceptions.";
 *          }
 *          else if (signum == SIGFPE){    //  8
 *              dlog << "SIGFPE Received.";
 *              log << "Typically an erroneous arithmetic operation.";
 *          }
 *          else if (signum == SIGKILL){   //  9
 *              dlog << "SIGKILL Received.";
 *              log << "Typically a kill command received from outside the program.";
 *          }
 *          else if (signum == SIGSEGV){   //  11
 *              dlog << "SIGSEGV Received: EMERGENCY EXIT PROGRAM NOW.";
 *              log << "Segmentation fault, exiting program immediately.";
 *              exit(0);
 *          }
 *          else if (signum == SIGTERM){   //  15
 *              dlog << "SIGTERM Received.";
 *              log << "Typically a terminate command received from outside the program.";
 *          }
 *
 *          __PROGRAM_RUNNING = false;
 *      }
 *
 * 
 */

#pragma GCC diagnostic pop

#endif  //  __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CPP_UTILITIES_H__