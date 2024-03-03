//
// Created by Dylan Andrew McAdam (DrengrCoder) on 28/11/22.
//  v1.0.1 TODO: UPDATE THIS
//

#ifndef __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_LOG_H__
#define __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_LOG_H__

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
#include <vector>

extern class LogSettings LOG_SETTINGS;

/**
 * @brief   Initialise the logging system. Set some values on LOG_SETTINGS and
 *          call this function.
 *
 * @param argv  The 'argv' parameter pulled in from int main.
 * @throw       runtime_error if there was an error trying to open the log file.
 *              Setting 'ls_print_to_file' to false avoids this error.
 */
#define LogInit(argv) LOG_SETTINGS.init(argv);
/**
 * @brief   Initialises the logging system for TEST CASE CODE ONLY.
 *
 * @throw       runtime_error if there was an error trying to open the log file.
 *              Setting 'ls_print_to_file' to false avoids this error.
 */
#define TestLogInit LOG_SETTINGS.tests_init();

/**
 * Close the output file stream.
 */
#define LogShutdown LOG_SETTINGS.shutdown();
/**
 * llog << "LT_LL_INFO type";
 */
#define llog Log(LogType::LT_LL_INFO, __FILE__, __LINE__)
/**
 * ilog << "LT_INFO type";
 */
#define ilog Log(LogType::LT_INFO, __FILE__, __LINE__)
/**
 * dlog << "LT_DEBUG type";
 */
#define dlog Log(LogType::LT_DEBUG, __FILE__, __LINE__)
/**
 * wlog << "LT_WARN type";
 */
#define wlog Log(LogType::LT_WARN, __FILE__, __LINE__)
/**
 * elog << "LT_ERROR type";
 */
#define elog Log(LogType::LT_ERROR, __FILE__, __LINE__)
/**
 * flog << "LT_FATAL type";
 */
#define flog Log(LogType::LT_FATAL, __FILE__, __LINE__)

/**
 * Logging levels enumerator, for label printing and including /
 * excluding certain log levels as needed through the command
 * line arguments.
 */
class LogType {
public:
    enum Value : uint8_t { LT_LL_INFO, LT_INFO, LT_DEBUG, LT_WARN, LT_ERROR, LT_FATAL };
    LogType() = default;
    constexpr LogType(Value aState) : value(aState) { }
    /**
     * Delete the default bool operator.
     */
    explicit operator bool() const = delete;
    /**
     * 'is equals' operator.
     */
    constexpr bool operator == (LogType a) const { return value == a.value; }
    /**
     * 'is not equals' operator.
     */
    constexpr bool operator != (LogType a) const { return value != a.value; }

    /**
     * Get the enumeration values in a std::vector.
     */
    static std::vector<Value> GetValues() {
        std::vector<Value> result;
        result.push_back(LT_LL_INFO);
        result.push_back(LT_INFO);
        result.push_back(LT_DEBUG);
        result.push_back(LT_WARN);
        result.push_back(LT_ERROR);
        result.push_back(LT_FATAL);
        return result;
    }

    /**
     * Convert enumeration value to exact string representation.
     */
    const char* c_str() const {
        switch (value) {
            case LT_LL_INFO:
                return "LT_LL_INFO";
            case LT_INFO:
                return "LT_INFO";
            case LT_DEBUG:
                return "LT_DEBUG";
            case LT_WARN:
                return "LT_WARN";
            case LT_ERROR:
                return "LT_ERROR";
            case LT_FATAL:
                return "LT_FATAL";
            default:
                return "";
        }
    }

    /**
     * Convert enumeration value to exact string representation.
     */
    static const char* c_str(Value a) {
        switch (a) {
            case LT_LL_INFO:
                return "LT_LL_INFO";
            case LT_INFO:
                return "LT_INFO";
            case LT_DEBUG:
                return "LT_DEBUG";
            case LT_WARN:
                return "LT_WARN";
            case LT_ERROR:
                return "LT_ERROR";
            case LT_FATAL:
                return "LT_FATAL";
            default:
                return "";
        }
    }

