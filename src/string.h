//
// Created by Dylan Andrew McAdam on 28/11/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_STRING_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_STRING_H__

#include <iostream>
#include <string.h>
#include <vector>

/**
 * @brief       A template class to instantiate 'begin()' and 'end()' functions
 *              for the custom String class so it can be used in iterator-based
 *              methods.
 * 
 * @tparam T    The type of data to iterate through.
 */
template < typename T >
class Iterator{
private:
    T* _data;
public:
    Iterator(T* data){
        _data = data;
    }

    T &operator*() {
        return *_data;
    }

    Iterator<T> &operator++(){
        _data++;
        return *this;
    }

    bool operator==(const Iterator<T> &a){
        return _data == a._data;
    }

    bool operator!=(const Iterator<T> &a){
        return _data != a._data;
    }
};

/**
 * @brief   The custom String class has been created to replicate high-level
 *          functionality that you might expect when using C# strings such
 *          as '.contains', '.split' and '.replace', although the 'replace'
 *          function already exists on C++ strings, this implementation has been
 *          rewritten to be more simple and more like C# functionality. All
 *          String class objects are null-terminated to prevent stack overflow
 *          errors.
 * 
 */
class String {
private:

    //  ##### Variables #####

    /**
     * @brief   The string (C strings are stored as char arrays - a char pointer
     *          is a char array). This is not exposed as the String class object
     *          itself can be returned as a std::string and char pointer (or a
     *          const char pointer if using the 'c_str()' function).
     */
    char *_str;

    const std::string WHITESPACE = " \n\r\t\f\v";

    //  ##### Match to other overloading operators #####

    /**
     * @brief   Prototype for stream insertion operator so this can be used with
     *          stream insertion functions, such as those used by string stream
     *          and file stream objects.
     * 
     * @param os    The output stream to insert this String in to.
     * @param obj   This String.
     * @return      The std::ostream& object the String data is inserted to so
     *              continuous insertion operators can be appended to one
     *              another.
     */
    friend std::ostream& operator << (std::ostream& os, const String& obj) {
        os << obj._str;
        return os;
    }

    /**
     * @brief   Prototype for the plus ( + ) operator so this allows Strings to
     *          be concatenated to other Strings.
     * 
     * @param lhs   The left hand side of the operator.
     * @param rhs   The right hand side of the operator.
     * @return      The concatenated String data.
     */
    friend String operator + (const String& lhs, const String& rhs) {
        int length = strlen(lhs._str) + strlen(rhs._str);
        char *buff = new char[length + 1];

        strcpy(buff, lhs._str);
        strcat(buff, rhs._str);
        buff[length] = '\0';
        String temp { buff };
        delete[] buff;
        return temp;
    }

    /**
     * @brief   Prototype for the plus ( + ) operator so this allows char's to
     *          be concatenated to Strings.
     * 
     * @param lhs   The left hand side of the operator, a String object.
     * @param rhs   The right hand side of the operator, a char object.
     * @return      The concatenated String data.
     */
    friend String operator + (const String& lhs, const char& rhs){
        int length = strlen(lhs._str) + 1;
        char *buff = new char[length + 1];

        std::string std_temp(1, rhs);
        strcpy(buff, lhs._str);
        strcat(buff, std_temp.c_str());
        buff[length] = '\0';
        String temp { buff };
        delete[] buff;
        return temp;
    }

    /**
     * @brief   Prototype for the plus ( + ) operator so this allows Strings to
     *          be concatenated to char's.
     * 
     * @param lhs   The left hand side of the operator, a char object.
     * @param rhs   The right hand side of the operator, a String object.
     * @return      The concatenated String data.
     */
    friend String operator + (const char& lhs, const String& rhs){
        int length = strlen(rhs._str) + 1;
        char *buff = new char[length + 1];

        std::string std_temp(1, lhs);
        strcpy(buff, std_temp.c_str());
        strcat(buff, rhs._str);
        buff[length] = '\0';
        String temp { buff };
        delete[] buff;
        return temp;
    }

public:

    //  ##### Constructors / initialisation assignment operators #####

    /**
     * @brief   Construct a new String.
     */
    String()
        : _str{nullptr}
    {
        _str = new char[1];
        _str[0] = '\0';
    }

    /**
     * @brief   Construct a new String object from a std::string variable.
     * 
     * @param source    The std::string object being assigned to this String.
     */
    String(const std::string &source){
        _str = new char[strlen(source.c_str()) + 1];
        strcpy(_str, source.c_str());
        _str[strlen(source.c_str())] = '\0';
    }

    /**
     * @brief   Construct a new String object from a const char* variable.
     * 
     * @param source    The const char pointer being assigned to this String.
     */
    String(const char *source) {
        if (source == nullptr){
            _str = new char[1];
            _str[0] = '\0';
        }else{
            _str = new char[strlen(source) + 1];
            strcpy(_str, source);
            _str[strlen(source)] = '\0';
        }
    }

