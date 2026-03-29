#include "server_tcp.h"

void ServerTCP::listen_socket()
{
    log("Initiating listen...\n");
    if (listen(sock_desc, BACKLOG) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
    log("Server listening...\n");
}

void ServerTCP::accept_socket()
{
    int32_t new_fd = -1;
    sockaddr_storage sa{};
    socklen_t sa_len = sizeof(sa);
    log("Waiting for connection...\n");
    if ((new_fd = accept(sock_desc, reinterpret_cast<sockaddr *>(&sa), &sa_len)) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
    pfds.push_back({new_fd, POLLIN, 0});
    log("Accepted connection:\n");
    print_sockaddr(reinterpret_cast<sockaddr *>(&sa), sa_len);
}

void ServerTCP::send_msg(const char *msg, int fd)
{
    ssize_t bytes_needed = strlen(msg);
    ssize_t bytes_sent = 0;

    uint32_t net_len = htonl(bytes_needed);

    if ((bytes_sent = send(fd, &net_len, sizeof(net_len), 0)) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }

    while (bytes_needed > 0)
    {
        log("Bytes left: {}", bytes_needed);
        if ((bytes_sent = send(fd, msg, bytes_needed, 0)) == -1)
        {
            throw std::runtime_error(strerror(errno));
        }
        bytes_needed -= bytes_sent;
        msg += bytes_sent;
    }

    log("Sent\n");
}

void ServerTCP::start_server()
{
    listen_socket();
    use_poll();
}

void ServerTCP::use_poll()
{
    pfds.push_back({sock_desc, POLLIN, 0});

    while (true)
    {
        int events = poll(&pfds[0], pfds.size(), -1);

        if (pfds[0].revents & POLLIN)
        {
            accept_socket();
        }

        for (int i = 1; i < pfds.size(); i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                char buf[20];
                int bytes = recv(pfds[i].fd, buf, sizeof(buf) - 1, 0);
                if (bytes <= 0)
                {
                    std::swap(pfds[i], pfds.back());
                    close(pfds.back().fd);
                    pfds.pop_back();
                    i--;
                    continue;
                }
                send_msg("Hello", pfds[i].fd);
            }
        }
    }
}