    /**
     * Convert enumeration value to custom string representation.
     */
    const char* custom_str() const {
        switch (value) {
            case LT_LL_INFO:
                return "low info";
            case LT_INFO:
                return "info";
            case LT_DEBUG:
                return "debug";
            case LT_WARN:
                return "warn";
            case LT_ERROR:
                return "error";
            case LT_FATAL:
                return "fatal";
            default:
                return "";
        }
    }

    /**
     * Convert enumeration value to custom string representation.
     */
    static const char* custom_str(Value a) {
        switch (a) {
            case LT_LL_INFO:
                return "low info";
            case LT_INFO:
                return "info";
            case LT_DEBUG:
                return "debug";
            case LT_WARN:
                return "warn";
            case LT_ERROR:
                return "error";
            case LT_FATAL:
                return "fatal";
            default:
                return "";
        }
    }

    /**
     * Convert enumeration value to custom upper case string representation.
     */
    const char* custom_upper_str() const {
        switch (value) {
            case LogType::LT_LL_INFO:
                return "LOW INFO";
            case LogType::LT_INFO:
                return "INFO";
            case LogType::LT_DEBUG:
                return "DEBUG";
            case LogType::LT_WARN:
                return "WARN";
            case LogType::LT_ERROR:
                return "ERROR";
            case LogType::LT_FATAL:
                return "FATAL";
            default:
                return "";
        }
    }

    /**
     * Convert enumeration value to custom upper case string representation.
     */
    static const char* custom_upper_str(Value a) {
        switch (a) {
            case LogType::LT_LL_INFO:
                return "LOW INFO";
            case LogType::LT_INFO:
                return "INFO";
            case LogType::LT_DEBUG:
                return "DEBUG";
            case LogType::LT_WARN:
                return "WARN";
            case LogType::LT_ERROR:
                return "ERROR";
            case LogType::LT_FATAL:
                return "FATAL";
            default:
                return "";
        }
    }

    /**
     * Convert custom string representation to enumeration value.
     */
    static Value custom_str_to_value(const char* a) {
        if (strcmp(a, "low info") == 0)
            return LT_LL_INFO;
        else if (strcmp(a, "info") == 0)
            return LT_INFO;
        else if (strcmp(a, "debug") == 0)
            return LT_DEBUG;
        else if (strcmp(a, "warn") == 0)
            return LT_WARN;
        else if (strcmp(a, "error") == 0)
            return LT_ERROR;
        else if (strcmp(a, "fatal") == 0)
            return LT_FATAL;
        else
            return LT_LL_INFO;
    }

private:
    Value value;
};

/**
 * Extern Log Settings class, to hold data exclusively separate from
 * the Log class as the Log class is being instantiated and destroyed
 * at every call.
 *
 * Example use:
 * <code>LogShutdown</code> should be used at the end of any
 * execution code to make sure the log file is closed. Most of the
 * time there will never be an issue because the program will release
 * its resources when execution ends, but this will further prevent
 * any issues.
 *
 * Declare #include "log.h" in any files required.
 *
 * Declare 'LogSettings LOG_SETTINGS' above int main.
 *
 * Try-catch should be declared at the top of int main in case of an
 * early error (A parser error if you use a command line arg parser),
 * but can be commented out at any time:
 *
 *          try {
 *          ... LogInit(argv);
 *          } catch (std::runtime_error err) {
 *          ... std::cout << err.what() << std::endl;
 *          ... exit(0);
 *          } catch (std::exception e) {
 *          ... std::cout << "Program is unable to start: " << e.what() << std::endl;
 *          ... exit(0);
 *          }
 *
 * Optionally declare LOG_SETTINGS with some specific settings before
 * any main program code executes, as well as or instead of the previous
 * declaration (This gives you the option to specify some settings found
 * in a custom command line parser, see full list of public variables for
 * options):
 *
 * ... LOG_SETTINGS.ls_selected_level = LT_LL_INFO;
 * ... LOG_SETTINGS.ls_use_working_dir = true;
 * ... LogInit(argv);
 *
 * Use the log macro's at the top of this file to log unique log levels,
 * see the full list of log levels above (Setting a different
 * '.ls_selected_level' will print those types of logs and above, where
 * LT_LL_INFO will log everything, and LT_FATAL will only log fatal
 * error messages).
 */