    /**
     * @brief   Construct a new String object from a single char variable.
     * 
     * @param source    The char being assigned to this String.
     */
    String(const char source){
        _str = new char[2];
        _str[0] = source;
        _str[1] = '\0';
    }

    /**
     * @brief   The copy constructor for String class objects.
     * 
     * @param source    The String object being copied, in memory to this String.
     */
    String(const String &source) {
        _str = new char[strlen(source._str) + 1];
        strcpy(_str, source._str);
        _str[strlen(source._str)] = '\0';
    }

    /**
     * @brief   The move constructor for String class objects.
     * 
     * @param source    The String object being moved, in memory, to this String.
     */
    String(String &&source) {
        _str = source._str;
        source._str = nullptr;
    }

    /**
     * @brief   Destroy this String object. Destructor for the String class.
     */
    ~String(){ delete _str; }

    //  ##### Common overloaded assignment operators #####

    /**
     * @brief   Overload the standard 'equal' assignment operator to assign a
     *          String object to this String object.
     * 
     * @param rhs   The right hand side of the operator.
     * @return      The resulting String& object after being assigned new data.
     */
    String& operator = (const String& rhs) {
        if (this == &rhs)
            return *this;
        delete[] _str;
        _str = new char[strlen(rhs._str) + 1];
        strcpy(_str, rhs._str);
        _str[strlen(rhs._str)] = '\0';
        return *this;
    }

    /**
     * @brief   Overloading the plus-equals ( += ) assignment operator to
     *          concatenate Strings to this String object.
     * 
     * @param rhs   The right hand side of the operator.
     * @return      The resulting String& object after data is concatenated
     */
    String& operator += (const String& rhs) {
        if (this == &rhs)
            return *this;
        String temp{_str };
        delete[] _str;
        _str = new char[strlen(temp._str) + strlen(rhs._str) + 1];
        strcpy(_str, temp._str);
        strcat(_str, rhs._str);
        _str[strlen(temp._str) + strlen(rhs._str)] = '\0';
        return *this;
    }

    /**
     * @brief   Allow return data type to be std::string.
     * 
     * @return  A std::string representation of this String data.
     */
    operator std::string() { return _str; }
    /**
     * @brief   Allow return data type to char*.
     * 
     * @return  A char* representation of this String data.
     */
    operator char*() { return _str; }

    //  ##### Existing string functions to mimic std::string #####

    /**
     * @brief   The definition for the 'begin' iterator function.
     * 
     * @return  An Iterator<char> for the start of this String data.
     */
    Iterator<char> begin() {
        Iterator<char> iter(_str);
        return iter;
    }

    /**
     * @brief   The definition for the 'end' iterator function.
     * 
     * @return  An Iterator<char> for the end of this String data.
     */
    Iterator<char> end() {
        Iterator<char> iter(_str + strlen(_str));
        return iter;
    }

    /**
     * @brief   Froce this String data to return as a const char*.
     * 
     * @return  A const char* representation of this String data.
     */
    const char *c_str() const { return _str; }

    /**
     * @brief   Get the length of the String data.
     * 
     * @return  An integer for the character count.
     */
    const int length() { return strlen(_str); }

    /**
     * @brief   Get a substring.
     * 
     * @param start     The start character index.
     * @param len       The number of characters onward from the start index.
     * @return          A String object of the new String data.
     */
    String substr(const int start, const int len) {
        std::string str = _str;
        return String(str.substr(start, len).c_str());
    }

    //  ##### Existing string functions but modified #####

    /**
     * @brief   Returns a new String with all occurrences of a specified Unicode
     *          character in this instance replaced with another specified
     *          Unicode character.
     * 
     * @param a     The char being replaced.
     * @param b     The char to replace with.
     * @return      A String object of the new String data.
     */
    String replace(const char a, const char b) {
        const int length = strlen(_str);
        char *buff = new char[length + 1];

        strcpy(buff, _str);
        buff[length] = '\0';

//  Ignore the sign comparison warning as it does not affect main running code
#pragma GCC diagnostic ignored "-Wsign-compare"

        for (int i = 0; i < strlen(buff); i++){
            if (buff[i] == a){
                buff[i] = b;
            }
        }

#pragma GCC diagnostic pop

        String temp = String(buff);
        delete[] buff;
        return temp;
    }

