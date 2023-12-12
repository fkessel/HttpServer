#pragma once

#include "Portability.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <functional>

#include <string.h>

const uint32_t bufferSize = 4096;

class Connection {
    
public:
    typedef std::function<void(Connection&)> ConnectionWorker;
    
    /*
     *  Constructors and Destructor.
     */
    Connection(int socket, ConnectionWorker f): m_socket(socket) { 
        m_thread = std::thread([f, this](){f(*this);}); 
    }
    ~Connection() { m_thread.join(); }

    /*
     *  Public methods.
     */
    void readMessage(std::string &message)
    {
        ssize_t receivedBytes = -1;
        char buffer[bufferSize];
        memset(buffer, 0, bufferSize);

        if ((receivedBytes = recv(this->m_socket, buffer, bufferSize - 1, 0)) < 0)
        {
            std::cerr << "Pöhses perror()" << std::endl;
        }
        else if (receivedBytes > 0)
        {
            buffer[receivedBytes] = '\0';
            message.assign(buffer);
        }
        std::cout << "Erhaltene Bytes: " << receivedBytes << "\n" << std::endl;
    }

    int writeMessage(const std::string& message)
    {
        ssize_t sentBytes = -1;
        if ((sentBytes = send(this->m_socket, message.c_str(), message.size(), 0)) < 0)
        {
            return -1;
        }
        return sentBytes;
    }

    void closeSocket()
    {
        CLOSESOCKET(m_socket);
        m_socket = 0;
    }

    int getSocket() const { return m_socket; };

private:
    int m_socket;
    std::thread m_thread;
};