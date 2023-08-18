//
// Created by Dylan Andrew McAdam on 28/11/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_LOG_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_LOG_H__

#include <limits.h>
#include <fstream>      //  File stream objects
#include <iostream>     //  Input/output stream objects
#include <sstream>      //  String stream objects
#include <string>
#include <string.h>
#include <unistd.h>     //  POSIX OS API ???
#include <sys/stat.h>   //  System info
#include <chrono>       //  Time functions
#include <ctime>
#include <iomanip>

extern class LogSettings LOG_SETTINGS;

/**
 * @brief   Initialises the logging system. This should be called at the start
 *          of int main to initialise a log file in the launch directory in the
 *          event there is an early error, and once again later initialised with
 *          custom settings before calling main code.
 * 
 * @param argv  The 'argv' parameter pulled in from int main.
 * @throw       RuntimeError if there was an error trying to open the log file.
 *              if 'ls_print_to_file' is set to false before this is called,
 *              this error is avoided.
 */
#define LogInit(argv) LOG_SETTINGS.init(argv);
/**
 * @brief   Initialises the logging system for TEST CASE CODE ONLY. This should
 *          be called at the start of the first test case in each file and
 *          initialised with any desired settings. The most common setting that
 *          should be set first is the 'ls_print_to_file' and this should be set
 *          to true before calling this init function.
 * 
 * @throw   RuntimeError if there was an error trying to open the log file. If
 *          'ls_print_to_file' is set to false before this is called, this error
 *          is avoided.
 */
#define TestLogInit LOG_SETTINGS.tests_init();

/**
 * @brief Closes the output file stream.
 */
#define LogShutdown LOG_SETTINGS.shutdown();
/**
 * @brief log << "LT_LL_INFO type";
 */
#define clog Log(LT_LL_INFO, __FILE__, __LINE__)
/**
 * @brief ilog << "LT_INFO type";
 */
#define ilog Log(LT_INFO, __FILE__, __LINE__)
/**
 * @brief dlog << "LT_DEBUG type";
 */
#define dlog Log(LT_DEBUG, __FILE__, __LINE__)
/**
 * @brief wlog << "LT_WARN type";
 * 
 */
#define wlog Log(LT_WARN, __FILE__, __LINE__)
/**
 * @brief elog << "LT_ERROR type";
 */
#define elog Log(LT_ERROR, __FILE__, __LINE__)
/**
 * @brief flog << "LT_FATAL type";
 */
#define flog Log(LT_FATAL, __FILE__, __LINE__)

/**
 * @brief   Logging levels enumerator, for label printing and including / 
 *          excluding certain log levels as needed through the command line
 *          arguments.
 * 
 */
enum LogType {
//  Level:             Brief description:          Examples:
    LT_LL_INFO,    //  Low-level, granular info.   This var set to this val
    LT_INFO,       //  Standard info.              Object successfully created
    LT_DEBUG,      //  Debug informtion.           Return value of functions or conditions, reporting values of certain var's in certain places
    LT_WARN,       //  Warning information.        Something occurred that probably shouldn't have, but has not caused any known issue at this point. Warning dialogs should be generated
    LT_ERROR,      //  Error information.          Something bad happened that could have caused the program to crash, but was handled successfully. Error dialogs should be generated
    LT_FATAL       //  Fatal error information.    Something happened that caused the program to crash, and we did not handle it.
};

/**
 * @brief 
 * Extern Log Settings class, to hold data exclusively separate from the Log 
 * class as the Log class is being instantiated and destroyed at every call.
 *
 * Example use:
 *      <code>LogShutdown</code> should be used at the end of any execution 
 *      code to make sure the log file is closed. Most of the time there will 
 *      never be an issue because the program will release its resources when 
 *      execution ends, but this will further prevent any issues.
 *
 *      Declare this include below any main library includes:
 *          #include "Log.h"
 *
 *      Declare this under any include statements above 
 *      int main(int argc, char *argv[]), in main.cpp:
 *          LogSettings LOG_SETTINGS;
 *
 *      This should be declared at the top of int main in case of an early error 
 *      (A parser error if you use a command line arg parser), but can be 
 *      commented out at any time:
 *          try {
 *              LogInit(argv);
 *          } catch (std::runtime_error err) {
 *              std::cout << err.what() << std::endl;
 *              exit(0);
 *          } catch (std::exception e) {
 *              std::cout << "Program is unable to start: " << e.what() << std::endl;
 *              exit(0);
 *          }
 *
 *      Optionally declare this with some specific settings before any main 
 *      program code executes, as well as or instead of the previous declaration 
 *      (This gives you the option to specify some settings found in a custom 
 *      command line parser, see full list of public variables for options):
 *          LOG_SETTINGS.ls_selected_level = LT_LL_INFO;
 *          LOG_SETTINGS.ls_use_working_dir = true;
 *          LogInit(argv);
 *
 *      Use the log macro's at the top of this file to log unique log levels, 
 *      see the full list of log levels above (Setting a different <code>.ls_selected_level</code> 
 *      will print those types of logs and above, with LT_LL_INFO will log 
 *      everything, and LT_FATAL will only log fatal error messages).
 * 
 */
