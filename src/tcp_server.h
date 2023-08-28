//
// Created by Dylan Andrew McAdam on 05/12/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <vector>

#include "log.h"

/**
 * The custom TCP Socket class is designed to simplify the process of using and
 * setting up network sockets for a TCP data stream in C++. A std::runtime_error
 * is thrown in places where an unrecoverable error occurs, along with a log
 * message printed and 'errno' being set and printed int he message as well,
 * otherwise the function will either return false or -1 where appropriate along
 * with a custom error message set that can be accessed via the ERROR_MSG string
 * function.
 * 
 * This class specifically and only listens for connect requests on a given port
 * number, if you need to write data back to that socket, the 'NextConnection'
 * function will return an integer for the socket file descriptor value, and
 * this should be used to initialise a custom TCP Client class object or to send
 * bytes via your own commands.
 */
class TcpServer{
public:
    class InternetProtocol{
    public:
        /**
         * The enumeration values.
         */
        enum class Value : uint8_t { v4, v6 };
        InternetProtocol() = default;
        constexpr InternetProtocol(Value aState) : value(aState){}
        /**
         * Delete the default bool operator.
         */
        explicit operator bool() const = delete;
        /**
         * 'is equals' operator.
         */
        constexpr bool operator == (InternetProtocol a) const { return value == a.value; }
        /**
         * 'is not equals' operator.
         */
        constexpr bool operator != (InternetProtocol a) const { return value != a.value; }

        /**
         * Get the enumeration values in a std::vector.
         */
        static std::vector<Value> GetValues(){
            std::vector<Value> result;
            result.push_back(Value::v4);
            result.push_back(Value::v6);
            return result;
        }

        /**
         * Convert enumeration value to exact string representation.
         */
        const char* c_str() const{
            switch (value){
                case Value::v4:
                    return "IPv4";
                case Value::v6:
                    return "IPv6";
                default:
                    return "";
            }
        }

        /**
         * Convert enumeration value to exact string representation.
         */
        static const char* c_str(Value a){
            switch (a){
                case Value::v4:
                    return "IPv4";
                case Value::v6:
                    return "IPv6";
                default:
                    return "";
            }
        }

    private:
        Value value;
    };

    /**
     * @brief   Construct a new Tcp Server object with a socket file descriptor
     *          and some default parameters for a TCP type connection.
     *          'socket(AF_INET, SOCK_STREAM, 0)' and 'setsockopt(_serverFd,
     *          SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt))'. Address Options:
     *          AF_INET family, INADDR_ANY s_addr.
     * 
     * @throw runtime_error if the socket failed to initialise and we did not
     *                      receive a file descriptor or if the socket options
     *                      failed to set.
     */
    TcpServer(InternetProtocol::Value ipv = InternetProtocol::Value::v4){
        clog << "Initialise new TCP server object...";

        _serverFd = socket((ipv == InternetProtocol::Value::v4 ? AF_INET : AF_INET6), SOCK_STREAM, 0);
        if (_serverFd < 0){
            std::stringstream msg;
            msg << "Server socket creation failed: _serverFd: " << _serverFd
                << ", errno: " << errno << ".";

            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        const int sockOptResult =
            setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));
        if (sockOptResult < 0){
            std::stringstream msg;
            msg << "Socket options failed: sockOptResult: " << sockOptResult
                << ", errno: " << errno << ".";

            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        _address.sin_family = (ipv == InternetProtocol::Value::v4 ? AF_INET : AF_INET6);
        _address.sin_addr.s_addr = INADDR_ANY;

        clog << "TCP server object initialised.";
    }

    /**
     * @brief   Destroy the Tcp Server object and perform necessary clean up.
     */
    ~TcpServer(){
        dlog << "TCP Server destruction...";
        this->Shutdown();
    }

    /**
     * @brief   Shuts down server socket file descriptor and resets the file
     *          descriptor value.
     */
    void Shutdown(){
        dlog << "Shutting down TCP Server...";
        shutdown(_serverFd, SHUT_RDWR);
        _serverFd = -1;
    }

    /**
     * @brief   Start listening to the socket on a given port number.
     * 
     * @param portNumber    The port number for this TCP server instance to
     *                      listen on.
     * @return true         if the socket successfully started listening to the
     *                      port,
     * @return false        otherwise.
     * @throw runtime_error if binding the address to the socket fails.
     */
    bool StartListening(const int portNumber){
        clog << "Start listening on " << portNumber << "...";

        _address.sin_port = htons(portNumber);

        const int bindResult =
            bind(_serverFd, (struct sockaddr*)&_address, _addressLength);
        if (bindResult < 0){
            std::stringstream msg;
            msg << "Binding failed: bindResult: " << bindResult << ", errno: "
                << errno << ".";

            elog << msg.str();
            throw std::runtime_error(msg.str());
        }

        const int listenResult = listen(_serverFd, _maxQueueLength);
        if (listenResult < 0){
            std::stringstream msg;
            msg << "Listen failed on port " << portNumber << ": listenResult: "
                << listenResult << ", errno: " << errno << ".";

            elog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        return true;
    }

    /**
     * @brief   Gets the next pending connection in the queue for the server
     *          socket file descriptor.
     * 
     * @return  The connected socket file descriptor value, or -1 if it failed
     *          to accept the new connection.
     */
    int NextConnection(){
        clog << "Accepting next connection in queue...";

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
        int newSocket = accept(_serverFd, (struct sockaddr*)&_address, 
                                (socklen_t*)&_addressLength);
#pragma GCC diagnostic pop

        if (newSocket < 0){
            std::stringstream msg;
            msg << "Failed to accept new connection: _serverFd: " << _serverFd
                << ", errno: " << errno << ".";

            flog << msg.str();
            __err_msg = msg.str();
            return -1;
        }

        return newSocket;
    }

    /**
     * @brief   Set the maximum queue size for this TCP server listener. This
     *          value should be set before 'StartListening' is called.
     * 
     * @param len   The size to set as an integer. 50 is the specified maximum
     *              for this class object, setting anything over 50 will forcibly
     *              set it to 50.
     */
    void SetMaximumQueueSize(int len){ _maxQueueLength = (len > 50 ? 50 : len); }

    /**
     * @brief   Get this server socket's file descriptor value.
     * 
     * @return  The ID for the server socket file descriptor as an integer.
     */
    int GetSocketFd(){ return _serverFd; }

    /**
     * @brief   Get the last error message that occurred on this object.
     * 
     * @return  The last error message when an error occurs on this object. This
     *          variable will contain the 'errno' value.
     */
    std::string ERROR_MSG(){ return __err_msg; }

private:
    /**
     * @brief   This TCP server instance's socket file descriptor.
     */
    int _serverFd = -1;

    /**
     * @brief   This server socket's address structure.
     */
    struct sockaddr_in _address;

    /**
     * @brief   The byte length of the address in memory.
     */
    const int _addressLength = sizeof(_address);

    /**
     * @brief   For setting socket option value. Specified as a const integer so
     *          it can be used as a reference parameter.
     */
    const int _opt = 1;

    /**
     * @brief   The maximum queue length for this server socket listener.
     */
    int _maxQueueLength = 10;
    
    /**
     * @brief   The last error message.
     */
    std::string __err_msg;
};

#endif // __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__