/**
 * @file RTMPServer.h
 * @author Hayden McAfee (hayden@outlook.com)
 * @date 2021-06-10
 * @copyright Copyright (c) 2021 Hayden McAfee
 */

namespace RTMPoop
{
    class RTMPServer
    {
    public:
        // Public methods
        RTMPServer(uint16_t port);
        void Listen();

    private:
        // Private constants
        static constexpr uint16_t MAX_EPOLL_EVENTS = 32;

        // Private fields
        uint16_t m_port;
        int m_listenSocketHandle;
    };
}