class LogSettings {
public:
    /**
     * @brief   Denoting the log level determines what is printed out to the log
     *          file (and the conosle when in an IDE). By default, LL_INFO for
     *          all log lines.
     */
    LogType ls_selected_level = LT_LL_INFO;

    /**
     * @brief   True if you want to log to the working directory, false
     *          otherwise. By default, False and logs to launch directory.
     */
    bool ls_use_working_dir = false;

    /**
     * @brief   True if you want to overwrite the existing file, false
     *          otherwise. By default, False and will append date and time
     *          information to the end of the file name.
     */
    bool ls_overwrite = false;

    /**
     * @brief   True if you want to log to a file, false otherwise. By default,
     *          True and will log to a file.
     */
    bool ls_print_to_file = false;

    /**
     * @brief   True if you want to log to the std::cout console, false
     *          otherwise. By default, True and will log to std::cout.
     */
    bool ls_print_to_debug = true;

    /**
     * @brief   True if you want to log the system date and time, false
     *          otherwise. (System date and time shouold always be UTC). By
     *          default, True and will log the system date and time with every
     *          log line.
     */
    bool ls_print_datetime = true;

    /**
     * @brief   True if you want to log the file name and line number this lo
     *          line came from, false otherwise. By default, True and will log
     *          file name and line number the log line comes from.
     */
    bool ls_print_fileline = true;

    /**
     * @brief   The output file stream. Holds the reference to the file to log
     *          to.
     */
    std::ofstream ls_ofs;

//  Disable the format warning and unused variable warning around these 
//  functions to stop the format error with getting milliseconds and a disposed
//  variable.
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wunused-variable"

