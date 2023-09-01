//
// Created by Dylan Andrew McAdam (DrengrCoder) on 05/12/22.
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
 * message printed. 'ERR_NO' and 'ERR_MSG' are set.
 * 
 * This class specifically and only listens for connection requests on a given port
 * number, if you need to write data back to that socket, the 'NextConnection'
 * function will return an integer for the socket file descriptor value, and this
 * should be used to initialise a custom TCP Client class object or to send bytes
 * via your own commands.
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
class TcpServer{
public:
    /**
     * The internet protocol version.
     */
    enum InternetProtocol : uint8_t { v4, v6 };

    /**
     * @brief   Construct a new Tcp Server object with a socket file descriptor
     *          and some default parameters for a TCP type connection:
     *          'socket(AF_INET, SOCK_STREAM, 0)' and 'setsockopt(_serverFd,
     *          SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt))'. Address Options:
     *          AF_INET family, INADDR_ANY s_addr.
     * 
     * @throw runtime_error if the socket failed to initialise and we did not
     *                      receive a file descriptor or if the socket options
     *                      failed to set.
     */
    TcpServer(InternetProtocol ipv = InternetProtocol::v4){
        __errmsg = "";
        __errno = 0;
        
        clog << "Initialise new TCP server object...";

        _ipv = ipv;
        _serverFd = socket((_ipv == InternetProtocol::v4 ? AF_INET : AF_INET6), SOCK_STREAM, 0);
        if (_serverFd < 0){
            __errno = 10000 + errno;

            std::stringstream msg;
            msg << "Server socket creation failed: _serverFd: "
                << _serverFd << ". ERROR CODE: " << __errno << ".";

            flog << msg.str();
            //  This prevents the object being initialised and throws seg fault
            //  if attempting to call the object.
            throw std::runtime_error(msg.str());
        }

        const int sockOptResult =
            setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));
        if (sockOptResult < 0){
            __errno = 14000 + errno;

            std::stringstream msg;
            msg << "Socket options failed: sockOptResult: " 
                << sockOptResult << ". ERROR CODE: " << __errno << ".";

            flog << msg.str();
            //  This prevents the object being initialised and throws seg fault
            //  if attempting to call the object.
            throw std::runtime_error(msg.str());
        }

        _address.sin_family = (_ipv == InternetProtocol::v4 ? AF_INET : AF_INET6);
        _address.sin_addr.s_addr = INADDR_ANY;

        clog << "TCP server object initialised.";
    }

    /**
     * Destroy the Tcp Server object and perform necessary clean up.
     */
    ~TcpServer(){
        dlog << "TCP Server destruction...";
        this->Shutdown();
    }

    /**
     * Shuts down server socket file descriptor and resets the file
     * descriptor value.
     */
    void Shutdown(){
        dlog << "Shutting down TCP Server...";
        shutdown(_serverFd, SHUT_RDWR);
        _serverFd = -1;
    }

    /**
     * @brief   Start listening to the socket on a given port number. Sets
     *          __errmsg and __errno on error.
     * 
     * @param portNumber    The port number for this TCP server to listen on.
     * @return true         if the socket successfully started listening to the
     *                      port,
     * @return false        otherwise.
     */
    bool StartListening(const int portNumber){
        __errmsg = "";
        __errno = 0;
        
        clog << "Start listening on " << portNumber << "...";

        _address.sin_port = htons(portNumber);

        const int bindResult =
            bind(_serverFd, (struct sockaddr*)&_address, _addressLength);
        if (bindResult < 0){
            std::stringstream msg;
            msg << "Binding failed: bindResult: " << bindResult << ".";

            elog << msg.str();
            __errmsg = msg.str();
            __errno = 15000 + errno;
            return false;
        }

        const int listenResult = listen(_serverFd, _maxQueueLength);
        if (listenResult < 0){
            std::stringstream msg;
            msg << "Listen failed on port " << portNumber << ": listenResult: "
                << listenResult << ".";

            elog << msg.str();
            __errmsg = msg.str();
            __errno = 16000 + errno;
            return false;
        }

        return true;
    }

    /**
     * Return the file descriptor value for the next
     * pending connection in the queue.
     * 
     * Sets __errmsg and __errno on error.
     */
    int NextConnection(){
        __errmsg = "";
        __errno = 0;
        
        clog << "Accepting next connection in queue...";

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
        int newSocket = accept(_serverFd, (struct sockaddr*)&_address, 
                                (socklen_t*)&_addressLength);
#pragma GCC diagnostic pop

        if (newSocket < 0){
            std::stringstream msg;
            msg << "Failed to accept new connection: _serverFd: " 
                << _serverFd << ".";

            flog << msg.str();
            __errmsg = msg.str();
            __errno = 17000 + errno;
            return -1;
        }

        return newSocket;
    }

    /**
     * Set the maximum queue size for this TCP server listener. This
     * value should be set before 'StartListening' is called. Must be
     * less than 50, recommended it is at least 2.
     */
    void SetMaximumQueueSize(int len){ _maxQueueLength = (len > 50 ? 50 : len); }

    /**
     * Get this server socket's file descriptor value.
     */
    int GetSocketFd(){ return _serverFd; }

    /**
     * Get the last error message set on this object.
     */
    std::string ERR_MSG(){ return __errmsg; }

    /**
     * Get the last error code set on this object.
     */
    int ERR_NO(){ return __errno; }

private:
    /**
     * The file descriptor for this server-side socket.
     */
    int _serverFd = -1;

    /**
     * The set Internet Protocol version number (or IPv number).
     */
    InternetProtocol _ipv = InternetProtocol::v4;

    /**
     * This server socket's address structure.
     */
    struct sockaddr_in _address;

    /**
     * The byte length of the address in memory.
     */
    const int _addressLength = sizeof(_address);

    /**
     * For setting socket option value. Specified as a const integer so
     * it can be used as a reference parameter.
     */
    const int _opt = 1;

    /**
     * The maximum queue length for this server socket listener.
     */
    int _maxQueueLength = 10;
    
    /**
     * The last error message set.
     */
    std::string __errmsg;

    /**
     * The last error code set.
     */
    int __errno;
};

#endif // __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__