    /**
     * @brief   Returns a new String with all occurrences of specified Unicode
     *          characters in this instance replaced with other specified
     *          Unicode characters.
     * 
     * @param a     The string being replaced.
     * @param b     The string to replace with.
     * @return      A String object of the new String data.
     */
    String replace(const char *a, const char *b) {
        if (a == nullptr || b == nullptr || a == NULL || b == NULL || strlen(a) < 1){
            return String(_str);
        }
        const int length = strlen(_str);
        char *buff = new char[length + 1];

        strcpy(buff, _str);
        buff[length] = '\0';

        std::string s = buff;

        const int length_a = strlen(a);
        size_t pos = 0;
        size_t diff = 0;
        bool diffSet = false;
        while ((pos = s.find(a, pos + diff)) != std::string::npos){
            s.replace(pos, length_a, b);
            if (!diffSet) {
                diff = strlen(b);
                diffSet = true;
            }
        }

        String temp = String(s.c_str());
        delete[] buff;
        return temp;
    }

    /**
     * @brief   Returns a new String with all occurrences of specified Unicode
     *          characters in this instance replaced with other specified
     *          Unicode characters.
     * 
     * @param a     The string being replaced.
     * @param b     The string to replace with.
     * @return      A String object of the new String data.
     */
    String replace(std::string a, std::string b) {
        return replace(a.c_str(), b.c_str());
    }

    /**
     * @brief   Returns a new String with all occurrences of specified Unicode
     *          characters in this instance replaced with another specified
     *          Unicode character.
     * 
     * @param a     The string being replaced.
     * @param b     The char to replace with.
     * @return      A String object of the new String data. 
     */
    String replace(const char* a, const char b){
        if (a == nullptr || a == NULL || strlen(a) < 1){
            return String(_str);
        }
        std::string temp(1, b);
        const char* newB = temp.c_str();
        return replace(a, newB);
    }

    /**
     * @brief   Returns a new String with all occurrences of specified Unicode
     *          characters in this instance replaced with another specified
     *          Unicode character.
     * 
     * @param a     The string being replaced.
     * @param b     The char to replace with.
     * @return      A String object of the new String data. 
     */
    String replace(std::string a, const char b){
        return replace(a.c_str(), b);
    }

    /**
     * @brief   Returns a new String with all occurrences of a specified Unicode
     *          character in this instance replaced with other specified Unicode
     *          characters.
     * 
     * @param a     The char being replaced.
     * @param b     The string to replace with.
     * @return      A String object of the new String data.
     */
    String replace(const char a, const char* b){
        if (b == nullptr || b == NULL || a == '\0'){
            return String(_str);
        }
        std::string temp(1, a);
        const char* newA = temp.c_str();
        return replace(newA, b);
    }

    /**
     * @brief   Returns a new String with all occurrences of a specified Unicode
     *          character in this instance replaced with other specified Unicode
     *          characters.
     * 
     * @param a     The char being replaced.
     * @param b     The string to replace with.
     * @return      A String object of the new String data.
     */
    String replace(const char a, std::string b){
        return replace(a, b.c_str());
    }

    //  ##### Custom string functions #####

    /**
     * @brief   Split this String into substrings based on the input delimiter
     *          characters.
     * 
     * @param delim     The string delimiter to spli on.
     * @return          A std::vector<String> object with
     */
    std::vector<String> split(const std::string delim) {
        std::string str = _str;
        str += delim;
        std::vector<String> output;

        size_t p = 0;
        while ((p = str.find(delim)) != std::string::npos){
            String temp = str.substr(0, p).c_str();
            output.push_back(temp);
            str.erase(0, p + delim.length());
        }

        return output;
    }

    /**
     * @brief   Split this String into substrings based on the input delimiter
     *          character.
     * 
     * @param delim 
     * @return std::vector<String> 
     */
    std::vector<String> split(const char delim) {
        std::string str = _str;
        str += delim;
        std::vector<String> output;

        size_t p = 0;
        while ((p = str.find(delim)) != std::string::npos){
            String temp = str.substr(0, p).c_str();
            output.push_back(temp);
            str.erase(0, p + 1);
        }

        return output;
    }

    /**
     * @brief   Returns a value indicating whether a specified character occurs
     *          within this String.
     * 
     * @param content   The character to look for.
     * @return true     if the character appears within this String,
     * @return false    otherwise.
     */
    bool contains(const char content) {
        std::string str = _str;
        return str.find(content) != std::string::npos;
    }

    /**
     * @brief   Returns a value indicating whether the specified string occurs
     *          within this String.
     * 
     * @param content   The string to look for.
     * @return true     if the string appears within this String,
     * @return false    otherwise.
     */
    bool contains(const std::string content) {
        std::string str = _str;
        return str.find(content) != std::string::npos;
    }

    String ltrim(){
        std::string s = _str;
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos ? "" : s.substr(start));
    }

    String rtrim(){
        std::string s = _str;
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos ? "" : s.substr(0, end + 1));
    }

    String trim(){
        std::string s = _str;
        auto start = s.begin();
        while (start != s.end() && std::isspace(*start)){
            start++;
        }

        auto end = s.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));

        return std::string(start, end + 1);
    }

};


#endif //__DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_STRING_H__
