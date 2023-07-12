//
// Created by Dylan Andrew McAdam on 05/12/22.
//

#ifndef __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__
#define __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__

// #include <sys/socket.h>
// #include <netdb.h>
#include <netinet/in.h>

#include "dylanclibs/log.h"

class TcpServer{
public: 
    TcpServer(){
        log << "Initialise new TCP server object...";

        _serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverFd < 0){
            std::stringstream msg;
            msg << "Server socket creation failed: _serverFd: " << _serverFd << ", errno: " << errno << ".";
            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        const int sockOptResult = setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));
        if (sockOptResult < 0){
            std::stringstream msg;
            msg << "Socket options failed: sockOptResult: " << sockOptResult << ", errno: " << errno << ".";
            flog << msg.str();
            throw std::runtime_error(msg.str());
        }

        _sockAddress.sin_family = AF_INET;
        _sockAddress.sin_addr.s_addr = INADDR_ANY;

        log << "TCP server object initialised.";
    }

    ~TcpServer(){
        dlog << "TCP Server destruction...";
        this->Shutdown();
    }

    void Shutdown(){
        dlog << "Shutting down TCP Server...";
        shutdown(_serverFd, SHUT_RDWR);
        _serverFd = -1;
    }

    bool StartListening(const int portNumber){
        log << "Start listening on " << portNumber << "...";

        _sockAddress.sin_port = htons(portNumber);

        const int bindResult = bind(_serverFd, (struct sockaddr*)&_sockAddress, _addressLength);
        if (bindResult < 0){
            std::stringstream msg;
            msg << "Binding failed: bindResult: " << bindResult << ", errno: " << errno << ".";
            elog << msg.str();
            throw std::runtime_error(msg.str());
        }

        const int listenResult = listen(_serverFd, _maxQueueLength);
        if (listenResult < 0){
            std::stringstream msg;
            msg << "Listen failed on port " << portNumber << ": listenResult: " << listenResult << ", errno: " << errno << ".";
            elog << msg.str();
            __err_msg = msg.str();
            return false;
        }

        return true;
    }

    int NextConnection(){
        log << "Accepting next connection in queue...";

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
        int newSocket = accept(_serverFd, (struct sockaddr*)&_sockAddress, (socklen_t*)&_addressLength);
#pragma GCC diagnostic pop

        if (newSocket < 0){
            std::stringstream msg;
            msg << "Failed to accept new connection: _serverFd: " << _serverFd << ", errno: " << errno << ".";
            flog << msg.str();
            __err_msg = msg.str();
            return -1;
        }

        return newSocket;
    }

    void SetMaximumQueueSize(int len){
        _maxQueueLength = (len > 50 ? 50 : len);
    }

    int GetSocketFd(){ return _serverFd; }

    std::string ERROR_MSG(){
        std::string temp = __err_msg;
        __err_msg = "";
        return temp;
    }

private:
    int _serverFd = -1;

    struct sockaddr_in _sockAddress;

    const int _addressLength = sizeof(_sockAddress);

    const int _opt = 1;

    int _maxQueueLength = 10;
    
    std::string __err_msg;
};

#endif // __DYLAN_MCADAM_SINGLE_INCLUDE_CUSTOM_TCP_server_H__