class LogSettings {
public:
    /**
     * The log level. Denoting this determines what is printed out to
     * std::cout, or a log file if selected. By default, LT_LL_INFO for all
     * log lines.
     */
    LogType::Value ls_selected_level = LogType::LT_LL_INFO;

    /**
     * True if you want to log to the working directory, false
     * otherwise. By default, False and logs to binary directory.
     */
    bool ls_use_working_dir = false;

    /**
     * True if you want to overwrite the existing file, false
     * otherwise. By default, False and will append date
     * and time information to the end of the file name
     * when it creates every new file.
     */
    bool ls_overwrite = false;

    /**
     * True if you want to log to a file, false otherwise. By default,
     * False and will not log to a file.
     */
    bool ls_print_to_file = false;

    /**
     * True if you want to log to std::cout, false otherwise. By
     * default, True and will log to std::cout.
     */
    bool ls_print_to_debug = true;

    /**
     * True if you want to log the system date and time, false
     * otherwise. (System date and time should always be
     * UTC, but this will print the current system clock). By
     * default, True and will log the system date and time with
     * every log line.
     */
    bool ls_print_datetime = true;

    /**
     * True if you want to log the file name and line number this log
     * line came from, false otherwise. By default, True and will log
     * file name and line number.
     */
    bool ls_print_fileline = true;

    /**
     * The output file stream. Holds the reference to the file to log to.
     */
    std::ofstream ls_ofs;

//  Disable the format warning and unused variable warning around these 
//  functions to stop the format error with getting milliseconds and a
//  disposed variable.
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wunused-variable"

    void init(char** argv) {
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

    void tests_init() {
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
     * Close the output file stream.
     */
    void shutdown() {
        ls_ofs.close();
    }

private:

    /**
     * The name of the file to log to, standard practice is the
     * program / app name. Automatically set based on the
     * input command arguments, where the first argument
     * always contains the name of the binary executable.
     */
    std::string ls_file_name;

    /**
     * The working directory path. Calculated when 'LogInit' is called.
     */
    std::string ls_working_dir;

    /**
     * The launch directory path. Calculated when 'LogInit' is called.
     */
    std::string ls_launch_dir;

};

/**
 * Custom Logger class. Each time one of the macro's is used it
 * creates a new instance of the logger class and appends all
 * data to the string stream buffer. The destructor is called when
 * the semi-colon is reached on each log line, which flushes the
 * buffer to the std::cout console and an output file stream if one
 * was selected. Only log lines of the specified LogType and above
 * are printed.
 *
 * Macro's in numeric order:
 *      llog << "LT_LL_INFO type";
 *      ilog << "LT_INFO type";
 *      dlog << "LT_DEBUG type";
 *      wlog << "LT_WARN type";
 *      elog << "LT_ERROR type";
 *      flog << "LT_FATAL type";
 */
class Log {
public:

    /**
     * Initialise a new Log instance to log some data.
     */
    Log(LogType::Value type, const std::string& file, const int line) {
        if (type >= LOG_SETTINGS.ls_selected_level) {
            l_level = type;
            operator<<(prefix(file, line));
        }
    }

    /**
     * Destroy the Log instance using std::endl on the output string
     * stream to flush the buffer to the std::cout console and to the
     * output file stream if one was selected.
     */
    ~Log() {
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
     * @brief   Insertion operator on custom Log class to append data
     *          to the current Log instance.
     *
     * @tparam T
     * @param t
     * @return      This Log instance.
     */
    template<typename T>
    Log& operator<<(T t) {
        if (l_level >= LOG_SETTINGS.ls_selected_level) {
            l__SS << t;
        }
        return *this;
    }

private:
    /**
     * The output string stream buffer.
     */
    std::ostringstream l__SS;

    /**
     * This Log instance's LogType level.
     */
    LogType::Value l_level = LogType::LT_LL_INFO;

    /**
     * Prefix the Date and Time as well as file name and line number
     * this log line was called.
     */
    std::string prefix(const std::string& file, const int line) {
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
     * Add an ordinal string to the DATE of the month portion ('st',
     * 'nd', 'rd', or 'th')
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
     * Get the current Log instance level as a string to print.
     */
    std::string LevelLabel() {
        return LogType::custom_upper_str(l_level);
    }
};

#endif //__DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_LOG_H__
