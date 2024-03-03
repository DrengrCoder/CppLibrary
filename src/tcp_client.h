//
// Created by Dylan Andrew McAdam (DrengrCoder) on 05/12/22.
//  v1.0.1 TODO: UPDATE THIS
//

#ifndef __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__
#define __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/ioctl.h>

#include "log.h"

/**
 * The custom TCP Socket class is designed to simplify the process of using and
 * setting up network sockets for a TCP data stream in C++. A log message
 * printed. 'ERR_NO' and 'ERR_MSG' are set.
 *
 * This TCP Client object holds a reference to the configured socket file
 * descriptor that is used for sending and reading data over the network socket,
 * as well as storing the server file descriptor this client connected to.
 *
 * __errno and __errmsg are local private variables that are set upon error, and
 * can be accessed using ERR_NO() and ERR_MSG() functions.
 *
 * NOTE:
 * There is a unique error code numbering system implemented for this class.
 * The error code system is a combination of custom error codes (between 100
 * and 9999) and partial custom error codes combined with the ‘errno’ macro
 * to produce values 10000 and over.
 *
 * Custom error codes between 100 and 9999 denote some sort of usage
 * error and have been documented in the readme. Error codes 10000 and
 * over follow the pattern YYXXX where YY is a 2 digit custom code I have
 * given it, and XXX is the 'errno' macro error code added to the value.
 * Examples:
 * - '12047' is the custom error code '12' and the errno code '047' or just '47'.
 * - '10111' is the custom error code '10' and the errno code '111'.
 *
 * The custom codes are documented in this readme. 'errno' code meaning
 * will change depending on what caused the errno code to be set.
 */
class TcpClient {
public:
    /**
     * The internet protocol version.
     */
    enum InternetProtocol : uint8_t { v4, v6 };

    /**
     * Construct a new Tcp Client object with a socket file descriptor
     * and some default parameters for a TCP type connection:
     * 'socket(AF_INET, SOCK_STREAM, 0)'. Address Options: AF_INET
     * family.
     */
    TcpClient(InternetProtocol ipv = InternetProtocol::v4) {
        __errno = 0;
        __errmsg = "";

        if (_socketFd != -1 || _serverFd != -1)
            Close();

        llog << "Initialise new TCP client object...";

        _ipv = ipv;
        _socketFd = socket((_ipv == InternetProtocol::v4 ? AF_INET : AF_INET6), SOCK_STREAM, 0);
        if (_socketFd < 0) {
            __errno = 10000 + errno;

            std::stringstream msg;
            msg << "Client socket creation failed: _socketFd: "
                << _socketFd << ". ERROR CODE: " << __errno << ".";

            flog << msg.str();
            
            return;
        }

        _address.sin_family = (_ipv == InternetProtocol::v4 ? AF_INET : AF_INET6);

        llog << "TCP socket object initialised.";
    }

