#include "server_base.h"

class ServerTCP : public Server
{

private:
    void listen_socket();

    void accept_socket();

public:
    void send_msg(const char *msg, int fd) override;

    void start_server() override;

    void use_poll() override;
};