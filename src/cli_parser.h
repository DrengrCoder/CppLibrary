//
// Created by Dylan Andrew McAdam on 23/05/23.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CLI_PARSER_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CLI_PARSER_H__

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

#include "log.h"
#include "cpp_utilities.h"

/**
 * @brief   The Parser Option class is used to quickly define CLI argument input
 *          with strict data validation and type checking when CLI arguments are
 *          parsed into the program. This also assists in generating help
 *          information with the help flags are found. 
 * 
 * @paragraph   There are 4 types of arguments that can be found in the CLI args
 *              list:
 *                  Flag-type       (Optional flags, they exist or they don't, 
 *                                  used to define 'true' or 'false' parameters).
 *                  Flag-data       (Optional flags, and optionally append data)
 *                  Flag-data-req   (Optional flags, but data must follow them 
 *                                  because they have no default)
 *                  Req-data        (Required flags, and data must follow them)
 */
class ParserOption {
private:
    /**
     * @brief
     * The list of tags this Parser Option can be identified with.
     */
    std::vector<std::string> _tags;

    /**
     * @brief   The name of the value expected to follow a Parser Option tag
     *          argument. If this variable has data, this Parser Option is made
     *          mandatory, unless a Default Value is specified in the ctor. The
     *          value name can be supplied with a single string, or it takes the
     *          first option in a list of choices.
     */
    std::string _valueName = "";

    /**
     * @brief   The default value for this Parser Option if a value name has
     *          been specified. If this variable has data, this Parser Option is
     *          not considered to be mandatory.
     */
    std::string _defaultValue = "";

    /**
     * @brief   The list of options that is expected to follow this Parser 
     *          Option tag argument. This is only used when the Parser Option
     *          must force a set list of choices, and the first choice in the
     *          list will set the Value Name variable.
     */
    std::vector<std::string> _choicesList;
    /**
     * @brief   True if one of the ctor's was used that specifies using a forced
     *          list of choices for the Parser Option, false otherwise. This
     *          variable gets set automatically if one of the specific ctors is
     *          used.
     */
    bool _usingChoices = false;

