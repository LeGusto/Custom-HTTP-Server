#pragma once

#include "server_base.h"
#include <csignal>
#include "protocol.h"
#include "latency_tracker.h"

class ServerTCP : public Server
{

private:
    LatencyTracker tracker;
    static volatile sig_atomic_t should_exit;

    static void signal_handler(int);

    void listen_socket();

    void accept_socket();

    MessageType process_request(int fd);

    void start_server() override;

    void use_poll() override;
};