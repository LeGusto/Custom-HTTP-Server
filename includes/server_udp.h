#include "server_base.h"

class ServerUDP : public Server
{
public:
    void send_msg(const char *msg, int fd) override;

    void start_server() override;

    void use_poll() override;
};