    void init(char **argv) {
        //  Create buffer
        char buff[PATH_MAX];
        memset(buff, 0, PATH_MAX);

        //  Get executable location
        ssize_t dsp = readlink("/proc/self/exe", buff, PATH_MAX);
        std::string r = buff;
        std::string exe_dir = r.substr(0, r.find_last_of('/'));
        ls_launch_dir = exe_dir + "/LogFiles";

        memset(buff, 0, PATH_MAX);

        //  Get working directory from terminal
        getcwd(buff, sizeof(buff));
        std::string wrk_dir = buff;
        ls_working_dir = wrk_dir + "/LogFiles";

        //  There is always one argument, and the first is always the command 
        //  to run the programs binary executable
        std::string firstArg = argv[0];
        //  Get the project or program name as the last item in the '/' 
        //  (forward-slash) delimited list
        ls_file_name = firstArg.substr(firstArg.find_last_of('/') + 1, 
                                        firstArg.size()) + ".txt";

        //  Get desired path
        const std::string chosenPath = ls_use_working_dir ? 
                                    ls_working_dir : ls_launch_dir;

        //  Make dir if it does not exist, returns -1 if it already exists
        int err = mkdir(chosenPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
        if (err < -1) {
            std::cout << "Error creating log file directory: "
                      << std::to_string(err) << "." << std::endl;
        }

        if (ls_ofs.is_open()) {
            ls_ofs.close();
        }

        if (!ls_print_to_file) {
            return;
        }

        //  Create / open file as needed
        if (ls_overwrite) {
            //  Overwriting means creating or opening a file without a date and 
            //  timestamp
            ls_ofs.open(chosenPath + '/' + ls_file_name);
        } else {
            //  Not overwriting means create a new file and appending date and 
            //  time, to milliseconds
            using namespace std::chrono;
            auto now = system_clock::now();

            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            char ms_char[3];
            sprintf(ms_char, "%03i", ms);

            auto t = system_clock::to_time_t(now);
            std::tm real_time = *std::localtime(&t);

            std::ostringstream oss;
            oss << std::put_time(&real_time, "%Y-%m-%dT%H-%M-%S") << "." 
                << ms_char << "Z";

            ls_ofs.open(chosenPath + '/' + oss.str() + "_" + ls_file_name);
        }

        if (!ls_ofs.is_open()) {
            throw std::runtime_error("Log failed to initialise, could not open "
                                    "file or output file stream.");
        }
    }

    void tests_init(){
        //  Create buffer
        char buff[PATH_MAX];
        memset(buff, 0, PATH_MAX);

        //  Get executable location
        ssize_t dsp = readlink("/proc/self/exe", buff, PATH_MAX);
        std::string r = buff;
        std::string exe_dir = r.substr(0, r.find_last_of('/'));
        std::string test_launch_dir = exe_dir + "/LogFiles";

        //  There is always one argument, and the first is always the command 
        //  to run the programs binary executable
        std::string firstArg = buff;
        //  Get the project or program name as the last item in the '/' 
        //  (forward-slash) delimited list
        ls_file_name = firstArg.substr(firstArg.find_last_of('/') + 1, 
                                        firstArg.size()) + ".txt";

        int err = mkdir(test_launch_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
        if (err < -1) {
            std::cout << "Error creating log file directory: "
                      << std::to_string(err) << "." << std::endl;
        }

        if (ls_ofs.is_open()) {
            ls_ofs.close();
        }

        if (!ls_print_to_file) {
            return;
        }

        //  Create / open file as needed
        if (ls_overwrite) {
            //  Overwriting means creating or opening a file without a date and 
            //  timestamp
            ls_ofs.open(test_launch_dir + '/' + ls_file_name);
        } else {
            //  Not overwriting means create a new file and appending date and 
            //  time, to milliseconds
            using namespace std::chrono;
            auto now = system_clock::now();

            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            char ms_char[3];
            sprintf(ms_char, "%03i", ms);

            auto t = system_clock::to_time_t(now);
            std::tm real_time = *std::localtime(&t);

            std::ostringstream oss;
            oss << std::put_time(&real_time, "%Y-%m-%dT%H-%M-%S") << "." 
                << ms_char << "Z";

            ls_ofs.open(test_launch_dir + '/' + oss.str() + "_" + ls_file_name);
        }

        if (!ls_ofs.is_open()) {
            throw std::runtime_error("Log failed to initialise, could not open "
                                    "file or output file stream.");
        }
    }

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

    /**
     * @brief Closes the output file stream.
     */
    void shutdown() {
        ls_ofs.close();
    }

private:

    /**
     * @brief   The name of the file to log to, typically the program / app name
     *          with no spaces. Automatically set based on the input command
     *          arguments, where the first argument always contains the name of
     *          the binary executable.
     */
    std::string ls_file_name;

    /**
     * @brief   The working directory path. Calculated when LogInit is called.
     * 
     */
    std::string ls_working_dir;

    /**
     * @brief   The launch directory path. Calculated when LogInit is called.
     * 
     */
    std::string ls_launch_dir;

};

/**
 * @brief 
 * Custom Logger class. Each time one of the macro's is used it creates a new 
 * instance of the logger and appends all data to the string stream buffer. 
 * The destructor is called when the semi-colon is reached which flushes the 
 * buffer to the std::cout console and an output file stream. Only log lines of 
 * the specified <code>.ls_selected_level</code> and above are printed.
 *
 * Macro's:
 *      log << "LT_LL_INFO type";
 *      ilog << "LT_INFO type";
 *      dlog << "LT_DEBUG type";
 *      wlog << "LT_WARN type";
 *      elog << "LT_ERROR type";
 *      flog << "LT_FATAL type";
 */
class Log {
public:

    /**
     * @brief   Initialise a new Log instance to log some data.
     * 
     * @param type  The LogType for the debug level of this line.
     * @param file  The __FILE__ macro.
     * @param line  The __LINE__ macro.
     */
    Log(LogType type, const std::string &file, const int line) {
        if (type >= LOG_SETTINGS.ls_selected_level) {
            l_level = type;
            operator<<(prefix(file, line));
        }
    }

    /**
     * @brief   Destroy the Log instance using std::endl; on the output string
     *          stream to write the buffer to the std::cout console and to the
     *          output file stream.
     */
    ~Log(){
        if (l_level >= LOG_SETTINGS.ls_selected_level) {
            if (LOG_SETTINGS.ls_print_to_file) {
                LOG_SETTINGS.ls_ofs << l__SS.str() << std::endl;
            }
            if (LOG_SETTINGS.ls_print_to_debug) {
                std::cout << l__SS.str() << std::endl;
            }
        }
    }

    /**
     * @brief   Overridden insertion operator on custom Log class to append data
     *          to the current Log instance string stream buffer.
     * 
     * @tparam T    Any Type.
     * @param t     Data to append.
     * @return      This Log instance used.
     */
    template<typename T>
    Log &operator<<(T t) {
        if (l_level >= LOG_SETTINGS.ls_selected_level) {
            l__SS << t;
        }
        return *this;
    }

private:
    /**
     * @brief   The output string stream buffer.
     * 
     */
    std::ostringstream l__SS;
    
    /**
     * @brief   This Log instance's LogType level.
     * 
     */
    LogType l_level = LT_LL_INFO;

    /**
     * @brief   Prefix the Date and Time as well as file name and line number
     *          this log line was called.
     * 
     * @param file  The __FILE__ macro.
     * @param line  The __LINE__ macro.
     * @return      A formatted std::string holding the date, time, file name
     *              and line number.
     */
    std::string prefix(const std::string &file, const int line) {
        //  Init return object
        std::ostringstream output;

        //  Date Time
        if (LOG_SETTINGS.ls_print_datetime) {
            //  Get current system date time
            using namespace std::chrono;
            auto now = system_clock::now();

            //  Calculate milliseconds portion
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            char ms_char[3];
            
//  Disable the format warning around this block to stop the format error with 
//  getting milliseconds and a disposed variable.
#pragma GCC diagnostic ignored "-Wformat"
            sprintf(ms_char, "%03i", ms);
#pragma GCC diagnostic pop

            //  Convert epoch to local system clock
            auto t = system_clock::to_time_t(now);
            std::tm real_time = *std::localtime(&t);

            //  Get an ordinal suffix for the date of the month
            std::ostringstream oss;
            oss << std::put_time(&real_time, "%d");
            std::string dateMonth = dateOrdinalSuffix(std::stoi(oss.str()));

            //  Format datetime as required string format, add ms, file and line info
            output << std::put_time(&real_time, "%a %b ") << dateMonth 
                    << std::put_time(&real_time, " %Y %H:%M:%S");
            output << "." << ms_char;

            if (!LOG_SETTINGS.ls_print_fileline) {
                output << ": ";
            } else {
                output << ", ";
            }
        }

        if (LOG_SETTINGS.ls_print_fileline) {
            output << file << "(" << line << "): ";
        }

        output << "|" << LevelLabel() << "| ";

        return output.str();
    }

    /**
     * @brief   Add an ordinal string to the date of the month portion ('st',
     *          'nd', 'rd', or 'th')
     * 
     * @param date  The date of the month to append the ordinal string.
     * @return      A formatted date of the month with appended ordinal
     *              std::string.
     */
    std::string dateOrdinalSuffix(int date) {
        std::ostringstream output;
        output << date;

        int i = date % 10, j = date % 100;
        if (i == 1 && j != 11) {
            output << "st";
        } else if (i == 2 && j != 12) {
            output << "nd";
        } else if (i == 3 && j != 13) {
            output << "rd";
        } else {
            output << "th";
        }

        return output.str();
    }

    /**
     * @brief   Get the current Log instance level as a string to print.
     * 
     * @return  A std::string corresponding to current LogType level.
     */
    std::string LevelLabel() {
        switch (l_level) {
            case LT_LL_INFO:
                return "LOW-INFO";
            case LT_INFO:
                return "INFO";
            case LT_DEBUG:
                return "DEBUG";
            case LT_WARN:
                return "WARN";
            case LT_ERROR:
                return "ERROR";
            case LT_FATAL:
                return "FATAL";
            default:
                return "";
        }
    }
};

#endif //__DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_LOG_H__
