#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <cstddef>
#include "config.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <print>
#include <thread>
#include <atomic>
#include <user.h>

User::User() : user_id(user_count++) {};

void User::log(std::string s)
{
    std::print("[user {}]: {}\n", user_id, s);
}

void User::get_addrinfo()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AI_FAMILY;
    hints.ai_socktype = AI_SOCKTYPE;

    int status = getaddrinfo("localhost", PORT, &hints, &servinfo);
    if (status != 0)
    {
        throw std::runtime_error(gai_strerror(status));
    }
    servinfo_head = servinfo;
}

void User::get_socket()
{
    if ((fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}

void User::connect_socket()
{
    while (connect(fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        if (servinfo->ai_next == nullptr)
        {
            throw std::runtime_error(strerror(errno));
        }
        servinfo = servinfo->ai_next;
    }
}

void User::get_data()
{
    char buf[20];
    memset(&buf, '\0', sizeof(buf));
    send(fd, buf, 1, 0);

    int len = 0;
    recv(fd, &len, sizeof(len), MSG_WAITALL); // waitall in case partial message gets delivered
    len = ntohl(len);
    while (len > 0)
    {
        int gotten = recv(fd, &buf, sizeof(buf) - 1, 0);
        len -= gotten;
        log(buf);
        memset(&buf, '\0', sizeof(buf));
    }
}

void User::use_server()
{
    get_addrinfo();
    get_socket();
    connect_socket();
    get_data();
}

User::~User()
{
    if (servinfo_head != nullptr)
        freeaddrinfo(servinfo_head);
    if (fd != -1)
        close(fd);
}
