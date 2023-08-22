#ifndef __HI_POINTER_INTERFACE_HTTP_REQUEST__
#define __HI_POINTER_INTERFACE_HTTP_REQUEST__

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#include "log.h"
#include "tcp_client.h"
#include "string.h"

namespace HTTP
{
    //  ################## HTTP request structures ###################
    //  ##############################################################

    /**
     * Custom class to define some functionality for an enumeration
     * typed data object. Has a local plain enum for the different HTTP
     * methods available, some initialisation and operator overloads.
     * 
     * Overloads 'is-equals', 'is-not-equals' and 'insertion' operators.
     * 
     * Defines a 'ToStdString' function to return the enumeration type
     * as a predefined string.
     */
    class Method{
    public:
        enum Value : uint8_t{ GET, POST, PUT, DELETE };
        Method() = default;
        constexpr Method(Value aMethod) : value(aMethod){}

        /**
         * Delete bool check operator.
         */
        explicit operator bool() const = delete;
        /**
         * 'is-equals' comparison.
         */
        constexpr bool operator == (Method a) const { return value == a.value; }
        /**
         * 'is-not-equals' comparison.
         */
        constexpr bool operator != (Method a) const { return value != a.value; }
        /**
         * 'insertion' to output stream.
         */
        friend std::ostream& operator << (std::ostream& os, const Method& m){
            os << m.ToStdString();
            return os;
        }

        /**
         * Convert this instance to predefined string.
         */
        std::string ToStdString() const {
            switch (value){
                case Value::GET:
                    return std::string{"GET"}.c_str();
                case Value::POST:
                    return std::string{"POST"}.c_str();
                case Value::PUT:
                    return std::string{"PUT"}.c_str();
                case Value::DELETE:
                    return std::string{"DELETE"}.c_str();
                default:
                    return "";
            }
        }

    private:
        /**
         * This enumeration type value;
         */
        Value value;
    };

    /**
     * The URI string components.
     */
    struct Uri final
    {
        std::string scheme;
        std::string user;
        std::string password;
        std::string host;
        std::string port;
        std::string path;
        std::string query;
        std::string fragment;
    };

    /**
     * The version number for the HTTP header.
     */
    struct Version final
    {
        uint16_t _major;
        uint16_t _minor;
    };

    /**
     * The HTTP Status details. Also holds a standardised compliant
     * response code for RFC 7231, 6. Response Status Codes, and
     * an explanation message.
     */
    struct Status final
    {
        // RFC 7231, 6. Response Status Codes
        enum class Code: std::uint16_t
        {
            //  Information
            
            Continue = 100,
            SwitchingProtocol = 101,
            Processing = 102,
            EarlyHints = 103,

            //  Success

            Ok = 200,
            Created = 201,
            Accepted = 202,
            NonAuthoritativeInformation = 203,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,               //  Used if the socket was disconnected while reading bytes. Read-bytes will return 0 in this instance, because if there are no bytes to read on a socket, then the 'read' command does not return.
            MultiStatus = 207,
            AlreadyReported = 208,
            ImUsed = 226,

            //  Redirected

            MultipleChoice = 300,
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            UseProxy = 305,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,

            //  Client error

            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,                    //  Used when connection attempt returns failure within a few seconds.
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            ProxyAuthenticationRequired = 407,
            RequestTimeout = 408,               //  Used when connection attempt returns failure after a few seconds.
            Conflict = 409,                     //  Used if bytes sent less than 1 or read bytes is less than 0. Sent bytes should always more than 0, and read bytes should always be at least 0, anything else is an error. This could be a temporary error and this command could be tried a second time to see if the problem persists.
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            PayloadTooLarge = 413,
            UriTooLong = 414,
            UnsupportedMediaType = 415,
            RangeNotSatisfiable = 416,
            ExpectationFailed = 417,
            MisdirectedRequest = 421,
            UnprocessableEntity = 422,
            Locked = 423,
            FailedDependency = 424,
            TooEarly = 425,
            UpgradeRequired = 426,
            PreconditionRequired = 428,
            TooManyRequests = 429,
            RequestHeaderFieldsTooLarge = 431,
            UnavailableForLegalReasons = 451,

            //  Server error

            InternalServerError = 500,          //  Used if some sort of unrecoverable error occurred but we don't want to crash the program
            NotImplemented = 501,
            BadGateway = 502,
            ServiceUnavailable = 503,
            GatewayTimeout = 504,
            HttpVersionNotSupported = 505,
            VariantAlsoNegotiates = 506,
            InsufficientStorage = 507,
            LoopDetected = 508,
            NotExtended = 510,
            NetworkAuthenticationRequired = 511,

            InternalProgramError = 999          //  Used when there is an internal error that would usually require a throw clause to be used but we don't want to prevent the program from crashing
        };

        Version _version;
        Code _code;
        std::string _reason;
    };
    
    /**
     * Formatted header field and value pairs.
     */
    struct HeaderField{
        std::string _name;
        std::string _value;
    };
    /**
     * List of header fields for a HTTP request.
     */
    using HeaderFields = std::vector<HeaderField>;

