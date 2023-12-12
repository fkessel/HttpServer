#pragma once

#include "Connection.hpp"
#include "Portability.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <memory>

// strerror is not required to be thread-safe
// https://www.club.cc.cmu.edu/~cmccabe/blog_strerror.html

class ServerException : std::exception
{
    std::string reason_;

public:
    ServerException(const std::string &reason) : std::exception(), reason_(reason) {}
    virtual const char *what() const noexcept override { return reason_.c_str(); }
};

class Server
{

    in_port_t m_port;
    int m_domain;
    int m_protocol;
    int m_socket{-1};
    struct sockaddr_in m_servAddr
    {
        0
    };
    int m_backlogListen{1024};

public:
    // Constructor
    Server(const in_port_t port, const int domain, const int protocol = 0) : m_port(port), m_domain(domain), m_protocol(protocol)
    {
#if defined(_WIN32)
        if (!InitializeWinsock())
        {
            throw ServerException(std::string("Initialize Winsock failed."));
        }
#endif

        SocketAndBind();
        Listen();
    }

    Server(const Server &) = delete;
    Server(Server &&) = delete;
    ~Server() { Close(); }

public:
    /*
     *  Methods that are called from outside the class.
     */
    std::shared_ptr<Connection> Accept(Connection::ConnectionWorker f)
    {
        struct sockaddr_storage clientAddr;
        socklen_t lengthClient = sizeof(clientAddr);

        int clientSock = accept(m_socket, (struct sockaddr *)&clientAddr, &lengthClient);
        if (clientSock < 0)
        {
            throw ServerException(std::string("accept() failed."));
        }

        return std::make_shared<Connection>(clientSock, f);
    }

    void Close()
    {
        CLOSESOCKET(m_socket);
        m_socket = 0;
    }

private:
    /*
     *  Methods that aren't called from outside the class.
     */

    void SocketAndBind()
    {
        struct addrinfo hints, *serverInfo;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        // Necessary for a server so he can use his own IP
        hints.ai_flags = AI_PASSIVE;

        int status{0};
        if ((status = getaddrinfo(nullptr, std::to_string(m_port).c_str(), &hints, &serverInfo)) != 0)
        {
            throw ServerException(std::string("getaddrinfo() failed: ") + gai_strerror(status));
        }

        // Bind to the first result that is possible
        for (struct addrinfo *p = serverInfo; p != nullptr; p = p->ai_next)
        {
            if ((m_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("server: socket() failed");
                continue;
            }

            bool optval = true;
            // SO_REUSEADDR because we don't want to wait out the TIME_WAIT period
            // https://stackoverflow.com/a/3233022
            // Cast to (char*) because of portability to Windows
            if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) == -1)
            {
                perror("setsockopt() failed");
                exit(1);
            }

            if (bind(m_socket, p->ai_addr, p->ai_addrlen) == -1)
            {
                CLOSESOCKET(m_socket);
                perror("server: bind() failed");
                continue;
            }
            break;
        }
        freeaddrinfo(serverInfo);
    }

    void Listen()
    {
        if (listen(m_socket, m_backlogListen) < 0)
        {
            throw ServerException(std::string("listen() failed."));
        }
    }

    /*
     *  Getter
     */
    int getSocket() const { return m_socket; }
    int getDomain() const { return m_domain; }
    in_port_t getPort() const { return m_port; }
    struct sockaddr_in getSockaddr_in() const { return m_servAddr; }
};

// get sockaddr, IPv4 or IPv6:
// void* get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET) {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }
