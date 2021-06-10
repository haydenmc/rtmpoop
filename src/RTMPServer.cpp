/**
 * @file RTMPServer.cpp
 * @author Hayden McAfee (hayden@outlook.com)
 * @date 2021-06-10
 * @copyright Copyright (c) 2021 Hayden McAfee
 */

#include "RTMPServer.h"

#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include "Util.h"

namespace RTMPoop
{
#pragma region Public methods
    RTMPServer::RTMPServer(uint16_t port) : m_port(port)
    { }

    void RTMPServer::Listen()
    {
        sockaddr_in socketAddress = { 0 };
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        socketAddress.sin_port = htons(m_port);

        m_listenSocketHandle = socket(AF_INET, SOCK_STREAM, 0);
        if (m_listenSocketHandle < 0)
        {
            int error = errno;
            throw std::runtime_error(fmt::format("Unable to create listen socket! Error {}: {}",
                    error, Util::ErrnoToString(error)));
        }

        // Allow re-use so we don't get hung up trying to rebind
        int reUseOption = 1;
        if (setsockopt(m_listenSocketHandle, SOL_SOCKET, SO_REUSEADDR, &reUseOption,
            sizeof(reUseOption)) != 0)
        {
            int error = errno;
            throw std::runtime_error(fmt::format(
                "Unable to set SO_REUSEADDR on listen socket! Error {}: {}", error,
                Util::ErrnoToString(error)));
        }

        // Set the socket to non-blocking IO mode
        int socketFlags = fcntl(m_listenSocketHandle, F_GETFL, 0);
        if (socketFlags == -1)
        {
            int error = errno;
            close(m_listenSocketHandle);
            throw std::runtime_error(fmt::format(
                "Unable to get flags on listen socket! Error {}: {}", error,
                Util::ErrnoToString(error)));
        }
        socketFlags = socketFlags | O_NONBLOCK;
        if (fcntl(m_listenSocketHandle, F_SETFL, socketFlags) != 0)
        {
            int error = errno;
            close(m_listenSocketHandle);
            throw std::runtime_error(fmt::format(
                "Couldn't set listen socket to non-blocking mode! Error {}: {}", error,
                Util::ErrnoToString(error)));
        }

        int bindResult = bind(m_listenSocketHandle, (const sockaddr*)&socketAddress,
            sizeof(socketAddress));
        switch (bindResult)
        {
        case 0:
            break;
        case EADDRINUSE:
            throw std::runtime_error("Could not bind to socket, this address is already in use.");
        case EACCES:
            throw std::runtime_error("Could not bind to socket, access was denied.");
        default:
            throw std::runtime_error("Could not bind to socket.");
        }
        
        int listenResult = listen(m_listenSocketHandle, SOMAXCONN);
        switch (listenResult)
        {
        case 0:
            break;
        case EADDRINUSE:
            throw std::runtime_error("Could not listen on socket, this port is already in use.");
        default:
            throw std::runtime_error("Could not listen on socket.");
        }

        // Accept new connections and process i/o from existing connections
        epoll_event events[MAX_EPOLL_EVENTS];
        int epollFd = epoll_create1(EPOLL_CLOEXEC);
        if (epollFd == -1)
        {
            int error = errno;
            close(m_listenSocketHandle);
            throw std::runtime_error(fmt::format(
                "Couldn't create epoll file descriptor. Error {}: {}", error,
                Util::ErrnoToString(error)));
        }


        while (true)
        {
            int connectionHandle = accept(m_listenSocketHandle, nullptr, nullptr);
            if (connectionHandle == -1)
            {
                if (errno == EINVAL)
                {
                    break;
                }
                else
                {
                    spdlog::warn("Accepted invalid connection");
                }
            }
            else
            {

            }
        }
    }
#pragma endregion Public methods

#pragma region Private methods
#pragma endregion Private methods
}