    /**
     * The HTTP response details. Holds HTTP::Status struct, returned
     * header fields and response body.
     */
    struct Response final
    {
        Status _status;
        HeaderFields _headerFields;
        std::vector<std::uint8_t> _body;
    };

    //  ############### Character validation functions ###############
    //  ##############################################################
    
    /**
     * Returns true if 'c' is a digit from 0 to 9, false otherwise.
     * 
     * RFC 5234,Appendix B.1. Core Rules.
     */
    template <typename C>
    constexpr bool IsDigitChar(const C c) noexcept
    {
        return c >= 0x30 && c <= 0x39; // 0 - 9
    }

    /**
     * Returns true if 'c' is an upper or lower case character, false otherwise.
     * 
     * RFC 5234, Appendix B.1. Core Rules.
     */
    template <typename C>
    constexpr bool IsAlphaChar(const C c) noexcept
    {
        return
            (c >= 0x61 && c <= 0x7A) || // a - z
            (c >= 0x41 && c <= 0x5A); // A - Z
    }

    /**
     * Returns true if 'c' is an alpha numeric character or one of
     * the following token characters, false otherwise:
     * 
     * ! # $ % & ' * + - . ^ _ ` | ~
     * 
     * RFC 7230, 3.2.6. Field Value Components.
     */
    template <typename C>
    constexpr bool IsTokenChar(const C c) noexcept
    {
        return c == 0x21 || // !
            c == 0x23 || // #
            c == 0x24 || // $
            c == 0x25 || // %
            c == 0x26 || // &
            c == 0x27 || // '
            c == 0x2A || // *
            c == 0x2B || // +
            c == 0x2D || // -
            c == 0x2E || // .
            c == 0x5E || // ^
            c == 0x5F || // _
            c == 0x60 || // `
            c == 0x7C || // |
            c == 0x7E || // ~
            IsDigitChar(c) ||
            IsAlphaChar(c);
    };

    /**
     * Returns true if 'c' is a white space or tab character, false otherwise.
     * 
     * RFC 7230, 3.2.3. WhiteSpace.
     */
    template <typename C>
    constexpr bool IsWhiteSpaceChar(const C c) noexcept
    {
        return c == 0x20 || c == 0x09; // space or tab
    };

    /**
     * Returns true if 'c' is a 'visible' character, false otherwise.
     * 
     * A visible char has a hex value between 0x21 and 0x7E.
     * 
     * RFC 5234, Appendix B.1. Core Rules.
     */
    template <typename C>
    constexpr bool IsVisibleChar(const C c) noexcept
    {
        return c >= 0x21 && c <= 0x7E;
    }

    /**
     * Returns true if 'c' is an 'obsolete' text character, false otherwise.
     * 
     * An obsolete char has a hex value between 0x80 and 0xFF.
     * 
     * RFC 7230, Appendix B. Collected ABNF.
     */
    template <typename C>
    constexpr bool IsObsoleteTextChar(const C c) noexcept
    {
        return static_cast<unsigned char>(c) >= 0x80 &&
            static_cast<unsigned char>(c) <= 0xFF;
    }

    //  ################## Header parsing functions ##################
    //  ##############################################################

    /**
     * Check the input is a valid Version major and minor number. The
     * HTTP version comes out in the fields line of the header fields.
     * 
     * Throws runtime_error if the HTTP version string is invalid.
     */
    inline Version ParseVersion(String input){
        clog << "Parsing version...";

        if (input[0] != 'H')
            throw std::runtime_error("Invalid HTTP version: index: 0, char: H");
        if (input[1] != 'T')
            throw std::runtime_error("Invalid HTTP version: index: 1, char: T");
        if (input[2] != 'T')
            throw std::runtime_error("Invalid HTTP version: index: 2, char: T");
        if (input[3] != 'P')
            throw std::runtime_error("Invalid HTTP version: index: 3, char: P");
        //  FIXME: Uncomment this when HTTPS has been implemented and verified to work
        //      FIXME: will further need to update the index values once this is done
        // if (input[4] != 'S'){
        //      throw std::runtime_error("Invalid HTTP version: index: 4, char: S");
        // }
        if (input[4] != '/')
            throw std::runtime_error("Invalid HTTP version: index: 4, char: /");

        const uint16_t verMajor = std::stoi(std::string{input[5]});
        
        if (input[6] != '.')
            throw std::runtime_error("Invalid HTTP version: index: 6, char: .");

        const uint16_t verMinor = std::stoi(std::string{input[7]});

        return { ._major = verMajor, ._minor = verMinor };
    }

    /**
     * Check the input is a valid status code value. The status
     * code is a uint16 variable type and can be matched up
     * to a HTTP::Status::Code enum value.
     * 
     * Throws runtime_error if the status code value is invalid.
     */
    inline uint16_t ParseStatusCode(String input){
        clog << "Parsing status code...";

        if (input.length() != 3){
            std::stringstream msg;
            msg << "Invalid status code, not correct length: " << input << ".";
            throw std::runtime_error(msg.str());
        }

        for (char c : input){
            if (!IsDigitChar(c)){
                std::stringstream msg;
                msg << "Invalid status code, char (" << std::to_string(c) << ") was not a digit: " << input << ".";
                throw std::runtime_error(msg.str());
            }
        }

        return static_cast<std::uint16_t>(std::stoi(input));
    }

    /**
     * Returns true if the input is a valid string, that is, if the
     * input only contains white space, visible, and obsolete
     * char's
     * 
     * Throws runtime_error if any of the characters are invalid.
     * 
     * See 'IsWhiteSpaceChar', 'IsVisibleChar' and
     * 'IsObsoleteTextChar' for details.
     */
    inline bool ParseReason(String input){
        for (char c : input){
            if (!IsWhiteSpaceChar(c) && !IsVisibleChar(c) && !IsObsoleteTextChar(c)){
                std::stringstream msg;
                msg << "Invalid reason string, invalid character: " << std::to_string(c) << ".";
                throw std::runtime_error(msg.str());
            }
        }
        return true;
    }

    /**
     * Parse the full status line portion of the header fields and
     * return a HTTP::Status struct object with a Status::Code,
     * the HTTP Version number and a reason phrase. Returns
     * Status::Code::InternalProgramError if an error was thrown
     * and caught from another function.
     * 
     * The reason phrase may be empty with Status::Code 200
     * for OK.
     * 
     * See 'ParseVersion', 'ParseStatusCode' and 'ParseReason'
     * for details.
     */
    inline Status ParseStatusLine(String headerLine){
        clog << "Parsing status line...";

        std::vector<String> headerParts = headerLine.split(' ');

        Version versionResult;
        try {
            versionResult = ParseVersion(headerParts[0]);
        }
        catch (std::runtime_error &e){
            std::stringstream msg;
            msg << "Error parsing version (" << errno << "): " << e.what();
            elog << msg.str();
            return { ._code = Status::Code::InternalProgramError, ._reason = msg.str() };
        }
        clog << "Version = " << versionResult._major << "." << versionResult._minor;

        Status::Code codeResult;
        try {
            codeResult = static_cast<Status::Code>(ParseStatusCode(headerParts[1])); 
        } 
        catch (std::runtime_error &e){
            std::stringstream msg;
            msg << "Error parsing status code (" << errno << "): " << e.what();
            elog << msg.str();
            return { ._code = Status::Code::InternalProgramError, ._reason = msg.str() };
        }
        clog << "Code = " << static_cast<std::uint16_t>(codeResult);

        std::string reasonPhrase;
        try {
            if (ParseReason(headerParts[2])) reasonPhrase = headerParts[2].c_str();
        } 
        catch (std::runtime_error &e){
            std::stringstream msg;
            msg << "Error parsing reason phrase (" << errno << "): " << e.what();
            elog << msg.str();
            return { ._code = Status::Code::InternalProgramError, ._reason = msg.str() };
        }
        clog << "Reason = " << reasonPhrase;

        return { ._version = versionResult, ._code = codeResult, ._reason = reasonPhrase };
    }

    /**
     * Check the token portion of the header field line only
     * contains valid token characters, returns true if so.
     * 
     * Throws runtime_error if there is an invalid character
     * in the token.
     * 
     * See 'IsTokenChar' for details.
     */
    inline bool ParseToken(String input){
        for (char c : input){
            if (!IsTokenChar(c)){
                std::stringstream msg;
                msg << "Invalid token: " << std::to_string(c);
                throw std::runtime_error(msg.str());
            }
        }
        return true;
    }

    /**
     * Check the content portion of the header field line only
     * contains valid characters, returns true if so.
     * 
     * Throws runtime_error if there is an invalid character in
     * the content.
     * 
     * See 'IsWhiteSpaceChar', 'IsVisibleChar' and
     * 'IsObsoleteTextChar' for details.
     */
    inline bool ParseContent(String input){
        for (char c : input){
            if (!IsWhiteSpaceChar(c) && !IsVisibleChar(c) && !IsObsoleteTextChar(c)){
                std::stringstream msg;
                msg << "Invalid content: " << std::to_string(c);
                throw std::runtime_error(msg.str());
            }
        }
        return true;
    }

    /**
     * Parse the full header field line and return a header field
     * object with the token and content values.
     * 
     * Throws runtime_error if the header field line is invalid and
     * re-throws caught runtime_error exceptions
     * 
     * See 'ParseToken' and 'ParseContent' for details.
     */
    inline HeaderField ParseHeaderLine(String headerLine){
        clog << "Parsing header line...";

        std::vector<String> headerParts = headerLine.split(':');

        if (headerParts.size() < 2)
            throw std::runtime_error("Invalid header.");

        String token;
        try {
            if (ParseToken(headerParts[0])) token = headerParts[0];
        }
        catch (std::runtime_error &e){
            throw e;
        }
        clog << "Token: " << token.c_str();

        String content = headerParts[1].trim();
        try {
            if (!ParseContent(content)) content = "";
        }
        catch (std::runtime_error &e){
            throw e;
        }
        clog << "Content: " << content.c_str();

        return { ._name = token.to_lower(), ._value = content };
    }

    //  ############## Encoding and Decoding functions ###############
    //  ##############################################################
    //  NOTE: The back slash is in front of the @ symbol here because doxygen
    //      uses @ symbols to define documentation tags.

    /**
     * Parses or decodes the input URI string and return a a URI data
     * object. The expected format is as follows:
     * 
     * "http://user:pass\@www.url.co.uk:port/path?query=0#fragment"
     * 
     * Throws logic_error if the HTTP scheme was invalid (must be http,
     * FIXME: https not currently implemented).
     * 
     * RFC 3986, 3. Syntax Components.
     */
    template <class Iterator>
    Uri ParseUri(const Iterator begin, const Iterator end){
        clog << "Parsing the input URI string...";

        Uri result;

        // RFC 3986, 3.1. Scheme
        //  Reference the 'begin' input iterator
        auto i = begin;
        if (i == end || !IsAlphaChar(*begin)){
            std::stringstream msg;
            msg << "Invalid scheme, 'begin' iterator was at the end, or first character was not an alpha char.";
            flog << msg.str();
            throw std::logic_error(msg.str());
        }

        //  Add the first character in the uri string
        result.scheme.push_back(*i++);
        
        //  Increment the iterator position before checking these...
        //  Continue adding characters WHILE the iterator is not at the end and either:
        //      - the character at the current iterator position is a char
        //      - the character at the current iterator position is a digit
        //      - the character is one of: plus (+), hyphon (-), or period / dot (.)
        for (; i != end && (IsAlphaChar(*i) || IsDigitChar(*i) || *i == '+' || *i == '-' || *i == '.'); ++i)
            result.scheme.push_back(*i);
            
        //  Validate the characters following this follow the correct 'http://' pattern
        if (i == end || *i++ != ':'){
            std::stringstream msg;
            msg << "Invalid scheme, iterator reached end, or character was not a colon (:).";
            flog << msg.str();
            throw std::logic_error(msg.str());
        }
        if (i == end || *i++ != '/'){
            std::stringstream msg;
            msg << "Invalid scheme, iterator reached end, or character was not a forward slash (/).";
            flog << msg.str();
            throw std::logic_error(msg.str());
        }
        if (i == end || *i++ != '/'){
            std::stringstream msg;
            msg << "Invalid scheme, iterator reached end, or character was not a forward slash (/).";
            flog << msg.str();
            throw std::logic_error(msg.str());
        }

        //  FIXME: Uncomment this when HTTPS has been implemented and verified to work
        if (!(strcmp(result.scheme.c_str(), "http") == 0 /* ||
                strcmp(result.scheme.c_str(), "https") == 0 */)){
            std::stringstream msg;
            msg << "Invalid scheme, scheme was not http or https.";
            flog << msg.str();
            throw std::logic_error(msg.str());
        }

        // RFC 3986, 3.2. Authority
        //      The remainder of the URI after the http or https scheme
        std::string authority = std::string(i, end);

        // RFC 3986, 3.5. Fragment
        //      Typically used for a speific section of a web page
        const auto fragmentPosition = authority.find('#');
        if (fragmentPosition != std::string::npos)
        {
            result.fragment = authority.substr(fragmentPosition + 1);
            authority.resize(fragmentPosition); // remove the fragment part
        }
        
        // RFC 3986, 3.4. Query
        //      uri query parameters
        const auto queryPosition = authority.find('?');
        if (queryPosition != std::string::npos)
        {
            result.query = authority.substr(queryPosition + 1);
            authority.resize(queryPosition); // remove the query part
        }

        // RFC 3986, 3.3. Path
        //      the host is the main portion of the uri, the path is everything after the '.com' section
        const auto pathPosition = authority.find('/');
        if (pathPosition != std::string::npos)
        {
            // RFC 3986, 3.3. Path
            result.path = authority.substr(pathPosition);
            authority.resize(pathPosition);
        }
        else{
            result.path = "/";
        }
        
        // RFC 3986, 3.2.1. User Information
        //      authorisation information goes before the host / main uri part
        std::string userinfo;
        const auto hostPosition = authority.find('@');
        if (hostPosition != std::string::npos)
        {
            userinfo = authority.substr(0, hostPosition);

            const auto passwordPosition = userinfo.find(':');
            if (passwordPosition != std::string::npos)
            {
                result.user = userinfo.substr(0, passwordPosition);
                result.password = userinfo.substr(passwordPosition + 1);
            }
            else
                result.user = userinfo;

            result.host = authority.substr(hostPosition + 1);
        }
        else{
            result.host = authority;
        }

        // RFC 3986, 3.2.2. Host
        //      port number goes between the host / main uri part, and the path
        const auto portPosition = result.host.find(':');
        if (portPosition != std::string::npos)
        {
            // RFC 3986, 3.2.3. Port
            result.port = result.host.substr(portPosition + 1);
            result.host.resize(portPosition);
        }

        clog << "result.scheme: " << result.scheme;
        clog << "result.user: " << result.user;
        clog << "result.password: " << result.password;
        clog << "result.host: " << result.host;
        clog << "result.port: " << result.port;
        clog << "result.path: " << result.path;
        clog << "result.query: " << result.query;
        clog << "result.fragment: " << result.fragment;

        return result;
    }

    /**
     * Return a string with the formatted METHOD request line with
     * TARGET path and query parameters.
     * 
     * The METHOD is any HTTP method such as GET or POST, and
     * the TARGET should be the path after the host, plus any query
     * parameters. The remaining text is added automatically.
     * 
     * Example: "GET /path?query_params=query_value HTTP/1.1\\r\\n".
     * 
     * RFC 7230, 3.1.1. Request Line.
     */
    inline std::string EncodeRequestLine(const std::string& method, const std::string& target)
    {
        std::stringstream result;
        result << method << " " << target << " HTTP/1.1\r\n";
        return result.str();
    }

    /**
     * Return a string for all formatted HEADERFIELDS.
     * 
     * Throws logic_error if one of the header fields is empty, or is
     * one of the characters is not a token, a white space, a visible,
     * or an obsolete character.
     * 
     * RFC 7230, 3.2. Header Fields.
     */
    inline std::string EncodeHeaderFields(const HeaderFields& headerFields)
    {
        std::stringstream result;
        for (const auto& headerField : headerFields)
        {
            if (headerField._name.empty())
                throw std::logic_error{"Invalid header field name"};

            for (const auto c : headerField._name)
                if (!IsTokenChar(c))
                    throw std::logic_error{"Invalid header field name"};

            for (const auto c : headerField._value)
                if (!IsWhiteSpaceChar(c) && !IsVisibleChar(c) && !IsObsoleteTextChar(c))
                    throw std::logic_error{"Invalid header field value"};

            result << headerField._name << ": " << headerField._value << "\r\n";
        }

        return result.str();
    }

    /**
     * Returns a string for the base 64 encoded authorisation string
     * of the HTTP request.
     * 
     * RFC 4648, 4. Base 64 Encoding.
     */
    template <class Iterator>
    std::string EncodeBase64(const Iterator begin, const Iterator end)
    {
        constexpr std::array<char, 64> chars{
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
        };

        std::stringstream result;
        std::size_t c = 0;
        std::array<std::uint8_t, 3> charArray;

        for (auto i = begin; i != end; ++i){
            charArray[c++] = static_cast<std::uint8_t>(*i);
            if (c == 3)
            {
                result << chars[static_cast<std::uint8_t>((charArray[0] & 0xFC) >> 2)];
                result << chars[static_cast<std::uint8_t>(((charArray[0] & 0x03) << 4) + ((charArray[1] & 0xF0) >> 4))];
                result << chars[static_cast<std::uint8_t>(((charArray[1] & 0x0F) << 2) + ((charArray[2] & 0xC0) >> 6))];
                result << chars[static_cast<std::uint8_t>(charArray[2] & 0x3f)];
                c = 0;
            }
        }
        
        if (c){
            result << chars[static_cast<std::uint8_t>((charArray[0] & 0xFC) >> 2)];

            if (c == 1){
                result << chars[static_cast<std::uint8_t>((charArray[0] & 0x03) << 4)];
            }else{  //  c == 2
                result << chars[static_cast<std::uint8_t>(((charArray[0] & 0x03) << 4) + ((charArray[1] & 0xF0) >> 4))];
                result << chars[static_cast<std::uint8_t>((charArray[1] & 0x0F) << 2)];
            }

            while (++c < 4) result << '='; // padding
        }

        return result.str();
    }

    /**
     * @brief   Encode input data to a valid string to send to the API.
     * 
     * @param uri           The URI struct object holding various URI data.
     * @param method        The HTTP method being used (GET, POST etc).
     * @param body          The request body (can be empty).
     * @param headerFields  The header fields (can be empty).
     * @return              A const char* of the correctly formatted html string.
     */
    inline std::string EncodeHtml(const Uri& uri,
                        const std::string& method,
                        const std::vector<uint8_t>& body,
                        HeaderFields headerFields)
    {
        // RFC 7230, 5.3. Request Target
        std::stringstream requestTarget;
        requestTarget << uri.path << (uri.query.empty() ? "" : "?" + uri.query);

        // RFC 7230, 5.4. Host
        headerFields.push_back({ 
            ._name = "Host", 
            ._value = uri.host
        });
        // RFC 7230, 3.3.2. Content-Length
        headerFields.push_back({
            ._name = "Content-Length",
            ._value = std::to_string(body.size())
        });
        
        // RFC 7617, 2. The 'Basic' Authentication Scheme
        if (!uri.user.empty() || !uri.password.empty()){
            std::stringstream userInfo;
            userInfo << uri.user << ':' << uri.password;
            headerFields.push_back({
                ._name = "Authorization", 
                ._value = "Basic " + EncodeBase64(userInfo.str().begin(), userInfo.str().end())
            });
        }

        std::stringstream headerData;
        headerData << EncodeRequestLine(method, requestTarget.str()) 
                << EncodeHeaderFields(headerFields) << "\r\n";

        std::stringstream result;
        result << headerData.str() << body.data();

        return result.str();
    }

