//
// Created by Dylan Andrew McAdam on 05/12/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__

// #include <sys/socket.h>
// #include <netdb.h>
#include <netinet/in.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <arpa/inet.h>

#include "dylanclibs/log.h"

class TcpClient {
public:
    TcpClient(){
        log << "Initialise new TCP client object...";

        _socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (_socketFd < 0){
            std::stringstream msg;
            msg << "Client socket creation failed: _socketFd: " << _socketFd << ", errno: " << errno;
            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        _sockAddress.sin_family = AF_INET;

        log << "TCP socket object initialised.";
    }

    ~TcpClient(){
        dlog << "TCP Client socket fd: " << _socketFd << " and client fd: " << _clientFd << " destruction...";
        Close();
    }

    TcpClient(const int sockFd){
        log << "Storing ref to pre-existing socket file descriptor: " << sockFd << "...";
        _socketFd = sockFd;
        log << "TCP client object initialised with file descriptor: " << _socketFd << ".";
    }

    void Close(){
        dlog << "Closing socket " << _socketFd << " and client " << _clientFd << "...";
        close(_clientFd);
        close(_socketFd);
        _clientFd = -1;
        _socketFd = -1;
    }

    bool Connect(const int portNumber, const char *ip = "127.0.0.1"){
        log << "Connecting to " << ip << ":" << portNumber << "...";

        _sockAddress.sin_port = htons(portNumber);

        const int inet_result = inet_pton(AF_INET, ip, &_sockAddress.sin_addr);
        if (inet_result <= 0){
            std::stringstream msg;
            msg << "Address invalid / not supported: inet_result: " << inet_result << ", errno: " << errno;
            flog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        _clientFd = connect(_socketFd, (struct sockaddr*)&_sockAddress, _addressLength);
        if (_clientFd < 0){
            std::stringstream msg;
            msg << "Connection failed: _clientFd: " << _clientFd << ", errno: " << errno;
            flog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        log << "Connected on " << ip << ":" << portNumber << ".";
        return true;
    }

    int Read(){
        if (_socketFd < 0){
            elog << "Socket read error, tried reading without valid socket file descriptor: " << _socketFd;
            return -1;
        }

        memset(_buffer, '\0', _buffSize);
        int bytes = read(_socketFd, _buffer, _buffSize);
        if (bytes < 0){
            std::stringstream msg;
            msg << "Error reading bytes, _socketFd: " << _socketFd << ", bytes: " << bytes << ", errno: " << errno;
            elog << msg.str();
            __err_msg = msg.str();
        } else if (bytes == 0){
            wlog << "No bytes were read.";
        } else {
            log << bytes << " bytes read.";
        }

        return bytes;
    }

    int Send(const char *input){
        if (_socketFd < 0){
            elog << "Socket send error, tried sending without valid socket file descriptor: " << _socketFd;
            return -1;
        }

        int bytes = send(_socketFd, input, strlen(input), 0);
        if (bytes < 0){
            std::stringstream msg;
            msg << "Error sending bytes, _socketFd: " << _socketFd << ", bytes: " << bytes << ", errno: " << errno;
            elog << msg.str();
            __err_msg = msg.str();
        } else if (bytes == 0){
            wlog << "No bytes were sent.";
        } else {
            log << bytes << " bytes sent: " << input;
        }

        return bytes;
    }

    char *GetBuffer(){ return _buffer; }

    int GetSocketFd(){ return _socketFd; }

    int GetClientFd(){ return _clientFd; }

    std::string ERROR_MSG(){
        std::string temp = __err_msg;
        __err_msg = "";
        return temp;
    }

protected:
    /**
     * @brief The file descriptor for THIS socket object.
     */
    int _socketFd = -1;


private:
    const int _buffSize = 1024;
    char _buffer[1024];

    int _clientFd = -1;

    struct sockaddr_in _sockAddress;

    const int _addressLength = sizeof(_sockAddress);

    std::string __err_msg;
};

#endif // __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_CLIENT_H__