    /**
     * @brief   Determine if a choice is a duplicate of one that already exists
     *          on this Parser Option. This function is only called in the
     *          ctor's when initialising this object instance.
     * 
     * @param choice    The input string to check against the other existing
     *                  choices.
     * @return true     if the input string is a duplicate,
     * @return false    otherwise.
     */
    bool ChoiceIsDuplicate(std::string choice) {
        for (std::string c : _choicesList) {
            if (c == choice) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief   Determine if a tag is a duplicate of one that already exists on
     *          this Parser Option. This function is only called in the ctor's
     *          when initialising this object instance.
     * 
     * @param tag       The input string to check against the other existing 
     *                  tags.
     * @return true     if the input string is a duplicate,
     * @return false    otherwise.
     */
    bool TagIsDuplicate(std::string tag) {
        for (std::string t : _tags) {
            if (t == tag) {
                return true;
            }
        }
        return false;
    }

public:
    /**
     * @brief   The description for this Parser Option. This is also printed out
     *          when generating the help information.
     */
    std::string _description = "Default Description";

    /**
     * @brief   A flag to show whether this Parser Option is required. This is
     *          initialised in the ctor's initialisation list and is set to TRUE
     *          if a Value Name or Choice's list was set but a Default Value was
     *          not, otherwise this variable will be false.
     */
    const bool _isRequired;

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This will default the 'is mandatory' field to
     *          false.
     * 
     * @param tag               The string argument to identify this Parser 
     *                          Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::string tag, std::string descr, 
            std::initializer_list<std::string> choices,
            std::string defaultValue = "")
        : _isRequired(false)
    {
        new(this) ParserOption(tag, descr, choices, _isRequired, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This ctor takes in an 'is mandatory' bool flag to
     *          set the local bool flag in this object.
     * 
     * @param tag               The string argument to identify this Parser 
     *                          Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::string tag, std::string descr, 
            std::initializer_list<std::string> choices, bool required,
            std::string defaultValue = "")
        : _isRequired(required)
    {
        new(this) ParserOption({tag}, descr, choices, _isRequired, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This will default the 'is mandatory' field to
     *          false.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::initializer_list<std::string> tags, std::string descr,
            std::initializer_list<std::string> choices, 
            std::string defaultValue = "")
        : _isRequired(false)
    {
        new(this) ParserOption(tags, descr, choices, _isRequired, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This ctor takes in an 'is mandatory' bool flag to
     *          set the local bool flag in this object.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::initializer_list<std::string> tags, std::string descr, 
            std::initializer_list<std::string> choices, bool required,
            std::string defaultValue = "")
        : _isRequired(required)
    {
        std::vector<std::string> tag_vector;
        for (std::string str : tags)
            tag_vector.push_back(str);
        new(this) ParserOption(tag_vector, descr, choices, _isRequired, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This will default the 'is mandatory' field to
     *          false.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::vector<std::string> tags, std::string descr, 
            std::initializer_list<std::string> choices,
            std::string defaultValue = "")
        : _isRequired(false)
    {
        new(this) ParserOption(tags, descr, choices, _isRequired, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags or choices with hyphons and duplicate tags and
     *          choices or empty strings will not be added to the lists. An
     *          object must be created with at least one or many tags and a
     *          description. This ctor takes in an 'is mandatory' bool flag to
     *          set the local bool flag in this object.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param choices           The list of choices for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags or choices were added to this Parser
     *                          Option because they did not satisfy entry
     *                          conditions. This function has been marked
     *                          'noexcept' so it cannot be caught and will
     *                          terminate the program, as this is considered to
     *                          be badly written code (Every Parser Option needs
     *                          at least 1 tag, and 1 choice where applicable).
     */
    ParserOption(std::vector<std::string> tags, std::string descr, 
            std::initializer_list<std::string> choices, bool required,
            std::string defaultValue = "") noexcept
        : _isRequired(required)
    {
        
        //  Check all the input choices...
        for (std::string choice : choices) {

            //  Get start and end of real string
            int firstNotOf = choice.find_first_not_of(' ');
            int lastNotOf = choice.find_last_not_of(' ') + 1;

            //  Make sure it is not an empty string
            if (firstNotOf > -1) {

                //  Then recreate string without leading and trailing whitespace
                choice = choice.substr(firstNotOf, lastNotOf - firstNotOf);

                //  Replace all remaining spaces with a hyphon character
                std::replace(choice.begin(), choice.end(), ' ', '-');
                
                //  Make sure this is not an existing duplicate
                if (!ChoiceIsDuplicate(choice)) {
                    //  Add the choice to the list of choices on this Option
                    _choicesList.push_back(choice);
                }
            }
        }

        //  Make sure at least one choice has been added before continuing
        if (_choicesList.size() < 1) {
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("No valid choices were added to the "
                                        "parser option, bad argument input.");
#pragma GCC diagnostic pop
        }

        //  Now check all the tags...
        for (std::string tag : tags){

            //  Get start and end of real string
            int firstNotOf = tag.find_first_not_of(' ');
            int lastNotOf = tag.find_last_not_of(' ') + 1;

            //  Make sure it is not an empty string
            if (firstNotOf > -1) {

                //  Then recreate string without leading and trailing whitespace
                tag = tag.substr(firstNotOf, lastNotOf - firstNotOf);

                //  Replace all remaining spaces with a hyphon character
                std::replace(tag.begin(), tag.end(), ' ', '-');

                //  Make sure they're not duplicates
                if (tag.length() == 1) {
                    tag = "-" + tag;
                    if (!TagIsDuplicate(tag)) {
                        _tags.push_back(tag);
                    }
                } else {
                    tag = "--" + tag;
                    if (!TagIsDuplicate(tag)) {
                        _tags.push_back(tag);
                    }
                }
            }
        }

        //  Make sure at least one tag has been added before continuing
        if (_tags.size() < 1) {
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("No valid Tags added to the parser option.");
#pragma GCC diagnostic pop
        }

        //  Set value name as the first choice
        std::string valueName = _choicesList[0];
        //  Toggle this flag with this constructor specifically
        _usingChoices = true;

        //  Remove leading and trailing white space in description before saving
        int firstNotOf = descr.find_first_not_of(' ');
        int lastNotOf = descr.find_last_not_of(' ') + 1;
        descr = firstNotOf < 0 ? "" :
                descr.substr(firstNotOf, lastNotOf - firstNotOf);
        _description = descr;

        //  Remove leading and trailing white space in value name before saving
        firstNotOf = valueName.find_first_not_of(' ');
        lastNotOf = valueName.find_last_not_of(' ') + 1;
        valueName = firstNotOf < 0 ? "" :
                    valueName.substr(firstNotOf, lastNotOf - firstNotOf);
        //  And replace remaining white space with hyphons
        std::replace(valueName.begin(), valueName.end(), ' ', '-');
        _valueName = valueName;

        //  Remove leading and trailing white space in default value before saving
        firstNotOf = defaultValue.find_first_not_of(' ');
        lastNotOf = defaultValue.find_last_not_of(' ') + 1;
        defaultValue = firstNotOf < 0 ? "" :
                       defaultValue.substr(firstNotOf, lastNotOf - firstNotOf);
        //  And replace remaining white space with hyphons
        std::replace(defaultValue.begin(), defaultValue.end(), ' ', '-');
        _defaultValue = defaultValue;
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This will
     *          default the 'is mandatory' field to false.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::string tag, std::string descr, std::string valueName = "", 
            std::string defaultValue = "") 
        : _isRequired(false)
    {
        new(this) ParserOption(tag, descr, _isRequired, valueName, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This ctor
     *          takes in an 'is mandatory' bool flag to set the local bool flag
     *          in this object.
     * 
     * @param tag               The string argument to identify this Parser 
     *                          Option.
     * @param descr             The description for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::string tag, std::string descr, bool required,
            std::string valueName, std::string defaultValue = "")
        : _isRequired(required)
    {
        new(this) ParserOption({tag}, descr, _isRequired, valueName, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This will
     *          default the 'is mandatory' field to false.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::initializer_list<std::string> tags, std::string descr, 
            std::string valueName = "", std::string defaultValue = "")
        : _isRequired(false)
    {
        new(this) ParserOption(tags, descr, _isRequired, valueName, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This ctor
     *          takes in an 'is mandatory' bool flag to set the local bool flag
     *          in this object.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::initializer_list<std::string> tags, std::string descr,
            bool required, std::string valueName, std::string defaultValue = "")
        : _isRequired(required)
    {
        std::vector <std::string> tag_vector;
        for (std::string s : tags)
            tag_vector.push_back(s);
        new(this) ParserOption(tag_vector, descr, _isRequired, valueName, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This will
     *          default the 'is mandatory' field to false.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::vector<std::string> tags, std::string descr, 
            std::string valueName = "", std::string defaultValue = "")
        : _isRequired(false)
    {
        new (this) ParserOption(tags, descr, _isRequired, valueName, defaultValue);
    }

    /**
     * @brief   Construct a new Parser Option object. This object should be
     *          added to the Parser class instance so the Parser can perform
     *          strict checks against and for this Parser Option argument tags
     *          and argument data. Removes leading and trailing white space
     *          characters from all strings, replaces any white space characters
     *          remaining in tags with hyphons and duplicate tags or empty
     *          strings will not be added to the list. An object must be created
     *          with at least one or many tags and a description. This ctor
     *          takes in an 'is mandatory' bool flag to set the local bool flag
     *          in this object.
     * 
     * @param tags              The list of string arguments to identify this
     *                          Parser Option.
     * @param descr             The description for this Parser Option.
     * @param required          True if this argument must exist in the list of
     *                          args, false otherwise.
     * @param valueName         The value name for argument following this
     *                          Option's tags, acting as a brief description for
     *                          the data. If you want to specify this parameter
     *                          in the constructor with a string literal,
     *                          surround the string in brackets with
     *                          'std::string()'.
     * @param defaultValue      The default value for this Parser Option. If you
     *                          want to specify this parameter in the
     *                          constructor with a string literal, surround the
     *                          string in brackets with 'std::string()'.
     * @throw InvalidArgument   If no tags were added to this Parser Option
     *                          because they did not satisfy entry conditions.
     *                          This function has been marked 'noexcept' so it
     *                          cannot be caught and will terminate the program,
     *                          as this is considered to be badly written code
     *                          (Every Parser Option needs at least 1 tag, and 1
     *                          choice where applicable).
     */
    ParserOption(std::vector<std::string> tags, std::string descr,
            bool required, std::string valueName, 
            std::string defaultValue = "") noexcept
        : _isRequired(required)
    {
        //  Check all tags...
        for (std::string tag : tags){

            //  Get start and end of real string
            int firstNotOf = tag.find_first_not_of(' ');
            int lastNotOf = tag.find_last_not_of(' ') + 1;

            //  Make sure it is not an empty string
            if (firstNotOf > -1){

                //  Then recreate string without leading and trailing whitespace
                tag = tag.substr(firstNotOf, lastNotOf - firstNotOf);

                //  Replace all remaining spaces with a hyphon character
                std::replace(tag.begin(), tag.end(), ' ', '-');

                //  Make sure they're not duplicates
                if (tag.length() == 1) {
                    tag = "-" + tag;
                    if (!TagIsDuplicate(tag)) {
                        _tags.push_back(tag);
                    }
                } else {
                    tag = "--" + tag;
                    if (!TagIsDuplicate(tag)) {
                        _tags.push_back(tag);
                    }
                }
            }

        }

        //  Make sure at least one tag has been added before continuing
        if (_tags.size() < 1) {
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("No valid Tags added to the parser option.");
#pragma GCC diagnostic pop
        }

        //  Remove leading and trailing white space in description before saving
        int firstNotOf = descr.find_first_not_of(' ');
        int lastNotOf = descr.find_last_not_of(' ') + 1;
        descr = firstNotOf < 0 ? "" :
                descr.substr(firstNotOf, lastNotOf - firstNotOf);
        _description = descr;

        //  Remove leading and trailing white space in value name before saving
        firstNotOf = valueName.find_first_not_of(' ');
        lastNotOf = valueName.find_last_not_of(' ') + 1;
        valueName = firstNotOf < 0 ? "" :
                    valueName.substr(firstNotOf, lastNotOf - firstNotOf);
        //  And replace remaining white space with hyphons
        std::replace(valueName.begin(), valueName.end(), ' ', '-');
        _valueName = valueName;

        //  Remove leading and trailing white space in default value before saving
        firstNotOf = defaultValue.find_first_not_of(' ');
        lastNotOf = defaultValue.find_last_not_of(' ') + 1;
        defaultValue = firstNotOf < 0 ? "" :
                       defaultValue.substr(firstNotOf, lastNotOf - firstNotOf);
        //  And replace remaining white space with hyphons
        std::replace(defaultValue.begin(), defaultValue.end(), ' ', '-');
        _defaultValue = defaultValue;
    }

    /**
     * @brief   Get the Value Name. This parameter is optional and may be empty.
     * 
     * @return  A std::string for the Value Name variable.
     */
    std::string GetValueName() { return _valueName; }

    /**
     * @brief   Get the Default Value. This parameter is optional and may be
     *          empty.
     * 
     * @return  A std::string for the Default Value variable.
     */
    std::string GetDefaultValue() { return _defaultValue; }

    /**
     * @brief   Get the vector of Tags. There will always be at least 1 tag.
     * 
     * @return  A std::vector<std::string> containing all the tags added to this
     *          Parser Option.
     */
    std::vector<std::string> GetTags() { return _tags; }

    /**
     * @brief   Get the vector of Choices. There should be at least 1 choice if
     *          choices are being used.
     * 
     * @return  A std::vector<std::string> containing all the choices added to
     *          this Parser Option.
     */
    std::vector<std::string> GetChoices() { return _choicesList; }

    /**
     * @brief   Return whether this Parser Option is using a list of choices.
     *          This variable is set in the ctor's if the ctor is used that
     *          allows you to define a list of choices with a brace-initialised
     *          list.
     * 
     * @return true     if there are a list of choices on this object,
     * @return false    otherwise.
     */
    bool IsUsingChoices() { return _usingChoices; }

    /**
     * @brief   Check if a given string is a valid choice from the list of
     *          choices on this Parser Option.
     * 
     * @param value     The string value to check for.
     * @return true     if 'value' exists in the list of choices,
     * @return false    otherwise.
     */
    bool ValueIsValidChoice(std::string value) {
        for (std::string choice : _choicesList) {
            if (choice == value) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief   Print the list of choices on this Parser Option. This function
     *          is triggered when the help flag follows a Parser Option tag for
     *          this option.
     */
    void PrintOptionChoiceInfo() {
        std::stringstream baseOss;
        baseOss << "Help Information for Argument: \"" << _tags[0] << "\"...\n";
        for (std::string choice : _choicesList) {
            baseOss << "\n\t" << _tags[0] << " " << choice;
        }
        std::cout << baseOss.str() << "\n" << std::endl;
    }

    /**
     * @brief   Override the equals sign operator for this object. Two objects
     *          of this type are said to be "equal" when all variable fields are
     *          identical across both of them.
     * 
     * @param p1        The first object to compare, left side of the statement. 
     * @param p2        The second object to compare, right side of the statement.
     * @return true     if both objects are said to be equal, defined in this
     *                  function,
     * @return false    otherwise.
     */
    friend bool operator==(const ParserOption &p1, const ParserOption &p2) {
        return p1._tags == p2._tags &&
               p1._description == p2._description &&
               p1._valueName == p2._valueName &&
               p1._defaultValue == p2._defaultValue &&
               p1._choicesList == p2._choicesList;
    }

};

/**
 * @brief   The Parser class is used to hold a list of Parser Option objects so
 *          the CLI arguments can be parsed quickly and easily. This class will
 *          force strict checks and validation on arg data based on the Parser
 *          Options added to it. The Help and Version flags can be easily added
 *          with built-in functions and do not require you to create your own.
 */
class Parser{
private:
    /**
     * @brief The list of arguments can has been pulled in from the command line.
     */
    std::vector<std::string> _args;
    /**
     * @brief The program version number, stored locally as a string.
     */
    std::string _version = "-1.-1.-1";

    /**
     * @brief   The list of Parser Options added to be Parsed by this class
     *          instance.
     */
    std::vector<ParserOption> _options;
    
    /**
     * @brief   Bool flag to denote if the help option has been added. The help
     *          argument flag should be added by the built-in function in this
     *          class.
     */
    bool _usingHelpOption = false;
    /**
     * @brief   Bool flag to denote if the version option has been added. The
     *          version argument flag should be added by the built-in function
     *          in this class.
     */
    bool _usingVersionOption = false;

    /**
     * @brief   Return whether this option has already been added to the this
     *          Parser instance. Called in the 'AddOptions' function to prevent
     *          duplicate options from being added.
     * 
     * @param opt       The Parser Option to check for.
     * @return true     if the Parser Option already exists in this Parser
     *                  instance,
     * @return false    otherwise.
     */
    bool ContainsOption(ParserOption opt) {
        for (ParserOption o : _options) {
            if (opt == o) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief   Calculate the exact number of tab spaces (4 white space char's)
     *          required to reach the required width of the Parser Option flag
     *          portion of the Help information print out. This is called in the
     *          'PrintHelpInfo' function and is used to help format the text
     *          printed out by the help option flag.
     * 
     * @return int  The number of tabs required to format the help info properly,
     *              based on the length the flags of every Parser Option
     *              requires.
     */
    int CalculateMaximumTabCount(){
        //  init counter
        int counter = -1;

        //  Loop all options to find the longest one...
        for (ParserOption option : _options){
            //  init string
            std::stringstream oss;
            //  Add the first tab that will be at the start of every string entry
            oss << "    ";

            //  Check all option tags
#pragma GCC diagnostic ignored "-Wsign-compare"
            for (int i = 0; i < option.GetTags().size(); i++) {
#pragma GCC diagnostic pop
                //  If this is not the first tag, add a comma and a space
                if (i > 0){
                    oss << ", ";
                }
                //  Add the tag string
                oss << option.GetTags()[i];
            }

            //  Add the value name if there is one
            if (option.GetValueName() != ""){
                oss << " <" << option.GetValueName() << ">";
            }

            //  Get the length of this string and save it if it's the longest
            int len = oss.str().length();
            if (len > counter){
                counter = len;
            }
        }

        // - Take the longest string of all combined argument flags from every
        //      parser option
        // - Add 1 to this number, in the event a option is 1 space short of
        //      being evenly divisible by 4
        // - Divide the result by 4, as 4 is the number of spaces for a tab key
        // - Add 1 to the final result to make sure we are moving at least 1 tab
        //      out from the end of the string
        // - We added 1 to the counter prior to dividing by 4 in the event the
        //      resulting number of tabs would have made it so only 1 space was
        //      appended to the end of the tags string before writing the 
        //      description.
        return ((counter + 1) / 4) + 1;
    }

    /**
     * @brief   Print the version information to std::cout/
     * 
     */
    void PrintVersionInfo() {
        std::cout << this->_name 
            << " Version: " << this->_version << std::endl;
    }

    /**
     * @brief   Formats text to be printed to the console. Loops through all
     *          Parser Option objects that were added to this Parser before
     *          calling 'Process' and formats the help info text with the
     *          correct number of spaces and indentations for the console.
     */
    void PrintHelpInfo(){
        //  The maximum width of the screen for a new line should be entered
        const int maxLineWidth = 80;

        //  Hold a local reference to the maximum number of tabs required to
        //  exceed the end of the string of combined tags for every option
        const int maxTabs = CalculateMaximumTabCount();
        //  Define the number of spaces required for every string width before
        //  the description is written
        const int maxTagWidth = (maxTabs * 4);

        //  Define the string for required arguments to print in the usage info
        std::stringstream ossRequiredOptions;
        for (ParserOption option : _options){
            if (option._isRequired) {
                ossRequiredOptions << " " << option.GetTags()[0] 
                        << " <" << option.GetValueName() << ">";
            }
        }

        //  Define the string for the list of possible options and arguments
        std::stringstream ossOptionInformation;
        for (ParserOption option : _options){

            //  Define the string for this specific option entry
            std::stringstream ossOptionEntry;
            //  Add the initial tab space
            ossOptionEntry << "    ";

#pragma GCC diagnostic ignored "-Wsign-compare"
            for (int i = 0; i < option.GetTags().size(); i++){
#pragma GCC diagnostic pop
                //  If this is not the first tag, add a comma
                if (i > 0){
                    ossOptionEntry << ", ";
                }
                //  Add the tag string
                ossOptionEntry << option.GetTags()[i];
            }

            //  If there is a value name, as this to the end of the final tag
            if (option.GetValueName() != ""){
                ossOptionEntry << " <" << option.GetValueName() << ">";
            }
            
            //  Pad the remaining space expected
            int remainingSpace = maxTagWidth - ossOptionEntry.str().length();
            do {
                ossOptionEntry << ' ';
            } while (--remainingSpace > 0);

            //  If the option is required, specify this before the description
            if (option._isRequired){
                ossOptionEntry << "(REQUIRED) ";
            }
            //  Add the description
            ossOptionEntry << option._description;
            //  If this option has a default value, append it to the description
            if (option.GetDefaultValue() != ""){
                ossOptionEntry << " [Default: " 
                        << option.GetDefaultValue() << "].";
            }

            //  First check if this entry is over the screen width threshold
            if (ossOptionEntry.str().length() > maxLineWidth){
                //  Get the current entry as a plain string
                std::string currentOptionEntry = ossOptionEntry.str();
                //  Get ready to construct a new string
                std::stringstream ossRestructuredEntry;

                //  Split the current string out with space as a delimiter
                std::stringstream currOptSs(currentOptionEntry);
                std::vector<std::string> currOptArray;
                std::string currOptStr;
                while (std::getline(currOptSs, currOptStr, ' ')){
                    currOptArray.push_back(currOptStr);
                }

                //  Go to add the string array entries to the new string stream
                int timesReachedLength = 1;
                for (std::string str : currOptArray){

                    //  If this array entry length plus existing string length 
                    //  is greater than the maximum width allowed...

#pragma GCC diagnostic ignored "-Wsign-compare"

                    if ((ossRestructuredEntry.str().length() + str.length()) > 
                            (maxLineWidth * timesReachedLength)){

#pragma GCC diagnostic pop

                        timesReachedLength++;
                        ossRestructuredEntry << '\n';

                        for (int i = 0; i < maxTagWidth; i++){
                            ossRestructuredEntry << ' ';
                        }

                        ossRestructuredEntry << str << ' ';
                    } else {
                        //  Otherwise just add it with a space appended
                        ossRestructuredEntry << str << ' ';
                    }
                }

                ossOptionInformation << ossRestructuredEntry.str();
            } else {
                //  If not, append it to the option information string stream
                ossOptionInformation << ossOptionEntry.str();
            }

            ossOptionInformation << '\n';
        }

        std::stringstream helpOutput;
        helpOutput << this->_name << " Help Information...\n\n";
        helpOutput << "Usage: " << this->_args[0] 
                << " [options]" << ossRequiredOptions.str() << '\n';
        helpOutput << "Description: " << this->_description << "\n\n";
        helpOutput << "Options:\n" << ossOptionInformation.str() << std::endl;
        
        flog << helpOutput.str();
    }

    /**
     * @brief   Returns a Parser Option object based on the string tag argument
     *          found in the command line args.
     * 
     * @param tag   The std::string tag to look for.
     * @return      A ParserOption object that has been identified with the
     *              std::string tag.
     * @throw       InvalidArgument if a Parser Option using the specified tag
     *              could not be found. This is used for catching errors and
     *              escaping portions of a function loop.
     */
    ParserOption GetOptionByTag(std::string tag)
    {
        for (ParserOption option : _options){
            for (std::string t : option.GetTags()){
                if (t == tag){ 
                    return option;
                }
            }
        }
        throw std::invalid_argument("INVALID TAG FOUND");
    }

public:
    /**
     * @brief   The program name. This can be set manually or pulled in
     *          automatically based on the execution path variable, which is the
     *          first argument in any command line argument list.
     */
    std::string _name = "Default Name.";
    /**
     * @brief   The description of the program. This can be adjusted after
     *          calling the contructor to set it, and should be a general brief
     *          description of what the program is doing and how.
     */
    std::string _description = "Default Description.";

    /**
     * @brief   Construct a new Parser object. This parser instance can be used
     *          to validate command line argument input by receiving the number
     *          of arguments passed in via the command line and the char pointer
     *          array of the arguments themselves. This constructor will
     *          automatically create a program name and assign it to the 'name'
     *          variable, which is based on the first argument from 'argv'.
     * 
     * @param argc      The number of arguments. 'argc' is pulled in from int
     *                  main.
     * @param argv      The char pointer array of arguments. 'argv' is pulled in
     *                  from int main.
     * @param descr     A description of the program.
     */
    Parser(int &argc, char **argv, std::string descr, std::string vers = "0.0.0.0"){
        std::string firstArg = argv[0];
        std::string exeName = firstArg
            .substr(firstArg.find_last_of('/') + 1, firstArg.size());
        new(this) Parser(argc, argv, exeName, descr, vers);
    }

    /**
     * @brief   Construct a new Parser object. This parser instance can be used
     *          to validate command line argument input by receiving the number
     *          of arguments passed in via the command line and the char pointer
     *          array of the arguments themselves. This constructor will remove
     *          leading and trailing white space from the name and the
     *          description, store the version number locally and add all the
     *          arguments from the 'argv' variable in the local list containing
     *          the args as a std::string vector.
     * 
     * @param argc      The number of arguments. 'argc' is pulled in from int
     *                  main.
     * @param argv      The char pointer array of arguments. 'argv' is pulled in
     *                  from int main.
     * @param name      A name for the program.
     * @param descr     A description of the program.
     * @throw           InvalidArgument if the name string is empty.
     */
    Parser(int &argc, char **argv, std::string name, std::string descr, std::string vers) noexcept
    {

        //  Remove leading and trailing whitespace
        int firstNotOf = name.find_first_not_of(' ');
        int lastNotOf = name.find_last_not_of(' ') + 1;
        
        //  Make sure it is not an empty string
        if (firstNotOf < 0){
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("Name cannot be an empty string.");
#pragma GCC diagnostic pop
        }

        this->_name = firstNotOf < 0 ? "" :
            name.substr(firstNotOf, lastNotOf - firstNotOf);

        //  Remove leading and trailing whitespace
        firstNotOf = descr.find_first_not_of(' ');
        lastNotOf = descr.find_last_not_of(' ') + 1;
        this->_description = firstNotOf < 0 ? "" :
            descr.substr(firstNotOf, lastNotOf - firstNotOf);

        //  Get the version information and store it locally
        _version = vers;

        //  Cannot use a foreach loop on argv due to the nature of its type
        for (int i = 0; i < argc; i++) {
            this->_args.push_back(std::string(argv[i]));
        }
    }

    /**
     * @brief Add a predefined Parser Option for the help flag.
     */
    void AddHelpOption() {
        ParserOption po({"h", "help"}, "Show this help.");
        if (int r = this->AddOptions({po}) != 0) {
            elog << "Error adding help option: " << r << ".";
        } else {
            _usingHelpOption = true;
        }
    }

    /**
     * @brief Add a predefined Parser Option for the version flag.
     */
    void AddVersionOption() {
        ParserOption po({"v", "version"}, "Show current version.");
        if (int r = this->AddOptions({po}) != 0) {
            elog << "Error adding version option: " << r << ".";
        } else{
            _usingVersionOption = true;
        }
    }

    /**
     * @brief   Add a list of Parser Options to this Parser class instance.
     *          Accepts a brace-initiailised list.
     * 
     * @param options   The list of Parser Option objects to add.
     * @return          An integer that represents the number of Parser Options
     *                  that failed to be added, or an error code where
     *                  appropriate.
     */
    int AddOptions(std::initializer_list<ParserOption> options) {
        int result = 0;
        for (ParserOption po : options) {
            //  Check if it already exists
            if (!this->ContainsOption(po)) {
                //  Add it if not
                _options.push_back(po);
                if (!this->ContainsOption(po)){
                    elog << "Failed to add new option.";
                    result++;
                }
            } else {
                wlog << "Option \"" << po.GetTags()[0] << "\" already exists.";
            }
        }
        return result;
    }

    /**
     * @brief   Returns whether the Parser Option supplied as a parameter has a
     *          tag that has been found in the list of command line arguments.
     * 
     * @param option    The Parser Option to check the arguments against.
     * @return true     if one of the Parser Option's tags has been found in the
     *                  list of args,
     * @return false    otherwise.
     * @throw           InvalidArgument if the Parser Option was not added to
     *                  this Parser instance. This function has been marked
     *                  'noexcept' so it cannot be caught and will terminate the
     *                  program, as this is considered to be badly written code
     *                  (You should not be checking for Parser Option's that
     *                  were not added to the Parser).
     */
    bool IsSet(ParserOption option) noexcept
    {
        bool optionExists = false;
        for (ParserOption opt : _options) {
            if (option == opt) {
                optionExists = true;
                break;
            }
        }
        if (!optionExists) {
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("Bad use of function: Option was not "
                                        "found in parser, every Parser Option "
                                        "needs to be added to the Parser for "
                                        "processing before they can be used.");
#pragma GCC diagnostic pop
        }

#pragma GCC diagnostic ignored "-Wsign-compare"
        for (int i = 1; i < _args.size(); i++){
#pragma GCC diagnostic pop
            for (std::string tag : option.GetTags()){
                if (tag == _args[i]){
                    return true;
                }
            }
        }

        return false;
    }

    std::string GetValue(ParserOption option) noexcept
    {
        bool optionExists = false;
        for (ParserOption opt : _options) {
            if (option == opt) {
                optionExists = true;
                break;
            }
        }
        if (!optionExists) {
#pragma GCC diagnostic ignored "-Wterminate"
            throw std::invalid_argument("Bad use of function: Option was not "
                                        "found in parser, every Parser Option "
                                        "needs to be added to the Parser for "
                                        "processing before they can be used.");
#pragma GCC diagnostic pop
        }

        //  Go through all the command line arguments
#pragma GCC diagnostic ignored "-Wsign-compare"
        for (int i = 1; i < _args.size(); i++){
#pragma GCC diagnostic pop

            //  Get this current argument
            std::string this_arg = _args[i];

            //  Check all the tags on this option
            for (std::string tag : option.GetTags()){
                //  If we found a tag matching this option
                if (tag == this_arg){
                    //  Check if we are at the end of the arguments list
#pragma GCC diagnostic ignored "-Wsign-compare"
                    if (i + 1 < _args.size()){
#pragma GCC diagnostic pop
                        //  return the data after this option's tag
                        //      This must be data because 'Process' would have
                        //      failed otherwise.
                        return _args[i + 1];
                    } else {
                        //  If this is the last argument, attempt to retrieve a 
                        //  default value or throw invalid argument exception
#pragma GCC diagnostic ignored "-Wterminate"
                        std::string defaultValue = option.GetDefaultValue();
                        return defaultValue != "" ? defaultValue :
                                throw std::invalid_argument("");
#pragma GCC diagnostic pop
                    }
                }
            }
        }

        //  This line is reached if no arguments matched this options tags
        //  and will attempt to return the default value if there is one, or
        //  throw invalid argument exception
#pragma GCC diagnostic ignored "-Wterminate"
        std::string defaultValue = option.GetDefaultValue();
        return defaultValue != "" ? defaultValue :
                throw std::invalid_argument("No default value found for this option.");
#pragma GCC diagnostic pop
    }

    /**
     * @brief   Single function to handle processing of all command line
     *          arguments passed in to this Parser class instance. This function
     *          will parse all the args and handle error output and enforce
     *          strict checks against the argument and accompanying data where
     *          applicable.
     * 
     * @return true     if args were successfully parsed,
     * @return false    otherwise.
     */
    bool Process(){

        //  Check for help and version flags first...
#pragma GCC diagnostic ignored "-Wsign-compare"
        for (int i = 1; i < _args.size(); i++){
#pragma GCC diagnostic pop

            if (_usingHelpOption && (_args[i] == "-h" || _args[i] == "--help")){

                //  If we found the help option but it wasn't the first argument...
                if (i > 1){

                    //  Get the tag preceeding the help tag
                    std::string tag = _args[i - 1];

                    try {
                        //  Identify which parser option it relates to
                        ParserOption option = GetOptionByTag(tag);
                        //  Print help info if any
                        if (option.IsUsingChoices())
                            option.PrintOptionChoiceInfo();
                        else
                            throw std::invalid_argument("No help information for tag");
                    } catch (std::invalid_argument &err){
                        //  Log exceptions thrown
                        elog << err.what() << ": \"" << tag << "\".";
                    }
                }
                PrintHelpInfo();
                exit(0);
            }
            if (_usingVersionOption && (_args[i] == "-v" || _args[i] == "--version")){
                PrintVersionInfo();
                exit(0);
            }
        }

        //  Define the string streams to store error output in...
        //      For required arguments not found
        std::stringstream ossMissingArgErr;
        //      For required arguments where no data was found
        std::stringstream ossMissingDatErr;
        //      For undefined arguments found
        std::stringstream ossInvalidArgErr;
        //      For no data following arguments requiring it
        std::stringstream ossBadArgDataErr;

        //  Look for required argument existence
        for (ParserOption option : _options){
            if (option._isRequired && !this->IsSet(option)){
                ossMissingArgErr << "\tRequired argument missing: \""
                                << option.GetTags()[0] << " <" 
                                << option.GetValueName() << ">\"." << std::endl;
            }
        }
        
        //  Loop all the arguments...
#pragma GCC diagnostic ignored "-Wsign-compare"
        for (int i = 1; i < _args.size(); i++){
#pragma GCC diagnostic pop

            //  Get this argument
            std::string arg = _args[i];

            try {

                //  First make sure the argument is a valid parser option
                //      This functions throws exception
                ParserOption this_option = GetOptionByTag(arg);

                //  Get the next argument in the list
                std::string next_arg = "";
#pragma GCC diagnostic ignored "-Wsign-compare"
                if (i + 1 < _args.size()){
#pragma GCC diagnostic pop
                    next_arg = _args[i + 1];
                }
                
                //  If the argument is marked as required, we're expecting data 
                //  after it, otherwise check if it's using choices to check
                //  those instead
                if (this_option._isRequired){

                    //  There is an error if:
                    //  - The next arg starts with a hyphon (means it was a 
                    //      parser option tag)
                    //  - This option is using choices and the next arg is an 
                    //      invalid choice
                    //  - The next arg is an empty string (means this option's 
                    //      argument was the last in the array)

                    if (next_arg.rfind('-', 0) == 0 || 
                            (this_option.IsUsingChoices() && 
                            !this_option.ValueIsValidChoice(next_arg))){
                        ossBadArgDataErr << "\tInvalid data: \"" << next_arg
                                << "\", followed the REQUIRED argument tag: \""
                                << arg << "\"." << std::endl;
                    } else if (next_arg == ""){
                        ossMissingDatErr << "\tData missing for argument: \""
                                << arg << "\"." << std::endl;
                    }
                } else if (this_option.IsUsingChoices()){
                    //  If the next argument does not begin with a hyphon...
                    if (next_arg.rfind('-', 0) != 0){
                        //  Then it is not a parser option, meaning data is
                        //  going to follow this parser option choice. Make
                        //  sure the data is a valid choice for this option.
                        if (!this_option.ValueIsValidChoice(next_arg)){
                            ossBadArgDataErr << "\tInvalid data: \"" << next_arg 
                                << "\", followed the argument tag: \"" << arg
                                << "\"." << std::endl;
                        }
                    }
                }

            }
            catch (std::invalid_argument &err) {
                //  If retrieving the parser option by tag failed, it could be
                //  because it was argument data for the previous tag...

                //  If i == 1 then it's definitely an error
                bool invArg = false;
                if (i == 1){
                    invArg = true;
                } else {
                    //  Otherwise get the argument previous to this one
                    std::string prev_arg = _args[i - 1];
                    try {
                        //  Make sure this is actually a valid argument as it
                        //  could be data for the previous argument to that one
                        ParserOption prev_option = GetOptionByTag(prev_arg);
                        //  If it was a valid parser option, check if it was not
                        //  expecting data after it
                        if (prev_option.GetValueName() == ""){
                            invArg = true;
                        }
                    }
                    catch (std::invalid_argument &err){    
                        invArg = true;
                    }
                }

                if (invArg){
                    ossInvalidArgErr << "\tArgument was not added to parser as an "
                            << "option: \"" << arg << "\"." << std::endl;
                }
            }
        }

        if (ossInvalidArgErr.str() != "" || ossMissingArgErr.str() != "" ||
            ossBadArgDataErr.str() != "" || ossMissingDatErr.str() != "") {
            std::stringstream output;
            output << "\n########################################\n";
            output << "Error processing command line arguments:\n";
            output << "########################################\n\n";

            int count = CountOccurrences(ossInvalidArgErr.str(), "\n");
            output << "Unidentified arguments (" << count << "):\n";
            output << ossInvalidArgErr.str() << "\n";

            count = CountOccurrences(ossMissingArgErr.str(), "\n");
            output << "Missing arguments (" << count << "):\n";
            output << ossMissingArgErr.str() << "\n";

            count = CountOccurrences(ossBadArgDataErr.str(), "\n");
            output << "Bad argument data (" << count << "):\n";
            output << ossBadArgDataErr.str() << "\n";

            count = CountOccurrences(ossMissingDatErr.str(), "\n");
            output << "Missing argument data (" << count << "):\n";
            output << ossMissingDatErr.str() << "\n";

            output << "########################################\n";

            flog << output.str();
            PrintHelpInfo();
            return false;
        }

        return true;
    }
};

#endif  //  __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_CLI_PARSER_H__