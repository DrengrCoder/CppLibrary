//
// Created by Dylan Andrew McAdam on 05/12/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
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
 * This TCP Client object holds a reference to the configured socket file
 * descriptor that is used for sending and reading data over the network socket,
 * as well as storing the server file descriptor this client connected to.
 */
class TcpClient {
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
     * @brief   Construct a new Tcp Client object with a socket file descriptor
     *          and some default parameters for a TCP type connection.
     *          'socket(AF_INET, SOCK_STREAM, 0)'. Address Options: AF_INET
     *          family.
     * 
     * @throw runtime_error if the socket failed to initialise and we did not
     *                      receive a file descriptor.
     */
    TcpClient(InternetProtocol::Value ipv = InternetProtocol::Value::v4){
        clog << "Initialise new TCP client object...";

        _socketFd = socket((ipv == InternetProtocol::Value::v4 ? AF_INET : AF_INET6), SOCK_STREAM, 0);
        if (_socketFd < 0){
            std::stringstream msg;
            msg << "Client socket creation failed: _socketFd: " << _socketFd
                << ", errno: " << errno;

            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        _address.sin_family = (ipv == InternetProtocol::Value::v4 ? AF_INET : AF_INET6);

        clog << "TCP socket object initialised.";
    }

    /**
     * @brief   Destroy the Tcp Client object and perform necessary clean up.
     */
    ~TcpClient(){
        dlog << "TCP Client socket fd: " << _socketFd << " and server fd: "
            << _serverFd << " destruction...";
        Close();
    }

    /**
     * @brief   Construct a new Tcp Client object with a pre-configured socket
     *          file descriptor. This function should be used if you already
     *          have a socket file descriptor by some other means, such as the
     *          return value from 'NextConnection' on the custom TCP Server class.
     * 
     * @param sockFd    The socket file descriptor value.
     */
    TcpClient(const int sockFd){
        clog << "Storing ref to pre-existing socket file descriptor: " << sockFd
            << "...";

        _socketFd = sockFd;

        clog << "TCP client object initialised with file descriptor: " << _socketFd
            << ".";
    }

    /**
     * @brief   Close this client socket and the server socket with the socket
     *          file descriptors.
     */
    void Close(){
        dlog << "Closing socket " << _socketFd << " and server " << _serverFd
            << "...";
        close(_serverFd);
        close(_socketFd);
        _serverFd = -1;
        _socketFd = -1;
    }

    /**
     * @brief   Connect this socket to a port number and ip address. 
     * 
     * @param portNumber    The port number to connect to.
     * @param ip            The IP address to connect to.
     * @return true         if the socket successfully connected to the endpoint,
     * @return false        otherwise.
     */
    bool Connect(const int portNumber, const char *ip = "127.0.0.1"){
        clog << "Connecting to " << ip << ":" << portNumber << "...";

        _address.sin_port = htons(portNumber);

        const int inet_result = inet_pton(AF_INET, ip, &_address.sin_addr);
        if (inet_result <= 0){
            std::stringstream msg;
            msg << "Address invalid / not supported: inet_result: "
                << inet_result << ", errno: " << errno;

            flog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        _serverFd = connect(_socketFd, (struct sockaddr*)&_address, _addressLength);
        if (_serverFd < 0){
            std::stringstream msg;
            msg << "Connection failed: _serverFd: " << _serverFd << ", errno: "
                << errno;

            flog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        clog << "Connected on " << ip << ":" << portNumber << ".";
        return true;
    }

    /**
     * @brief   Read n_bytes into buff on this socket.
     * 
     * @param buff      The char array to output the bytes to.
     * @param n_bytes   The number of bytes to read on the socket at once.
     * @return          The number of bytes read on the socket, or -1 on error.
     */
    int Read(void *buff, size_t n_bytes){
        if (_socketFd < 0){
            elog << "Socket read error, tried reading without valid socket file descriptor: "
                << _socketFd;
            return -1;
        }

        memset(buff, '\0', n_bytes);
        int bytes = read(_socketFd, buff, n_bytes);
        if (bytes < 0){
            std::stringstream msg;
            msg << "Error reading bytes, _socketFd: " << _socketFd << ", bytes: "
                << bytes << ", errno: " << errno;

            elog << msg.str();
            __err_msg = msg.str();
        } else if (bytes == 0){
            wlog << "No bytes were read.";
        } else {
            clog << bytes << " bytes read.";
        }

        return bytes;
    }

    /**
     * @brief   Send input string to the connected socket.
     * 
     * @param input     The string of characters to send.
     * @return          The number of bytes sent, or -1 on error.
     */
    int Send(const char *input){
        if (_socketFd < 0){
            elog << "Socket send error, tried sending without valid socket file descriptor: "
                << _socketFd;
            return -1;
        }

        int bytes = send(_socketFd, input, strlen(input), 0);
        if (bytes < 0){
            std::stringstream msg;
            msg << "Error sending bytes, _socketFd: " << _socketFd << ", bytes: "
                << bytes << ", errno: " << errno;

            elog << msg.str();
            __err_msg = msg.str();
        } else if (bytes == 0){
            wlog << "No bytes were sent.";
        } else {
            clog << bytes << " bytes sent: " << input;
        }

        return bytes;
    }

    /**
     * @brief   Get this client socket's file descriptor value.
     * 
     * @return  The client socket's file descriptor integer.
     */
    int GetSocketFd(){ return _socketFd; }

    /**
     * @brief   Get the Server file descriptor value this client connected to.
     * 
     * @return  The server socket file descriptor integer.
     */
    int GetServerFd(){ return _serverFd; }

    /**
     * @brief   Get the last error message that occurred on this object.
     * 
     * @return  The last error message when an error occurs on this object. This
     *          variable will contain the 'errno' value.
     */
    std::string ERROR_MSG(){
        return __err_msg;
    }

protected:
    /**
     * @brief   The file descriptor for THIS socket object.
     */
    int _socketFd = -1;

private:
    /**
     * @brief   The file descriptor for the socket this client is connected to.
     */
    int _serverFd = -1;

    /**
     * @brief   This client socket's address structure.
     */
    struct sockaddr_in _address;

    /**
     * @brief   The byte length of the address in memory.
     */
    const int _addressLength = sizeof(_address);

    /**
     * @brief   The last error message.
     */
    std::string __err_msg;
};

#endif // __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__