    //  ################ The main HTTP Request class #################
    //  ##############################################################

    /**
     * The main class to control, issue, and received formatted data
     * from a HTTP request.
     * 
     * Construct a HTTP::Request object with or without predefined
     * URI and IP values, and use the Request.send functions to
     * initiate a HTTP request.
     */
    class Request final
    {
    public:
        /**
         * @brief   Construct a new HTTP Request object.
         * 
         * @param uriString     The URI string to use.
         * @param ip            The IP address the request is going to.
         * @param ipv           The Internet Protocol Version Number.
         */
        explicit Request(const std::string& uriString, const std::string& ip, 
                            const TcpClient::InternetProtocol::Value ipv = TcpClient::InternetProtocol::Value::v4):
                _ipAddress(ip),
                _uri(ParseUri(uriString.begin(), uriString.end())),
                _ipv(ipv)
        { clog << "A HTTP Request object has been initiailised."; }
        
        /**
         * @brief   Construct a new HTTP Request object.
         * 
         * @param ipv   The Internet Protocol Version Number.
         */
        explicit Request(const TcpClient::InternetProtocol::Value ipv = TcpClient::InternetProtocol::Value::v4):
                _ipv(ipv)
        { clog << "A HTTP Request object has been initiailised."; }
        
        /**
         * Initiate a HTTP request to URISTRING at IP of type METHOD, optionally
         * sending a request BODY with configurable HEADERFIELDS and an optional
         * TIMEOUT threshold for the request (the timeout is strictly for information
         * returned and not used in the network sockets). Return a HTTP::Response
         * struct object.
         * 
         * This function is used when you want to initiate a HTTP request to a different
         * URI and IP than what has been previously stored via the primary constructor,
         * or if one was not supplied when using the secondary constructor.
         * 
         * Marked noexcept so it cannot be caught, as an exception thrown here would
         * be bad code.
         * 
         * A BODY is not required when using the METHOD type GET, and header fields
         * should be left empty unless requiring special header field parameters.
         */
        Response send(const std::string& uriString,
                        const std::string& ip,
                        const Method method = Method::GET,
                        const std::string& body = "",
                        const HeaderFields& headerFields = {},
                        const int timeout_milliseconds = 5000) noexcept
        {
            return send(ParseUri(uriString.begin(), uriString.end()),
                        ip,
                        method, 
                        std::vector<uint8_t>(body.begin(), body.end()),
                        headerFields,
                        timeout_milliseconds);
        }

        /**
         * Initiate a HTTP request of type METHOD, optionally sending a request BODY
         * with configurable HEADERFIELDS and an optional TIMEOUT threshold for
         * the request (the timeout is strictly for information returned and not used in
         * the network sockets). Return a HTTP::Response struct object.
         * 
         * This function is used when you want to initiate a HTTP request to the URI and
         * IP that has already been supplied to this class object instance via the primary
         * constructor, and will throw logic_error if the URI has no scheme.
         * 
         * Marked noexcept so it cannot be caught, as an exception thrown here would
         * be bad code.
         * 
         * A BODY is not required when using the METHOD type GET, and header fields
         * should be left empty unless requiring special header field parameters.
         */
        Response send(const Method method = Method::GET,
                        const std::string& body = "",
                        const HeaderFields& headerFields = {},
                        const int timeout_milliseconds = 5000) noexcept
        {
#pragma GCC diagnostic ignored "-Wterminate"
            if (_uri.scheme.empty())
                throw std::logic_error("No valid scheme to initiate a HTTP request.");
#pragma GCC diagnostic pop

            return send(_uri,
                        _ipAddress,
                        method,
                        std::vector<uint8_t>(body.begin(), body.end()),
                        headerFields,
                        timeout_milliseconds);
        }

        /**
         * Initiate a HTTP request to URI and IP of type METHOD, optionally sending a
         * request BODY with configurable HEADERFIELDS and an optional TIMEOUT
         * threshold for the request (the timeout is strictly for information returned and
         * not used in the network sockets). Return a HTTP::Response struct object.
         * 
         * This is the primary logic function that performs all necessary actions to fulfill
         * the request. A URI struct object, IP, METHOD and BODY must be supplied
         * with this function.
         * 
         * Marked noexcept so it cannot be caught, as an exception thrown here would
         * be bad code.
         * 
         * The BODY can be left empty when using the METHOD type GET, and header
         * fields should be left empty unless requiring special header field parameters.
         */
        Response send(Uri uri,
                        const std::string& ip,
                        const Method method,
                        const std::vector<uint8_t>& body,
                        const HeaderFields& headerFields = {},
                        const int timeout_milliseconds = 5000) noexcept
        {
            clog << "Initiating URI request...";
            const std::string requestData = EncodeHtml(uri, method.ToStdString(), body, headerFields);

            clog << "Constructed request html: \"" << requestData
                << "\", beginning TCP Client initialisation and comms...";
            TcpClient *client = new TcpClient(_ipv);
            
            //  Capture the start time before a connection attempt is made for
            //      later processing
            const auto start_time_connection_attempt = 
                    std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::steady_clock::now().time_since_epoch());