    /**
     * Destroy the Tcp Client object and perform necessary clean up.
     */
    ~TcpClient() {
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
    TcpClient(const int sockFd) {
        llog << "Storing ref to pre-existing socket file descriptor: "
            << sockFd << "...";

        _socketFd = sockFd;

        llog << "TCP client object initialised with file descriptor: "
            << _socketFd << ".";
    }

    /**
     * Close this client socket and the server socket with the socket
     * file descriptors.
     */
    void Close() {
        dlog << "Closing socket " << _socketFd << " and server " << _serverFd
            << "...";
        close(_serverFd);
        close(_socketFd);
        _serverFd = -1;
        _socketFd = -1;
    }

    /**
     * @brief   Connect this socket to a port number and ip address. Sets
     *          __errmsg and __errno on error.
     *
     * @param portNumber    The port number to connect to.
     * @param ip            The IP address to connect to.
     * @return true         if the socket successfully connected to the endpoint,
     * @return false        otherwise.
     */
    bool Connect(const int portNumber, const char* ip = "127.0.0.1") {
        __errno = 0;
        __errmsg = "";

        llog << "Connecting to " << ip << ":" << portNumber << "...";

        _address.sin_port = htons(portNumber);

        const int inet_result = inet_pton((_ipv == InternetProtocol::v4 ? AF_INET : AF_INET6), ip, &_address.sin_addr);
        if (inet_result <= 0) {
            std::stringstream msg;
            msg << "Address invalid / not supported: inet_result: "
                << inet_result << ".";

            flog << msg.str();
            __errmsg = msg.str();
            __errno = 11000 + errno;
            return false;
        }

        _serverFd = connect(_socketFd, (struct sockaddr*) &_address, _addressLength);
        if (_serverFd < 0) {
            std::stringstream msg;
            msg << "Connection failed: _serverFd: "
                << _serverFd << ".";

            flog << msg.str();
            __errmsg = msg.str();
            __errno = 12000 + errno;
            return false;
        }

        llog << "Connected on " << ip << ":" << portNumber << ".";
        return true;
    }

    /**
     * Read N_BYTES into BUFF on this socket. Return
     * number of bytes read, -1 if error or 0 if EOF.
     *
     * Sets __errmsg and __errno on error.
     */
    int Read(void* buff, size_t n_bytes, int flags = 0) {
        __errmsg = "";
        __errno = 0;

        if (_socketFd < 0) {
            std::stringstream msg;
            msg << "Socket read error, tried reading without valid socket file "
                << "descriptor: " << _socketFd << ".";
            elog << msg.str();
            __errmsg = msg.str();
            __errno = 101;
            return -1;
        }

        memset(buff, 0, n_bytes);
        int bytes = recv(_socketFd, buff, n_bytes, flags);
        if (bytes < 0) {
            std::stringstream msg;
            msg << "Error reading bytes, _socketFd: " << _socketFd << ", bytes: "
                << bytes << ".";

            elog << msg.str();
            __errmsg = msg.str();
            __errno = 13000 + errno;
        } else if (bytes == 0) {
            dlog << "No bytes were read.";
        } else {
            llog << bytes << " bytes read.";
        }

        return bytes;
    }

    /**
     * Send INPUT string to the connected socket. Return
     * number of bytes read, -1 if error or 0 if EOF. Uses
     * STRLEN(input) to calculate N_BYTES for cascading
     * function calls. NOTE: This function will not work for
     * any data that is not simply a C-String, as C-String's
     * are null-terminated and if the data contains any null
     * characters, the length calculation will assume this is
     * the end of the data.
     *
     * Sets __errmsg and __errno on error.
     */
    int Send(const char* input) {
        return Send(input, strlen(input), 0);
    }

    /**
     * Send N_BYTES of BUFF to the connected socket. Return
     * number of bytes read, -1 if error or 0 if EOF.
     *
     * Sets __errmsg and __errno on error.
     */
    int Send(const void* buff, size_t n_bytes, int flags = 0) {
        __errmsg = "";
        __errno = 0;

        if (_socketFd < 0) {
            std::stringstream msg;
            msg << "Socket send error, tried sending without valid socket file "
                << "descriptor: " << _socketFd << ".";
            elog << msg.str();
            __errmsg = msg.str();
            __errno = 101;
            return -1;
        }

        int bytes = send(_socketFd, buff, n_bytes, flags);
        if (bytes < 0) {
            std::stringstream msg;
            msg << "Error sending bytes, _socketFd: " << _socketFd << ", bytes: "
                << bytes << ".";

            elog << msg.str();
            __errmsg = msg.str();
            __errno = 13000 + errno;
        } else if (bytes == 0) {
            wlog << "No bytes were sent.";
        } else {
            llog << bytes << " bytes sent: " << buff;
        }

        return bytes;
    }

    /**
     * Returns the number of bytes available to read on the socket, or
     * -1 on error.
     * 
     * Sets _errmsg and __errno on error.
     */
    int BytesAvailable() {
        __errmsg = "";
        __errno = 0;

        if (_socketFd < 0) {
            std::stringstream msg;
            msg << "Socket read error, tried checking for available bytes on "
                << "without valid socket file descriptor: " << _socketFd << ".";
            elog << msg.str();
            __errmsg = msg.str();
            __errno = 101;
            return -1;
        }
        
        int bytesAvailable;
        if (ioctl(_socketFd, FIONREAD, &bytesAvailable) < 0) {
            std::stringstream msg;
            msg << "Error reading available byte count, _socketFd: "
                << _socketFd << ".";

            elog << msg.str();
            __errmsg = msg.str();
            __errno = 13000 + errno;
            return -1;
        }

        return bytesAvailable;
    }

    /**
     * Get this client socket's file descriptor value.
     */
    int GetSocketFd() { return _socketFd; }

    /**
     * Get the Server file descriptor value this client connected to.
     */
    int GetServerFd() { return _serverFd; }

    /**
     * Get the last error message set on this object.
     */
    std::string ERR_MSG() { return __errmsg; }

    /**
     * Get the last error code set on this object.
     */
    int ERR_NO() { return __errno; }

protected:
    /**
     * The file descriptor for THIS socket object.
     */
    int _socketFd = -1;

    /**
     * The file descriptor for the server-side socket this client is connected to.
     */
    int _serverFd = -1;

    /**
     * The Internet Protocol version number (IPv number).
     */
    InternetProtocol _ipv = InternetProtocol::v4;

    /**
     * This client socket's address structure.
     */
    struct sockaddr_in _address;

    /**
     * The byte length of the address in memory.
     */
    const int _addressLength = sizeof(_address);

    /**
     * The last error message set.
     */
    std::string __errmsg;

    /**
     * The last error code set.
     */
    int __errno;
};

#endif // __DAM_DRENGR_CODER_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__