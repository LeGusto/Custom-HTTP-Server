#pragma once

#include "server_base.h"

class ServerUDP : public Server
{
public:
    void start_server() override;

    void use_poll() override;
};