            if (!client->Connect(uri.port.empty() ? 80 : std::stoi(uri.port), ip.c_str())){

                //  Evaluate how much time as passed since the connect call was made
                const auto time_elapsed =
                        std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::steady_clock::now().time_since_epoch())
                            - start_time_connection_attempt;
                elog << "HTTP request failed to connect to host, elapsed time: " 
                    << time_elapsed.count() << ".";

                if (time_elapsed.count() > timeout_milliseconds){
                    std::stringstream msg;
                    msg << "Host took too long to respond, request timeout. "
                        << "Elapsed time (ms): " << time_elapsed.count();

                    elog << msg.str();

                    return { ._status = {
                                ._code = Status::Code::RequestTimeout,
                                ._reason = msg.str() } };
                }else{
                    std::stringstream msg;
                    msg << "Failed to connect before expected timeout, "
                        << "possible host actively refused connection.";

                    elog << msg.str();

                    return { ._status = {
                                ._code = Status::Code::Forbidden,
                                ._reason = msg.str() } };
                }
            }
            
            clog << "Client connected on " << ip << ":" << (uri.port.empty() ? "80" : uri.port);

            const int bytesSent = client->Send(requestData.c_str());
            if (bytesSent < 1){
                std::stringstream msg;
                msg << "Client failed to send any bytes, bytesSent: " << bytesSent 
                    << ", errno: " << errno;

                elog << msg.str();

                return { ._status = {
                            ._code = Status::Code::Conflict,
                            ._reason = msg.str() } };
            }
            
            //  The HTTP response information
            Response response;
            //  The full output response for html content
            std::vector<uint8_t> responseData;
            responseData.clear();

            //  A flag to toggle after the header data has been parsed
            bool headerParsed = false;
            //  Definition of Carriage Return and Line Feed
            const std::string crlf = "\r\n";
            //  The character sequence that defines the end of the header section in the output html
            const std::string headerEndString = "\r\n\r\n";
            
            //  A flag to toggle is the header specifies this is a chunked data reply
            bool chunkedResponse = false;
            //  Define the content length of the response
            std::size_t contentLength = 0U;
            //  Toggle if the content length header was found
            bool contentLengthReceived = false;
            
            //  To store the updated expected chunk size during processing
            std::size_t expectedChunkSize = 0U;
            //  A flag to remove crlf after the chunk
            bool removeCrlfAfterChunk = false;

            clog << "Parsing response...";

            //  Need to continuously read bytes on the socket until no more bytes are available
            while (true)
            {
                //  Read from the socket
                size_t buffSize = 10000;
                uint8_t buff[buffSize];

                const int bytesRead = client->Read(buff, buffSize);

                if (bytesRead < 0){
                    //  error
                    std::stringstream msg;
                    msg << "Client failed to read any bytes, bytesRead: " << bytesRead 
                        << ", errno: " << errno;

                    elog << msg.str();

                    return { ._status = {
                                ._code = Status::Code::Conflict,
                                ._reason = msg.str() } };
                }else if (bytesRead == 0){
                    //  disconnected
                    return response;
                }
                
                clog << "Read bytes: " << bytesRead << ", socket buff:\n\n" << buff << "\n";

                //  Raw output
                responseData.insert(responseData.end(), buff, buff + bytesRead);
                
                if (!headerParsed){
                    clog << "Parsing header...";

                    // RFC 7230, 3. Message Format
                    // Two empty lines indicates the end of the header section (RFC 7230, 2.1. Client/Server Messaging)
                    std::stringstream ss;
                    ss << responseData.data();
                    String fullOutput = ss.str();

                    //  two consecutive CRLFs not found
                    if (!fullOutput.contains(headerEndString)) break;
                    
                    clog << "End of header found, parsing header fields...";

                    std::vector<String> headerLines = fullOutput.split(crlf);
                    if (headerLines.size() < 2){
                        std::stringstream msg;
                        msg << "Error splitting header content into separate lines; "
                            << "Returned headers were not formatted correctly.";
                        elog << msg.str();

                        return { ._status = {
                                    ._code = Status::Code::InternalServerError,
                                    ._reason = msg.str() } };
                    }

                    response._status = ParseStatusLine(headerLines[0]);
                    if (response._status._code == Status::Code::InternalProgramError){
                        std::stringstream msg;
                        msg << "Internal program error occurred with processing.";

                        elog << msg.str();

                        return { ._status = {
                                    ._code = Status::Code::InternalProgramError,
                                    ._reason = msg.str() } };
                    }
                    
                    clog << "\nResponse:\n\tStatus:\n\t\tCode: " 
                        << static_cast<std::uint16_t>(response._status._code) 
                        << "\n\t\tVersion: " 
                            << response._status._version._major 
                            << "." << response._status._version._minor
                        << "\n\t\tReason: " 
                        << response._status._reason;

#pragma GCC diagnostic ignored "-Wsign-compare"
                    for (int i = 1; i < headerLines.size(); i++){
#pragma GCC diagnostic pop

                        HeaderField headerField;
                        try { 
                            headerField = ParseHeaderLine(headerLines[i]); 
                        }
                        catch (std::runtime_error &e){
                            return { ._status = {
                                        ._code = Status::Code::InternalProgramError,
                                        ._reason = e.what() } };
                        }

                        clog << "\nHeader:\n\tName:"
                            << headerField._name
                            << "\n\tValue:"
                            << headerField._value;

                        if (strcmp(headerField._name.c_str(), "transfer-encoding") == 0){
                            // RFC 7230, 3.3.1. Transfer-Encoding
                            if (strcmp(headerField._value.c_str(), "chunked") == 0){
                                chunkedResponse = true;
                            }
                            else{
                                std::stringstream msg;
                                msg << "Unsupported transfer encoding: " << headerField._value;
                                flog << msg.str();

#pragma GCC diagnostic ignored "-Wterminate"
                                throw std::runtime_error(msg.str());
#pragma GCC diagnostic pop

                            }
                        }
                        else if (strcmp(headerField._name.c_str(), "content-length") == 0){
                            // RFC 7230, 3.3.2. Content-Length
                            contentLength = std::stoi(headerField._value);
                            contentLengthReceived = true;
                            response._body.reserve(contentLength);
                        }

                        response._headerFields.push_back(headerField);

                        //  Check if this is the last header field
                        if (strcmp(headerLines[i + 1], "") == 0) break;
                    }
                    
                    // RFC 7230, 3. Message Format
                    // Empty line indicates the end of the header section (RFC 7230, 2.1. Client/Server Messaging)
                    const auto endIterator = std::search(responseData.cbegin(), responseData.cend(),
                                                    headerEndString.cbegin(), headerEndString.cend());

                    responseData.erase(responseData.begin(), endIterator + 4);
                    headerParsed = true;
                }
            
                if (headerParsed){
                    // Content-Length must be ignored if Transfer-Encoding is received (RFC 7230, 3.2. Content-Length)
                    if (chunkedResponse){
                        // RFC 7230, 4.1. Chunked Transfer Coding
                        for (;;){
                            if (expectedChunkSize > 0)
                            {
                                const auto toWrite = (std::min)(expectedChunkSize, responseData.size());
                                response._body.insert(response._body.end(), responseData.begin(),
                                                     responseData.begin() + static_cast<std::ptrdiff_t>(toWrite));
                                responseData.erase(responseData.begin(),
                                                   responseData.begin() + static_cast<std::ptrdiff_t>(toWrite));
                                expectedChunkSize -= toWrite;

                                if (expectedChunkSize == 0) removeCrlfAfterChunk = true;
                                if (responseData.empty()) break;
                            }
                            else
                            {
                                if (removeCrlfAfterChunk)
                                {
                                    if (responseData.size() < 2) break;
                                    
                                    if (!std::equal(crlf.begin(), crlf.end(), responseData.begin())){
                                        flog << "Invalid chunk.";

#pragma GCC diagnostic ignored "-Wterminate"
                                        throw std::runtime_error("Invalid chunk.");
#pragma GCC diagnostic pop

                                    }

                                    removeCrlfAfterChunk = false;
                                    responseData.erase(responseData.begin(), responseData.begin() + 2);
                                }

                                const auto i = std::search(responseData.begin(), responseData.end(),
                                                           crlf.begin(), crlf.end());

                                if (i == responseData.end()) break;

                                std::stringstream ss;
                                ss << responseData.data();
                                String output = ss.str();

                                //  Convert the hex string to int
                                expectedChunkSize = std::stoi(output.split(crlf)[0], 0, 16);
                                responseData.erase(responseData.begin(), i + 2);

                                if (expectedChunkSize == 0)
                                    return response;
                            }
                        }
                    }else{
                        response._body.insert(response._body.end(), responseData.begin(), responseData.end());
                        responseData.clear();

                        // got the whole content
                        if (contentLengthReceived && response._body.size() >= contentLength)
                            return response;
                    }
                }
            }
            
            return response;
        }

    private:
        TcpClient::InternetProtocol::Value _ipv = TcpClient::InternetProtocol::Value::v4;
        std::string _ipAddress;
        Uri _uri;
    };
};

#endif // __HI_POINTER_INTERFACE_HTTP_REQUEST__